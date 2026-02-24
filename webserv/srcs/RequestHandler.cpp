#include <string>
#include "Request.hpp"
#include "Client.hpp"
#include "CgiResponse.hpp"
#include "ErrorResponse.hpp"
#include "StaticResponse.hpp"
#include "RediResponse.hpp"

int save_file(std::string filename, Request &request)
{
	if (filename.empty())
	filename = "unnamed";

}

int validate_part(std::string part)
{
	
}

int parse_multipart(Request &request)
{
	std::string	substr;
	int			it = 0, last_it = 0;
	std::string	full_boundary = "--" + request._boundary;

	if (request._boundary.empty())
		return(BAD_REQUEST_ERROR);

	if (last_it = request._body.find(full_boundary) != 0)
		return(BAD_REQUEST_ERROR);

	while (last_it != std::string::npos)
	{
		it = request._body.find(full_boundary);
		substr = request._body.substr(last_it, it - 1);
		validate_part(substr);
		last_it = it;
	}
}

int if_file_upload(Request &request)
{
	if (request._method == "POST")
	{
		if (request.findHeader("content-type")[0]->val == "multipart/form-data")
		{
			parse_multipart(request);
		}
		else if (request.findHeader("content-type")[0]->val.find("text/") != std::string::npos)
		{
			std::string filename = request.findQuery("ImportFile");
			save_file(filename, request);
		}
	}
		
}

void handle_request(Request &request, Client &client)
{
	int error_catch = 0;
	Response *re;
	std::string path = request._uri.path;
	
	// TODO if: look for redirec order

	if (if_file_upload(request) == 0)
	{}

	else if (path.find(CGI_PATH) != std::string::npos && path.find((CGI_PATH + std::string("assets"))) == std::string::npos)
	{
		struct stat st;
		if (stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
		{
			re = &CgiResponse(request);
			error_catch = re->generate();
			if (error_catch != 0)
			{
				re = &ErrorResponse(error_catch, request);
			}
		}
		else
		{
			re = &ErrorResponse(NOT_FOUND_ERROR, request); //again, could be 403 for directories etc but this is for security
		}
	}
	else if (path.find(HTML_PATH) != std::string::npos)
	{
		re = &StaticResponse(request);
		if ((error_catch = re->generate()) != 0)
			re = &ErrorResponse(error_catch, request);
	}
	else
		re = &ErrorResponse(NOT_FOUND_ERROR, request); //defaults to 404 not found
	client.append_write(re->serialize());

}