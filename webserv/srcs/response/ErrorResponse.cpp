#include "ErrorResponse.hpp"
//#include <unistd.h>
//#include <fcntl.h>

ErrorResponse::ErrorResponse() : Response()
{
	generate();
}

ErrorResponse::ErrorResponse(int status) : Response(status)
{
	generate();
}

ErrorResponse::ErrorResponse(const ErrorResponse &other) : Response(other)
{}

ErrorResponse &ErrorResponse::operator=(const ErrorResponse &other)
{
	Response::operator=(other);
	return(*this);
}

ErrorResponse::~ErrorResponse()
{}

void ErrorResponse::generate()
{
	gen_error_page();
	defaultHeaders();
	error_headers();
}

void ErrorResponse::gen_error_page()
{
    std::string title = to_str(_status) + ": " + status_text();

    _body = "<!doctype html>\n\
	<html>\n\
		<head>\n\
			<title>" + title + "</title>\n\
		</head>\n\
		<body>\n\
			<h1>" + title + "</h1>\n\
			<p>Something went wrong.</p>\n\
		</body>\n\
	</html>";

	//int fd = open("file.html", O_CREAT| O_WRONLY | O_TRUNC, 0644);
	//write(fd, _body.c_str(), get_content_len());
	//close(fd);
}

void ErrorResponse::error_headers()
{
	if (_status >= 400 && _status <= 499)
		_headers["Connection"] = "close";
	//if (_status == 405)
	//	_headers["Allow"] = //figure out allowed methods
	_headers["Content-Type"] = "text/html; charset=utf-8";
}