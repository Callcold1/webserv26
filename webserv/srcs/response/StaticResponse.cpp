#include "StaticResponse.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>

#include <unistd.h>
#include <fcntl.h>

StaticResponse::StaticResponse(Request &request) : Response(request), _is_dir(false)
{
}

StaticResponse::StaticResponse(int status, Request &request) : Response(status, request), _is_dir(false)
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

	_path = _request._uri.path;
	if ((re_status = get_file()) != 0)
		return (re_status);
	defaultHeaders();
	static_headers();
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
				if (directory_tree(it + 1, outfile, fullpath) != 0)
					return (1);
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
	directory_tree(0, &ss, _path);
	ss << "<pre>\n";
	ss << "	</body>\n\
	</html>";

	_body = ss.str();
	// int fd = open("Index.html", O_CREAT| O_WRONLY | O_TRUNC, 0644);
	// write(fd, _body.c_str(), get_content_len());
	// close(fd);
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
			_is_dir = true;
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

std::string StaticResponse::get_content_type()
{
	const unsigned char png_magic[] = {
		0x89, 0x50, 0x4E, 0x47,
		0x0D, 0x0A, 0x1A, 0x0A
	};

	if (_body.size() >= 15 && _body.compare(0, 15, "<!doctype html>") == 0)
	{
		return("text/html");
	}
	else if (_body.size() >= sizeof(png_magic) &&
        std::memcmp(_body.data(), png_magic, sizeof(png_magic)) == 0)
    {
        return("image/png");
    }
	else
	{
		return("application/octet-stream");
	}
}

void StaticResponse::static_headers()
{
	_headers["Content-Type"] = get_content_type();
}