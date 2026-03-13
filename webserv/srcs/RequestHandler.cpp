#include <string>
#include "Request.hpp"
#include "Client.hpp"
#include "CgiResponse.hpp"
#include "ErrorResponse.hpp"
#include "StaticResponse.hpp"
#include "RediResponse.hpp"


bool file_exists(const char* filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

int save_file(std::string filename, std::string file_body)
{
	std::string base = filename;
	std::string ext;

	size_t dot = filename.find_last_of('.');
	if (dot != std::string::npos)
	{
		base = filename.substr(0, dot);
		ext = filename.substr(dot);
	}

	std::string file_path = "../photos/" + filename;
	base = "../photos/" + base;
	int i = 2;

	while (file_exists(file_path.c_str()))
	{
		file_path = base + "_" + std::to_string(i) + ext;
		i++;
	}
	
	int fd = open(file_path.c_str(), O_CREAT | O_WRONLY | O_EXCL, 0644);
	if (fd < 0)
		return (500);
	if (write(fd, file_body.c_str(), file_body.length()) < 1)
	{
		close(fd);
		return (500);
	}
	close(fd);
	return (0);
}

int save_mp_file(std::vector<MultipartPart> mp_vec)
{
	std::vector<MultipartPart>::iterator it;
	std::string body;

	for(it = mp_vec.begin(); it != mp_vec.end(); it++)
	{
		if ((*it).isFile)
		{
			body = std::string((*it).content.begin(), (*it).content.end());
			return (save_file((*it).filename, body));
		}
	}
	return(1);
}

int if_file_upload(Request &request)
{
	if (request._method == "POST")
	{
		std::vector<field_t* > header = request.findHeader("content-type");
		if (header.empty())
		{
			return (1);
		}
		else if (header[0]->val.find("multipart/form-data"))
		{
			return(save_mp_file(request._multipart.parts));
		}
		else if (header[0]->val.find("image/png") != std::string::npos)
		{
			std::string filename = request.findQuery("ImportFile");
			return (save_file(filename, request._body));
		}
	}
	return (1);
}

void Client::handle_request()
{
	int error_catch = 0;
	std::string path = request._uri.path;

	if (!_matched_location->getRedirect().url.empty())
	{
		response = new RediResponse(request, _matched_location->getRedirect().url);
		response->generate();
	}

	else if ((error_catch = if_file_upload(request)) != 1)
	{
		// TODO generate response for successful file upload
		if (error_catch >= 400)
			response = new ErrorResponse(error_catch, request);
	}

	else if (path.find(CGI_PATH) != std::string::npos && path.find((CGI_PATH + std::string("assets"))) == std::string::npos)
	{
		struct stat st;
		if (stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
		{
			response =  new CgiResponse(request);
			error_catch = response->generate();
			if (error_catch != 0)
			{
				delete response;
				response = new ErrorResponse(error_catch, request);
			}
		}
		else
		{
			response = new ErrorResponse(NOT_FOUND_ERROR, request); //again, could be 403 for directories etc but this is for security
		}
	}
	else if (path.find(HTML_PATH) != std::string::npos || path.find(PHOTO_PATH) != std::string::npos)
	{
		response = new StaticResponse(request);
		if ((error_catch = response->generate()) != 0)
		{
			delete response;
			response = new ErrorResponse(error_catch, request);
		}
	}
	else
		response =  new ErrorResponse(NOT_FOUND_ERROR, request); //defaults to 404 not found
	append_write(response->serialize());
	delete response;
}