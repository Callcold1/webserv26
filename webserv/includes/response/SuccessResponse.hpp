#pragma once

#include "Response.hpp"

class SuccessResponse : public Response
{
public:
	SuccessResponse(Request &request);
	SuccessResponse(int status, Request &request);
	SuccessResponse(const SuccessResponse &other);
	SuccessResponse &operator=(const SuccessResponse &other);
	~SuccessResponse();

	int			generate();

	int			gen_index_page(bool is_gallery);
	std::string get_index_file();
	int			get_file();
	int			if_delete();

	void	static_headers();

	std::string get_content_type();
private:
	std::string _path;
	bool		_is_dir;
};