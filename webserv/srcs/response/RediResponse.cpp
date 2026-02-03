#include "RediResponse.hpp"
// #include <unistd.h>
// #include <fcntl.h>

RediResponse::RediResponse() : Response()
{
	generate();
}

RediResponse::RediResponse(int status) : Response(status)
{
	generate();
}

RediResponse::RediResponse(const RediResponse &other) : Response(other)
{}

RediResponse &RediResponse::operator=(const RediResponse &other)
{
	Response::operator=(other);
	return(*this);
}

RediResponse::~RediResponse()
{}

void RediResponse::generate()
{
	get_location();
	gen_redi_page();
	defaultHeaders();
	redi_headers();
}

void RediResponse::gen_redi_page()
{
    _body = "<!doctype html>\n\
	<html>\n\
		<head>\n\
			<title>Moved</title>\n\
		</head>\n\
		<body>\n\
			<h1>Moved</h1>\n\
			<p>This resource has moved to\n\
			<a href=\"" + _location + "\">" + _location + "</a>.\n\
			</p>\n\
		</body>\n\
	</html>";

	// int fd = open("redifile.html", O_CREAT| O_WRONLY | O_TRUNC, 0644);
	// write(fd, _body.c_str(), get_content_len());
	// close(fd);
}

void RediResponse::redi_headers()
{
	_headers["Content-Type"] = "text/html; charset=utf-8";
	_headers["Location"] = _location;
}


//TODO figure out actual location here:
void RediResponse::get_location()
{
	_location = "https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Headers/Content-Type";
}