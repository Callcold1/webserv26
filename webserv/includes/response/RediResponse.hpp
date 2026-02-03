#pragma once

#include "Response.hpp"

class RediResponse : public Response
{
public:
	RediResponse();
	RediResponse(int status);
	RediResponse(const RediResponse &other);
	RediResponse &operator=(const RediResponse &other);
	~RediResponse();

	void		generate();
	void		gen_redi_page();

	//headers:
	void redi_headers();
	void get_location();
private:
	std::string _location;
};