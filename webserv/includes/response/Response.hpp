#pragma once

#include <map>
#include <string>
#include <iterator>
#include <sstream>
#include "helpers.hpp"

#define HTTP_VERSION "HTTP/1.1"

typedef std::map<std::string, std::string> HeaderMap_t;

class Response
{
public:
	Response();
	Response(int status);
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();

	virtual void	generate() = 0;
	std::string		status_text();
	std::string		serialize();

	//headers:
	void			defaultHeaders();

	std::string		get_date();
	size_t			get_content_len();

	//debug:
	void print_response();

protected:
	int			_status;
	HeaderMap_t _headers;
	std::string _body;
	//Request		_request;
};