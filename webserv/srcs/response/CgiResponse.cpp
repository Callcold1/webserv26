#include "CgiResponse.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>


CgiResponse::CgiResponse(Request &request) : Response(request)
{
}

CgiResponse::CgiResponse(int status, Request &request) : Response(status, request)
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
	_path = _request.get_root_dir() + _request._uri.path;
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

		close(toCgi[0]); close(toServer[1]);

		const char* env_path = "env";

		char *args[] = {
			const_cast<char*>(env_path),
			(char*)"python3",               
			const_cast<char*>(_path.c_str()), 
			NULL
		};

		char* system_path = getenv("PATH"); // get the current system PATH so we don't have to hardcode it
		std::string path_env = "PATH=" + std::string(system_path ? system_path : "/usr/bin:/bin");
		std::string q = "QUERY_STRING=" + _request._uri.query;
		std::string len = "CONTENT_LENGTH=" + std::to_string(_request._body.length());
		std::string type = "CONTENT_TYPE=application/x-www-form-urlencoded"; // or from headers
		std::string m = "REQUEST_METHOD=" + _request._method;

		char* envp[] = {
			const_cast<char*>(q.c_str()),
			const_cast<char*>(m.c_str()),
			const_cast<char*>(len.c_str()),
			const_cast<char*>(type.c_str()),
			const_cast<char*>(path_env.c_str()), 
			NULL
		};
		execve("/usr/bin/env", args, const_cast<char * const *>(envp)); 
		_exit(1);
	}
	else //parent
	{
		char buf[2048];
		std::string img;
		ssize_t bytes_read;
		ssize_t total_bytes = 0;

		close(toServer[1]);
		close(toCgi[0]);

		if (_request._method == "POST" && !_request._body.empty()) {
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
			img.append(buf, bytes_read);
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
			return(4);
		}
		int fd = open("outfile.png", O_CREAT| O_WRONLY | O_TRUNC, 0644);
		write(fd, img.c_str(), total_bytes);
		close(fd);
		_body = img;
		close(toServer[0]);
		waitpid(pid, NULL, 0);
		return (0);
	}
}

// int CgiResponse::execute() //TODO maybe remove one of the pipes 
// {
// 	int toCgi[2];
// 	int toServer[2];

// 	if (pipe(toCgi) < 0)
// 		return(1);
// 	if (pipe(toServer) < 0)
// 	{
// 		close(toCgi[0]);
// 		close(toCgi[1]);
// 		return(1);
// 	}

// 	pid_t pid;
// 	if ((pid = fork()) < 0) //failure
// 	{
// 		close(toCgi[0]);
// 		close(toCgi[1]);
// 		close(toServer[0]);
// 		close(toServer[1]);
// 		return(2);
// 	}
// 	if (pid == 0) //child
// 	{
// 		close(toServer[0]); close(toCgi[1]);

// 		dup2(toCgi[0], STDIN_FILENO);    //POST request data 
// 		dup2(toServer[1], STDOUT_FILENO); //PNG bytes 

// 		close(toCgi[0]); close(toServer[1]);

// 		char *args[] = {
// 			const_cast<char*>(_path.c_str()),
// 			NULL
// 		};

// 		const char *envp[] = {
// 			_request._uri.query.c_str(),
// 			NULL
// 		};
// 		execve(_path.c_str(), args, const_cast<char * const *>(envp)); 
// 		_exit(1);
// 	}
// 	else //parent
// 	{
// 		close(toServer[1]);
// 		close(toCgi[0]);

// 		// if (write(toCgi[1], _request, sizeof(_request)) < 0)
// 		// 	{
// 		// 		close(toCgi[1]); close(toServer[0]);
// 		// 		waitpid(pid, NULL, 0);
// 		// 		return(3);
// 		// 	}
		
// 		char buf[2048];
// 		std::string img;
// 		ssize_t bytes_read;
// 		ssize_t total_bytes = 0;
// 		while ((bytes_read = read(toServer[0], buf, 2048)) > 0)
// 		{
// 			img.append(buf, bytes_read);
// 			total_bytes += bytes_read;
// 		}
// 		if (bytes_read < 0)
// 		{
// 			close(toCgi[1]); close(toServer[0]);
// 			waitpid(pid, NULL, 0);
// 			return(4);
// 		}
// 		if (total_bytes <= 0)
// 		{
// 			close(toCgi[1]); close(toServer[0]);
// 			waitpid(pid, NULL, 0);
// 			return(4);
// 		}
// 		int fd = open("outfile.png", O_CREAT| O_WRONLY | O_TRUNC, 0644);
// 		write(fd, img.c_str(), total_bytes);
// 		close(fd);
// 		_body = img;
// 		close(toServer[0]);
// 		close(toCgi[1]);
// 		waitpid(pid, NULL, 0);
// 	}
// }

// int CgiResponse::execute()
// {
// 	int toServer[2];

// 	if (pipe(toServer) < 0)
// 		return(1);

// 	pid_t pid;
// 	if ((pid = fork()) < 0) //fork failure
// 	{
// 		close(toServer[0]);
// 		close(toServer[1]);
// 		return(2);
// 	}
// 	if (pid == 0) //child
// 	{
// 		close(toServer[0]);

// 		dup2(toServer[1], STDOUT_FILENO); //PNG bytes 

// 		close(toServer[1]);

// 		const char* env_path = "/usr/bin/env";

// 		char *args[] = {
// 			const_cast<char*>(env_path),
// 			(char*)"python3",               
// 			const_cast<char*>(_path.c_str()), 
// 			NULL
// 		};

		
// 		char* system_path = getenv("PATH"); // get the current system PATH so we don't have to hardcode it
// 		std::string path_env = "PATH=" + std::string(system_path ? system_path : "/usr/bin:/bin");
// 		std::string q;
// 		if (_request._method == "POST")
// 			q = "QUERY_STRING=" + _request._body;
// 		else if (_request._method == "GET")
// 			q = "QUERY_STRING=" + _request._uri.query;
// 		std::cout << _request._body << std::endl;

// 		std::string m = "REQUEST_METHOD=GET"; //+ _request._method;

// 		char* envp[] = {
// 			const_cast<char*>(q.c_str()),
// 			const_cast<char*>(m.c_str()),
// 			const_cast<char*>(path_env.c_str()), 
// 			NULL
// 		};
// 		execve(_path.c_str(), args, const_cast<char * const *>(envp));
// 		_exit(1);
// 	}
// 	else //parent
// 	{
// 		char buf[2048];
// 		std::string img;
// 		ssize_t bytes_read;
// 		ssize_t total_bytes = 0;

// 		close(toServer[1]);
		
// 		while ((bytes_read = read(toServer[0], buf, 2048)) > 0)
// 		{
// 			img.append(buf, bytes_read);
// 			total_bytes += bytes_read;
// 		}
// 		if (bytes_read < 0)
// 		{
// 			close(toServer[0]);
// 			waitpid(pid, NULL, 0);
// 			return(4);
// 		}
// 		if (total_bytes <= 0)
// 		{
// 			close(toServer[0]);
// 			waitpid(pid, NULL, 0);
// 			return(5);
// 		}
// 		// int fd = open("outfile.png", O_CREAT| O_WRONLY | O_TRUNC, 0644);
// 		// write(fd, img.c_str(), total_bytes);
// 		// close(fd);
// 		_body = img;
// 		close(toServer[0]);
// 		waitpid(pid, NULL, 0);
// 	}
// 	return(0);
// }

void CgiResponse::cgi_headers()
{
	_headers["Content-Type"] = "image/png";
}