#include "SuccessResponse.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>

#include <unistd.h>
#include <fcntl.h>

SuccessResponse::SuccessResponse(Request &request) : Response(request), _is_dir(false)
{
}

SuccessResponse::SuccessResponse(int status, Request &request) : Response(status, request), _is_dir(false)
{
}

SuccessResponse::SuccessResponse(const SuccessResponse &other) : Response(other)
{}

SuccessResponse &SuccessResponse::operator=(const SuccessResponse &other)
{
	Response::operator=(other);
	return(*this);
}

SuccessResponse::~SuccessResponse()
{}

int SuccessResponse::generate()
{
	int re_status = 0;

	//_path = _request.get_root_dir() + _request._uri.path;
	_path = _request._re.paths.resource_path.path;
	if ((re_status = if_delete()) != 1)
		return (re_status);
	if ((re_status = get_file()) != 0)
		return (re_status);
	defaultHeaders();
	if (!_body.empty())
		static_headers();
	return (0);
}

int SuccessResponse::if_delete()
{
	if ((_path.find(HTML_PATH) != std::string::npos) && (_request._method == "DELETE"))
	 	return(405); //method not allowed
	else if ((_path.find(PHOTO_PATH) != std::string::npos) && (_request._method == "DELETE"))
	{
		if (access(_path.c_str(), F_OK) == -1)
		{
        	return(404);
		}
		else
		{
			if (std::remove(_path.c_str()) == 0)
			{
				_status = 204; // 204 = No Content (Standard for successful delete)
				defaultHeaders();
				return (0);
			}
			else
			{
				return(403); //permission denied
			}
		}
	}
	return (1);
}

int directory_tree(int it, std::stringstream *outfile, std::string rel_path, std::string abs_path, bool is_gallery)
{
	std::string rel_fullpath;
	std::string abs_fullpath;
	DIR *dir = opendir(abs_path.c_str());
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
		rel_fullpath = rel_path + "/" + entry->d_name;
		abs_fullpath = abs_path + "/" + entry->d_name;
		for (int i = 0; i < it; i++)
			*outfile << "	";
		*outfile << "|--->" << "<a href=\"" << rel_fullpath << "\">" << entry->d_name << "</a>";
		if (is_gallery)
		{
			*outfile << "&nbsp;&nbsp;&nbsp;" << " <button class=\"delete-btn\" onclick=\"if(confirm('Delete " << entry->d_name << "?')) "
             << "fetch('" << rel_fullpath << "', {method: 'DELETE'}).then(()=>location.reload())\">"
             << "delete</button>";
		}
		*outfile << "<br>\n";
		if (stat(abs_fullpath.c_str(), &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
			{
				if (directory_tree(it + 1, outfile, rel_fullpath, abs_fullpath, is_gallery) != 0)
					return (1);
			}
		}
	}
	closedir(dir);
	return(0);
}

int SuccessResponse::gen_index_page(bool is_gallery)
{
	std::stringstream ss;

	std::string title = "Index";
	if (is_gallery)
		title = "Gallery";
    ss << "<!doctype html>\n\
	<html>\n\
		<head>\n\
			<title>" << title << "</title>\n\
			<style>\n\
                .delete-btn {\n\
                    margin-left: 15px;\n\
                    padding: 2px 8px;\n\
                    background-color: #ff4d4d;\n\
                    color: white;\n\
                    border: none;\n\
                    border-radius: 4px;\n\
                    cursor: pointer;\n\
                    font-size: 12px;\n\
                    transition: background 0.2s;\n\
                }\n\
                .delete-btn:hover {\n\
                    background-color: #cc0000;\n\
                }\n\
                a { text-decoration: none; color: #0066cc; }\n\
                a:hover { text-decoration: underline; }\n\
            </style>\n\
		</head>\n\
		<body>\n\
			<h1>" << title << "</h1>\n";
	ss << "<pre>\n";
	directory_tree(0, &ss, _request._uri.path, _path, is_gallery);
	ss << "<pre>\n";
	ss << "	</body>\n\
	</html>";

	_body = ss.str();
	// int fd = open("Index.html", O_CREAT| O_WRONLY | O_TRUNC, 0644);
	// write(fd, _body.c_str(), get_content_len());
	// close(fd);
	return(0);
}

int SuccessResponse::get_file()
{
	struct stat st;
	std::cout << "get file entered: " << _path << std::endl;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			std::cout << "dir" << std::endl;
			if (_path.find(HTML_PATH) != std::string::npos)
				gen_index_page(false);
			else if (_path.find(PHOTO_PATH) != std::string::npos)
				gen_index_page(true);
			else
				gen_index_page(true);
			_is_dir = true;
		}
		else if (S_ISREG(st.st_mode))
		{
			std::cout << "file found" << std::endl;
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

std::string SuccessResponse::get_content_type()
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

void SuccessResponse::static_headers()
{
	_headers["Content-Type"] = get_content_type();
}