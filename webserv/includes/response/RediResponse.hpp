#pragma once

#include "Response.hpp"

class RediResponse : public Response
{
public:
	RediResponse(Request &request, std::string url);
	RediResponse(int status, Request &request, std::string url);
	RediResponse(int status, Request &request, bool is_multipart);
	RediResponse(const RediResponse &other);
	RediResponse &operator=(const RediResponse &other);
	~RediResponse();

	int		generate();
	void	gen_redi_page();

	//file creation
	int		save_file(std::string filename, std::string file_body);
	int		save_mp_file(std::vector<MultipartPart> mp_vec);

	//headers:
	void redi_headers();
private:
	std::string _location;
	std::string _filename;
	bool		_is_upload;
	bool		_is_mp;
};