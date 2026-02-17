#pragma once

#include "Response.hpp"

class CgiResponse : public Response
{
public:
	CgiResponse();
	CgiResponse(int status);
	CgiResponse(const CgiResponse &other);
	CgiResponse &operator=(const CgiResponse &other);
	~CgiResponse();

	int		generate();
	int		execute();
	void	cgi_headers();

private:
	std::string _path;
};