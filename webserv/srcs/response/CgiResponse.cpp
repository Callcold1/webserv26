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
	_path = _request._uri.path;
	if (execute() != 0)
	{
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

		char *args[] = {
			const_cast<char*>(_path.c_str()),
			NULL
		};

		const char *envp[] = {
			_request._uri.query.c_str(),
			NULL
		};
		execve(_path.c_str(), args, const_cast<char * const *>(envp)); 
		_exit(1);
	}
	else //parent
	{
		close(toServer[1]);
		close(toCgi[0]);

		// if (write(toCgi[1], _request, sizeof(_request)) < 0)
		// 	{
		// 		close(toCgi[1]); close(toServer[0]);
		// 		waitpid(pid, NULL, 0);
		// 		return(3);
		// 	}
		
		char buf[2048];
		std::string img;
		ssize_t bytes_read;
		ssize_t total_bytes = 0;
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
		close(toCgi[1]);
		waitpid(pid, NULL, 0);
	}
	return(0);
}

void CgiResponse::cgi_headers()
{
	_headers["Content-Type"] = "image/png";
}