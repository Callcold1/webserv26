#include "StaticResponse.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>

#include <unistd.h>
#include <fcntl.h>

StaticResponse::StaticResponse() : Response()
{
}

StaticResponse::StaticResponse(int status) : Response(status)
{
}

StaticResponse::StaticResponse(const StaticResponse &other) : Response(other)
{}

StaticResponse &StaticResponse::operator=(const StaticResponse &other)
{
	Response::operator=(other);
	return(*this);
}

StaticResponse::~StaticResponse()
{}

int StaticResponse::generate()
{
	int re_status;
	//_path = "html/redifile.html"; //TODO change this to get actual path from response
	_path = "html";
	if ((re_status = get_file()) != 0)
		return (re_status);
	defaultHeaders();
//	static_headers();
	return (0);
}

int directory_tree(int it, std::stringstream *outfile, std::string path)
{
	std::string fullpath;
	DIR *dir = opendir(path.c_str());
	if (!dir)
	{
		//std::cerr << "Error: Directory could not be opened." << std::endl;
		return(1);
	}
	struct stat st;
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.' || std::string(entry->d_name) == "..")
			continue;
		fullpath = path + "/" + entry->d_name;
		for (int i = 0; i < it; i++)
			*outfile << "	";
		*outfile << "|--->" << "<a href=\"" << fullpath << "\">" << entry->d_name << "</a><br>";
		if (stat(fullpath.c_str(), &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
			{
				directory_tree(it + 1, outfile, fullpath);
			}
		}
	}
	closedir(dir);
	return(0);
}

int StaticResponse::gen_index_page()
{
	std::stringstream ss;

    ss << "<!doctype html>\n\
	<html>\n\
		<head>\n\
			<title>Index</title>\n\
		</head>\n\
		<body>\n\
			<h1>Index</h1>\n";
	ss << "<pre>\n";
	directory_tree(0, &ss, "html");
	ss << "<pre>\n";
	ss << "	</body>\n\
	</html>";

	_body = ss.str();
	int fd = open("Index.html", O_CREAT| O_WRONLY | O_TRUNC, 0644);
	write(fd, _body.c_str(), get_content_len());
	close(fd);
	return(0);
}

int StaticResponse::get_file()
{
	struct stat st;
 
	if (stat(_path.c_str(), &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			gen_index_page();
		}
		else if (S_ISREG(st.st_mode))
		{
			std::ifstream in(_path.c_str(), std::ios::binary);
			std::string s((std::istreambuf_iterator<char>(in)), \
            	std::istreambuf_iterator<char>());
			_body = s;
		}
	}
	else 
		return (NOT_FOUND_ERROR); //some of this can be permission denied (403) as well but this is safer
	return (0);
}

// void StaticResponse::error_headers()
// {
// 	if (_status >= 400 && _status <= 499)
// 		_headers["Connection"] = "close";
// 	//if (_status == 405)
// 	//	_headers["Allow"] = //figure out allowed methods
// 	_headers["Content-Type"] = "text/html; charset=utf-8";
// }