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

// std::string	RediResponse::getMPUploadLocation(){
	
// 	size_t	size = _request._re.paths.upload_paths.size();
// 	if (size == 1){
// 		std::string path = _request._re.paths.upload_paths[0].first.path.substr(_request._re.paths.upload_paths[0].first.path.find(_request._re.uri_path));
// 		//return _request._re.paths.upload_paths[0].first.path.substr(_request._re.paths.upload_base.length()); // return upload path - cwd
// 		//std::string path = pathAppendToRoot(_request._re.paths.upload_paths[0].second, _request._re.uri_path);
// 		return path;
// 	}
// 	else {
// 		std::string base = _request._re.paths.upload_base;
// 		std::string result_path = base + "/results/" + generate_batch_id(1); //next_folder - int that tracks filename 

// 		if (mkdir(result_path.c_str(), 0755) == 0){
// 			std::string filepath = result_path + "/" + "results.txt";
// 			std::ofstream results (filepath.c_str());
// 			for (size_t i = 0; i < size; i++)
// 				results << _request._re.paths.upload_paths[i].second.substr(_request._re.paths.resource_path.path.size()) << std::endl;
// 			results.close();
// 			return result_path;
// 		}
// 		else
//         	std::cerr << "Error creating results directory" << std::endl;
//         return "";
// 	}
// }

int RediResponse::generate()
{
	int error_catch = 0;
	if (_is_upload)
	{
		if (_is_mp){
			error_catch = save_mp_file(_request._mp_m.parts);
			// _headers["Location"] = getMPUploadLocation(); // If single file, return upload path + sanitised filename, otherwise create new page (save to)
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

	// int fd = open("redifile.html", O_CREAT| O_WRONLY | O_TRUNC, 0644);
	// write(fd, _body.c_str(), get_content_len());
	// close(fd);
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

	std::string file_path = "../photos/" + filename;
	//std::string file_path = _request._re.paths.upload_paths[0];
	base = "../photos/" + base;
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

int RediResponse::save_mp_file(std::vector<mpPart_t>& mp_vec)
{
	std::vector<mpPart_t>::iterator it;
	std::string body;

	std::cout << "MP PART COUNT: " << mp_vec.size() << std::endl;

	for (size_t i = 0; i < mp_vec.size(); i++) {
		std::cout << "Part[" << i << "] isFile: " << mp_vec[i].isFile
				<< " filename: " << mp_vec[i].filename
				<< " body size: " << mp_vec[i].body.size()
				<< std::endl;
	}
	for(it = mp_vec.begin(); it != mp_vec.end(); it++)
	{
		if ((*it).isFile)
		{
			std::cout << "save dir : " <<  it->upload_dest.path.c_str() << std::endl;
			if (!rename(it->tempFilePath.c_str(), it->upload_dest.path.c_str()))
				std::cout << "Not renamed" << std::endl;
			else
				std::cout << "Renamed" << std::endl;
			//body = std::string((*it).body.begin(), (*it).body.end());
			return (save_file((*it).filename, body));
		}
	}
	return(1);
}

// int RediResponse::save_mp_file(std::vector<mpPart_t>& mp_vec)
// {
// 	(void)mp_vec;
// 	size_t size = _request._re.paths.upload_paths.size();
	
// 	for (size_t i = 0; i < size; i++){

// 		fileInfo_s& dest = _request._re.paths.upload_paths[i].first;
// 		std::string& tmp = _request._re.paths.upload_paths[i].second;
		
// 		size_t num = 1;													// number to append

// 		std::string name = dest.path;

// 		while (42){
// 			int i = std::rename(tmp.c_str(), name.c_str());
// 			perror("Rename failed : ");
// 			std::cout << i << std::endl;
// 			if (std::rename(tmp.c_str(), name.c_str()) == 0){
// 				dest.path = name;
// 				std::cout << "file successfully saved to upload folder as : " << name << std::endl;
// 				break;
// 			}

// 			std::cerr << "Existing file with same name : file renamed" << std::endl;
// 			name = dest.path + "_" + toString(num);
// 			num++;
// 		}
// 	}
// 	return 0;
// 	/* 
// 		std::vector<mpPart_t>::iterator it;
// 		std::string body; // If it's a file, see below 

// 		for(it = mp_vec.begin(); it != mp_vec.end(); it++)
// 		{
// 			if ((*it).isFile)
// 			{
// 				body = std::string((*it).body.begin(), (*it).body.end());
// 				return (save_file((*it).filename, body)); // If we have to handle more than one file? If so, should this iterate until end
// 			}
// 		}
// 		return(1);
// 	*/
// }
