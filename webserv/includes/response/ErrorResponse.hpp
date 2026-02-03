#pragma once

#include "Response.hpp"

class ErrorResponse : public Response
{
public:
	ErrorResponse();
	ErrorResponse(int status);
	ErrorResponse(const ErrorResponse &other);
	ErrorResponse &operator=(const ErrorResponse &other);
	~ErrorResponse();

	void		generate();
	void		gen_error_page();

	//headers:
	void error_headers();
};