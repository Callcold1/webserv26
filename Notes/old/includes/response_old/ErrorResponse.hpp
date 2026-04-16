#pragma once

#include "Response.hpp"

class ErrorResponse : public Response
{
public:
	ErrorResponse(Request &request);
	ErrorResponse(int status, Request &request);
	ErrorResponse(int status, Request &request, std::string err_msg);
	ErrorResponse(const ErrorResponse &other);
	ErrorResponse &operator=(const ErrorResponse &other);
	~ErrorResponse();

	int		generate();
	void	gen_error_page();

	//headers:
	void error_headers();
private:
	std::string err_msg;
};