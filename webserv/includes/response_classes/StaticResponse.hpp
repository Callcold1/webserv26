#pragma once

#include "Response.hpp"

class StaticResponse : public Response
{
public:
	StaticResponse();
	StaticResponse(const StaticResponse &other);
	StaticResponse &operator=(const StaticResponse &other);
	~StaticResponse();

	void	generate();
	
};