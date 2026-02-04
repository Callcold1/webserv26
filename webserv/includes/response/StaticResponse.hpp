#pragma once

#include "Response.hpp"

class StaticResponse : public Response
{
public:
	StaticResponse();
	StaticResponse(int status);
	StaticResponse(const StaticResponse &other);
	StaticResponse &operator=(const StaticResponse &other);
	~StaticResponse();

	int		generate();

	int		gen_index_page();
	int		get_file();

	void	static_headers();
private:
	std::string _path; 
};