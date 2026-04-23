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
	if ((_path.find(_request._re.paths.upload_base) != std::string::npos) && (_request._method == "DELETE"))
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
	return(0);
}

int SuccessResponse::get_index_file()
{	
	std::vector<std::string> index_pages;
	std::string target;
	std::string root;
	fileInfo_s	info;

	index_pages = _request._matched_location->getIndex();
	root = _path;

	
	std::cout << "vec size: " << index_pages.size() << std::endl;
	for (std::vector<std::string>::iterator it = index_pages.begin() ; it != index_pages.end(); ++it)
	{
		target = *it;
		std::cout << "lookiingn for: " << target << std::endl;
		std::string	index = appendPathToRoot(target, root);
		getFileInfo(index, info);
		if (info.is_file)
		{
			if (info.inaccessible)
			{
				return (403); //permission denied
			}
			_path = index;
			serve_file();
			return (0);
		}
	}
	if (_request._matched_location->getAutoIndex())
	{
		gen_index_page(false);
		return (0);
	}
	return (NOT_FOUND_ERROR);
}

void SuccessResponse::serve_file()
{
	std::cout << "serving: " << _path << std::endl;
	std::ifstream in(_path.c_str(), std::ios::binary);
	std::string s((std::istreambuf_iterator<char>(in)), \
		std::istreambuf_iterator<char>());
	_body = s;
}

int SuccessResponse::get_file()
{
	fileInfo_s	info;
	std::cout << "get file entered: " << _path << std::endl;
	getFileInfo(_path, info);
	
	if (info.is_dir)
	{
		std::cout << "dir " << _request._re.paths.upload_base << std::endl;
		if (_request._matched_location->isUploadPathSet() && _path.find(_request._re.paths.upload_base) != std::string::npos)
			gen_index_page(true);
		else
		{
			int err = get_index_file();
			if ( err > 0)
				return (err);
		}
		_is_dir = true;
	}
	else if (info.is_file)
	{
		std::cout << "file found" << std::endl;
		serve_file();
	}
	else if (info.inaccessible)
	{
		return (403); //permission denied
	}
	else 
	{
		return (NOT_FOUND_ERROR);
	}
	return (0);
}

void SuccessResponse::static_headers()
{
	_headers["Content-Type"] = get_content_type();
}