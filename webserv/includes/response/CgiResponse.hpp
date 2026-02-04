#pragma once

#include "Response.hpp"

class CgiResponse : public Response
{
public:
	CgiResponse();
	CgiResponse(const CgiResponse &other);
	CgiResponse &operator=(const CgiResponse &other);
	~CgiResponse();

	int	generate();
	
};