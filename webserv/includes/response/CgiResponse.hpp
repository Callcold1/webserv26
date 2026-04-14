#pragma once

#include "Response.hpp"

class CgiResponse : public Response
{
public:
	CgiResponse(Request &request, bool is_python);
	CgiResponse(int status, Request &request, bool is_python);
	CgiResponse(const CgiResponse &other);
	CgiResponse &operator=(const CgiResponse &other);
	~CgiResponse();

	int		generate();
	int		execute();
	void	cgi_headers();

private:
	std::string _path;
	bool		_is_py;
};