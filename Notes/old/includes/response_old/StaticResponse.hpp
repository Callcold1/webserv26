#pragma once

#include "Response.hpp"

class StaticResponse : public Response
{
public:
	StaticResponse(Request &request);
	StaticResponse(int status, Request &request);
	StaticResponse(const StaticResponse &other);
	StaticResponse &operator=(const StaticResponse &other);
	~StaticResponse();

	int		generate();

	int		gen_index_page();
	int		get_file();

	void	static_headers();

	std::string get_content_type();
private:
	std::string _path;
	bool		_is_dir;
};