#pragma once

#include "Response.hpp"

class RediResponse : public Response
{
public:
	RediResponse(Request &request);
	RediResponse(int status, Request &request);
	RediResponse(const RediResponse &other);
	RediResponse &operator=(const RediResponse &other);
	~RediResponse();

	int		generate();
	void	gen_redi_page();

	//headers:
	void redi_headers();
	void get_location();
private:
	std::string _location;
};