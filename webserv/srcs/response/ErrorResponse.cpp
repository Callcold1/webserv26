#include "ErrorResponse.hpp"
#include <unistd.h>
#include <fcntl.h>

ErrorResponse::ErrorResponse(Request &request) : Response(request), err_msg("Something went wrong")
{
	generate();
}

ErrorResponse::ErrorResponse(int status, Request &request) : Response(status, request), err_msg("Something went wrong")
{
	generate();
}

ErrorResponse::ErrorResponse(int status, Request &request, std::string custom_err_msg) : Response(status, request), err_msg(custom_err_msg)
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

int ErrorResponse::generate()
{
	gen_error_page();
	defaultHeaders();
	error_headers();
	return (0);
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
			<p>" + err_msg + "</p>\n\
		</body>\n\
	</html>";

	int fd = open("errfile.html", O_CREAT| O_WRONLY | O_TRUNC, 0644);
	write(fd, _body.c_str(), get_content_len());
	close(fd);
}

void ErrorResponse::error_headers()
{
	if (_status >= 400 && _status <= 499)
		_headers["Connection"] = "close";
	//if (_status == 405)
	//	_headers["Allow"] = createArgsString(request.matched_location.getMethod())
	if (_status == 429 || _status == 503) //429 = too many requests; 503 service unavailable
		 _headers["Retry-After"] = "60";
	_headers["Content-Type"] = "text/html; charset=utf-8";
}