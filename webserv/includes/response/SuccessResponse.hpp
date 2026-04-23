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
	int			get_index_file();\
	void		serve_file();
	int			get_file();
	int			if_delete();

	void	static_headers();

private:
	std::string _path;
	bool		_is_dir;
};