#pragma once

#include <map>
#include <string>
#include <iterator>
#include <sstream>

#define HTTP_VERSION "HTTP/1.1"

class Response
{
public:
	Response();
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();

	virtual void	generate() = 0;
	std::string		serialize();
	std::string		status_text(); 

protected:
	int status_code;
	std::map<std::string,std::string> headers;
	std::string body;
};