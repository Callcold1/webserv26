#include <string>
#include "Request.hpp"
#include "Client.hpp"
#include "CgiResponse.hpp"
#include "ErrorResponse.hpp"
#include "StaticResponse.hpp"
#include "RediResponse.hpp"

void handle_request(Request &request, Client &client)
{
	int error_catch = 0;
	Response *re;
	std::string path = request._uri.path;
	//look for redirec order
	if (path.find("www/cgi-bin/") != std::string::npos && path.find("www/cgi-bin/assets") == std::string::npos)
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
				re = &ErrorResponse(NOT_FOUND_ERROR, request); //again, could be 403 for directories etc but security
			}
	}
	if (path.find("www/html") != std::string::npos)
	{
		re = &StaticResponse(request);
		if ((error_catch = re->generate()) != 0)
			re = &ErrorResponse(error_catch, request);
	}
	else
		re = &ErrorResponse(???, request); //TODO: figure out which is default response, probably not found in this case though
	client.append_write(re->serialize());

}