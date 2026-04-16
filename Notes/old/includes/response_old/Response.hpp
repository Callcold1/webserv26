#pragma once

#include <map>
#include <string>
#include <iterator>
#include <sstream>
#include <Request.hpp>
#include "helpers.hpp"

#define HTTP_VERSION "HTTP/1.1"
#define CGI_PATH "www/cgi-bin/"
#define HTML_PATH "/html"
#define PHOTO_PATH "www/photos"
#define NOT_FOUND_ERROR 404
#define BAD_REQUEST_ERROR 400
#define FILE_UPLOAD_REDIREC 303

typedef std::map<std::string, std::string> HeaderMap_t;

class Response
{
public:
	Response();
	Response(Request &request);
	Response(int status, Request &request);
	Response(const Response &other);
	Response &operator=(const Response &other);
	virtual ~Response();

	virtual int		generate() = 0;
	std::string		status_text();
	std::string		serialize();

	//headers:
	void			defaultHeaders();
	void			addHeader(std::string header, std::string value);

	std::string		get_date();
	size_t			get_content_len();

	//debug:
	void print_response();
	int			_status;
	Request&	_request;

protected:
	HeaderMap_t _headers;
	std::string _body;
	fileInfo_s _path_km;
};