#include "RediResponse.hpp"
#include <unistd.h>
#include <fcntl.h>

RediResponse::RediResponse(Request &request, std::string url) : Response(request), _location(url)
{
	_status = 301;
}

RediResponse::RediResponse(int status, Request &request, std::string url) : Response(status, request), _location(url)
{
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

int RediResponse::generate()
{
	gen_redi_page();
	defaultHeaders();
	redi_headers();
	return (0);
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
