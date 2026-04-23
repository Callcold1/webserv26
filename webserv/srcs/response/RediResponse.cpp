#include "RediResponse.hpp"
#include "utilityFunctions.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

RediResponse::RediResponse(Request &request, std::string url) : Response(request), _location(url), _is_upload(false)
{
	_status = 301;
}

RediResponse::RediResponse(int status, Request &request, std::string url) : Response(status, request), _location(url), _is_upload(false)
{
}

RediResponse::RediResponse(int status, Request &request, bool is_multipart) : Response(status, request), _is_upload(true), _is_mp(is_multipart)
{
}

RediResponse::RediResponse(const RediResponse &other) : Response(other)
{
}


RediResponse &RediResponse::operator=(const RediResponse &other)
{
	Response::operator=(other);
	return(*this);
}

RediResponse::~RediResponse()
{}


int RediResponse::generate()
{
	int error_catch = 0;
	std::cout << "location for redi: " << _location << std::endl;
	if (_is_upload)
	{
		if (_is_mp){
			error_catch = save_mp_file();
			_headers["Content-Length"] = "0";
		}
		else
		{
			_filename = _request.findQuery("ImportFile");
			error_catch = save_file(_filename, _request._body);
		}
	}
	else
	{
		gen_redi_page();
		redi_headers();
	}
	defaultHeaders();
	return (error_catch);
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
}

void RediResponse::redi_headers()
{
	_headers["Content-Type"] = "text/html; charset=utf-8";
	_headers["Location"] = _location;
}

int RediResponse::save_file(std::string filename, std::string file_body)
{
	std::string base = filename;
	std::string ext;

	size_t dot = filename.find_last_of('.');
	if (dot != std::string::npos)
	{
		base = filename.substr(0, dot);
		ext = filename.substr(dot);
	}

	std::string file_path = _request._re.paths.upload_base + filename;
	base = _request._re.paths.upload_base + base;
	int i = 2;

	while (file_exists(file_path.c_str()))
	{
		file_path = base + "_" + toString(i) + ext;
		i++;
	}
	int fd = open(file_path.c_str(), O_CREAT | O_WRONLY | O_EXCL, 0644);
	if (fd < 0)
		return (500);
	std::cout << "aaaaaaaaaaaaaaa " << file_body.c_str() << std::endl;
	if (write(fd, file_body.c_str(), file_body.length()) < 1)
	{
		close(fd);
		return (500);
	}
	close(fd);
	_location = file_path.substr(2);
	addHeader("Location", _location);
	return (0);
}

int RediResponse::save_mp_file()
{
	std::string body;
	std::vector<std::pair<fileInfo_s, std::string> >::iterator it = _request._re.paths.upload_paths.begin();
	std::vector<std::pair<fileInfo_s, std::string> >::iterator end = _request._re.paths.upload_paths.end();
	
	for(; it != end; it++)
	{
		int copy_res = copyFile(it->second, it->first.path);
		switch (copy_res){
			case (-1) : return 500; // open src failed
			case (-2) : return 500; // open dest failed
			case (-3) : return 500; // write failed
			case (-4) : return 500; // read failed
		}
	}
	_location = getMPUploadLocation(); 
	_headers["Location"] = _location;
	return(0);
}

std::string	RediResponse::getMPUploadLocation(){
	
	size_t	size = _request._re.paths.upload_paths.size();
	std::string path;

	if (size == 1)
		std::string path = _request._re.paths.upload_paths[0].first.path.substr(_request._re.paths.upload_paths[0].first.path.find(_request._re.uri_path));
	else
		path = _request._re.uri_path;
	return path;
}