#include "CgiResponse.hpp"
#include <unistd.h>
#include <sys/wait.h>


CgiResponse::CgiResponse() : Response()
{
}

CgiResponse::CgiResponse(int status) : Response(status)
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
	_path = "www/cgi-bin/meme.py";
	if (execute() != 0)
	{
		return(500);
	}
	defaultHeaders();
	cgi_headers();
	return(0);
}

int CgiResponse::execute()
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
			"jimtext=sryj",
			"dwighttext=srthaze",
			"michaeltext=sethser",
			NULL
		};
		execve(_path.c_str(), args, const_cast<char * const *>(envp)); //get env, especially the ones from query string
		_exit(1);
	}
	else //parent
	{
		close(toServer[1]);
		close(toCgi[0]);

		// if (write(toCgi[1], _request, sizeof(_request)) < 0)
		// 	{
		// 		close(toCgi[1]); close(toServer[0]);
		// 		waitpid(0, NULL, 0);
		// 		return(3);
		// 	}
		close(toCgi[1]);
		
		char *buf[2048];
		if (read(toServer[0], buf, 2048) < 0)
			{
				close(toCgi[1]); close(toServer[0]);
				waitpid(0, NULL, 0);
				return(4);
			}
		close(toServer[0]);
		waitpid(0, NULL, 0);
	}
	return(0);
}

void CgiResponse::cgi_headers()
{
	_headers["Content-Type"] = "image/png";
}