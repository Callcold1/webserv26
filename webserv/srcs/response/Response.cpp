#include "Response.hpp"
#include <ctime>
#include <iostream>

Response::Response(Request &request)
	: _status(200), _request(request)
{}

Response::Response(int status, Request &request)
	: _status(status), _request(request)
{}

Response::Response(const Response &other)
	: _request(other._request)
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

	s << HTTP_VERSION << " " << _status << " " << status_text() << "\r\n";

	for (HeaderMap_t::iterator it = _headers.begin(); \
			it != _headers.end(); it++)
	{
		s << it->first << ": " << it->second << "\r\n";
	}
	s << "\r\n";

	s << _body;

	return(s.str());
}

std::string Response::status_text()
{
	switch (_status)
	{
		case 200: return "OK";
		case 201: return "Resource Created";
		case 202: return "Accepted";
		case 204: return "No Content";

		case 301: return "Moved Permanently";
		case 302: return "Moved Temporarily";
		case 303: return "See Other";
		case 304: return "Not Modified";

		case 400: return "Bad Request";
		//case 401: return "Unauthorized";
		//case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";

		case 500: return "Internal Server Error";
		case 501: return "Not Implemented"; //TODO where do we check if requested method is implemented
		//case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		//etc
		default: return "Unknown Error";
	}
}

//header +header helper implementations:

void Response::defaultHeaders()
{
	_headers["Server"] = "WebServ";
	//added KM : previously "_headers["Connection"] = "keep-alive";" 
	if (_request._re.keep_alive)
		_headers["Connection"] = "keep-alive";
	else
		_headers["Connection"] = "close";
	_headers["Date"] = get_date();
	//if (!_body.empty()) // does this need to change for file uploads? Have changed to handle in get_content_len()
		_headers["Content-Length"] = to_str(get_content_len());
}

void Response::addHeader(std::string header, std::string value)
{
	_headers[header] = value;
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
	if (_body.empty()){
		//if ()
		if(_request._re.paths.upload_paths.size() == 0)
			return 0;
		else 
			return 0;
			//return file size? What if there are multiple files? (set to 0 and return Location: /upload_path/result/generated_file_with_list_of_uploads)	
	}
	return(_body.length());
}

//debug:
void Response::print_response()
{
	std::cout << this->serialize() << std::endl;
}