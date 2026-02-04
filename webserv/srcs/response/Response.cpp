#include "Response.hpp"
#include <ctime>
#include <iostream>

Response::Response()
	: _status(200)
{}

Response::Response(int status)
	: _status(status)
{}

Response::Response(const Response &other)
{
	_status = other._status;
	_headers.insert(other._headers.begin(), other._headers.end());
	_body = other._body;
}

Response &Response::operator=(const Response &other)
{
	_status = other._status;
	_headers.clear();
	_headers.insert(other._headers.begin(), other._headers.end());
	_body = other._body;
	return(*this);
}

Response::~Response()
{}

std::string Response::serialize()
{
	std::stringstream s;

	s << HTTP_VERSION << " " << _status << " " << status_text() << "/r/n";

	for (HeaderMap_t::iterator it = _headers.begin(); \
			it != _headers.end(); ++it)
	{
		s << it->first << ": " << it->second << "/r/n";
	}
	s << "/r/n/r/n";

	s << _body;

	return(s.str());
}

std::string Response::status_text()
{
	switch (_status)
	{
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 204: return "No Content";

		case 301: return "Moved Permanently";
		case 302: return "Moved Temporarily";
		case 304: return "Not Modified";

		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";

		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		//etc
		default: return "Unknown Error";
	}
}

//header +header helper implementations:

void Response::defaultHeaders()
{
	_headers["Server"] = "WebServ";
	_headers["Connection"] = "keep-alive";
	_headers["Date"] = get_date();
	_headers["Content-Length"] = to_str(get_content_len());
}

std::string Response::get_date()
{
	std::time_t now = std::time(NULL);
    std::tm gmt;

    gmt = *std::gmtime(&now);

    char buf[32];
    std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &gmt);

    return std::string(buf);
}

size_t Response::get_content_len()
{
	return(_body.size());
}

//debug:
void Response::print_response()
{
	std::cout << this->serialize() << std::endl;
}