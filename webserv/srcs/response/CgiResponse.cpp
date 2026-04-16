#include "CgiResponse.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>


CgiResponse::CgiResponse(Request &request, bool is_python) : Response(request), _is_py(is_python)
{
}

CgiResponse::CgiResponse(int status, Request &request, bool is_python) : Response(status, request), _is_py(is_python)
{
}

CgiResponse::CgiResponse(const CgiResponse &other) : Response(other)
{}

CgiResponse &CgiResponse::operator=(const CgiResponse &other)
{
	Response::operator=(other);
	return(*this);
}

CgiResponse::~CgiResponse()
{}

int	CgiResponse::generate()
{
	_path = _request._re.paths.resource_path.path;
	std::cout << "entered cgi repone: " << _path << std::endl;
	int i = 0; //debug
	if ((i = execute()) != 0)
	{
		std::cout << i << std::endl;
		return(500);
	}
	defaultHeaders();
	cgi_headers();
	return(0);
}

int CgiResponse::execute() //TODO maybe remove one of the pipes
{
	int toCgi[2];
	int toServer[2];

	if (pipe(toCgi) < 0)
		return(1);
	if (pipe(toServer) < 0)
	{
		close(toCgi[0]);
		close(toCgi[1]);
		return(1);
	}
	std::cout << "query; " << _request._uri.query << std::endl;
	// std::string abs_path = get_absolute_path(_path);
	// if (abs_path.empty())
	// {
	//		std::cout << " no target path" << std::endl;
	//		return (500);
	// }
	// std::cout << "target path" << abs_path << std::endl;

	pid_t pid;
	if ((pid = fork()) < 0) //failure
	{
		close(toCgi[0]);
		close(toCgi[1]);
		close(toServer[0]);
		close(toServer[1]);
		return(2);
	}
	if (pid == 0) //child
	{
		close(toServer[0]); close(toCgi[1]);

		dup2(toCgi[0], STDIN_FILENO);    //POST request data
		dup2(toServer[1], STDOUT_FILENO); //PNG bytes
		dup2(toServer[1], STDERR_FILENO);

		close(toCgi[0]); close(toServer[1]);

		std::string interpreter = "python3";
		if (!_is_py)
			interpreter = "php-cgi";

		char *args[] = {
			const_cast<char*>("env"),
			const_cast<char*>(interpreter.c_str()),
			const_cast<char*>(_path.c_str()),
			NULL
		};

		char* system_path = getenv("PATH"); // current system PATH so we dont have to hardcode it
		std::string path_env = "PATH=" + std::string(system_path ? system_path : "/usr/bin:/bin");
		std::string q = "QUERY_STRING=" + _request._uri.query;
		std::string len = "CONTENT_LENGTH=" + toString(_request._body.length());
		std::string type = "CONTENT_TYPE=application/x-www-form-urlencoded"; // or from headers
		std::string m = "REQUEST_METHOD=" + _request._method;
		std::string script = "SCRIPT_FILENAME=" + _path; // system path to file
		std::string script_name = "SCRIPT_NAME=" + _path;//_request._uri.path;
		std::string redirect = "REDIRECT_STATUS=200";    // security handshake for PHP
		std::string gateway = "GATEWAY_INTERFACE=CGI/1.1";
		std::string protocol = "SERVER_PROTOCOL=HTTP/1.1";

		char* envp[] = {
			const_cast<char*>(q.c_str()),
			const_cast<char*>(m.c_str()),
			const_cast<char*>(len.c_str()),
			const_cast<char*>(type.c_str()),
			const_cast<char*>(path_env.c_str()),
			const_cast<char*>(script.c_str()),
			const_cast<char*>(script_name.c_str()),
			const_cast<char*>(redirect.c_str()),
			const_cast<char*>(gateway.c_str()),
			const_cast<char*>(protocol.c_str()),
			NULL
		};
		//std::string interpreter_path = "/usr/bin/python3";
		std::string interpreter_path = "~/.local/lib/python3.13/site-packages";
		if (!_is_py)
			interpreter_path = "/usr/bin/php-cgi";
			//interpreter_path = "/usr/local/bin/php-cgi";
		//execve(interpreter_path.c_str(), args, const_cast<char * const *>(envp));
		execve("/usr/bin/env", args, envp);
		_exit(1);
	}
	else //parent
	{
		char buf[2048];
		std::string res;
		ssize_t bytes_read;
		ssize_t total_bytes = 0;

		close(toServer[1]);
		close(toCgi[0]);

		if ((!_request._body.empty())) {
			if (write(toCgi[1], _request._body.c_str(), _request._body.length()) < 0)
			{
				close(toCgi[1]); close(toServer[0]);
				waitpid(pid, NULL, 0);
				return(3);
			}
		}
		close(toCgi[1]);

		while ((bytes_read = read(toServer[0], buf, 2048)) > 0)
		{
			res.append(buf, bytes_read);
			total_bytes += bytes_read;
		}
		if (bytes_read < 0)
		{
			close(toCgi[1]); close(toServer[0]);
			waitpid(pid, NULL, 0);
			return(4);
		}
		if (total_bytes <= 0)
		{
			close(toCgi[1]); close(toServer[0]);
			waitpid(pid, NULL, 0);
			return(5);
		}
		if (!_is_py)
		{
			size_t pos;
			if ((pos = res.rfind("\r\n\r\n")) != std::string::npos)
			{
				_body = res.substr(pos + 4);
			}
			else if ((pos = res.rfind("\n\n")) != std::string::npos)
			{
				_body = res.substr(pos + 2);
			}
			else
			{
				_body = res;
			}
		}
		else
			_body = res;

		int fd = open("outfile.html", O_CREAT| O_WRONLY | O_TRUNC, 0644);
		write(fd, res.c_str(), total_bytes);
		close(fd);
		close(toServer[0]);
		waitpid(pid, NULL, 0);
		return (0);
	}
}

void CgiResponse::cgi_headers()
{
	if (_is_py)
		_headers["Content-Type"] = "image/png";
	if (!_is_py)
		_headers["Content-Type"] = "text/html";
}