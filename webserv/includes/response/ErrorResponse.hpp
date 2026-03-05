#pragma once

#include "Response.hpp"

class ErrorResponse : public Response
{
public:
	ErrorResponse(Request &request);
	ErrorResponse(int status, Request &request);
	ErrorResponse(const ErrorResponse &other);
	ErrorResponse &operator=(const ErrorResponse &other);
	~ErrorResponse();

	int		generate();
	void	gen_error_page();

	//headers:
	void error_headers();
};