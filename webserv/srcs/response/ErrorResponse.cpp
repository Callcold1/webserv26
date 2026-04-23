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
	_request.updateErrorPagePath(_status);
	std::cout << _request._re.paths.error_path.is_file << std::endl;
	std::cout << _request._re.paths.error_path.inaccessible << std::endl;
	std::cout << _request._re.paths.error_path.path << std::endl;
	if (_request._re.paths.error_path.is_file && !_request._re.paths.error_path.inaccessible)
	{
		std::cout << "error page found" << std::endl;
		get_file(_request._re.paths.error_path.path);
	}
	else 
		gen_error_page();
	defaultHeaders();
	error_headers();
	return (0);
}

void ErrorResponse::get_file(std::string path)
{
	std::ifstream in(path.c_str(), std::ios::binary);
	std::string s((std::istreambuf_iterator<char>(in)), \
    	std::istreambuf_iterator<char>());
	_body = s;
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
}

void ErrorResponse::error_headers()
{
	if (_status >= 400 && _status <= 499)
		_headers["Connection"] = "close";
	if (_status == 429 || _status == 503) //429 = too many requests; 503 service unavailable
		 _headers["Retry-After"] = "60";
	_headers["Content-Type"] = "text/html; charset=utf-8";
}