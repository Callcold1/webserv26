/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 15:18:30 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/19 10:13:40 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser( void ){}

ConfigParser::~ConfigParser( void ){}

ConfigParser::ConfigParser(const ConfigParser &other){}

ConfigParser& ConfigParser::operator=(const ConfigParser &other) {
	(void) other;
    return *this;
}

Config& ConfigParser::parseConfigFile(std::string& filename)
{
	std::ifstream stream(filename.c_str());

	if (!stream.is_open())
		throw std::runtime_error("Could not open configuration file.");

	config_parser_state state = CONFIG_PARSER_GLOBAL;
	Config& config = new Config[];
	std::string line;

	while (true){
		
		std::getline(stream, line);
		trimWS(line);
		
		switch (state){
			case CONFIG_PARSER_GLOBAL 	: {parseGlobalAttributes(state, config, line); continue;}
			case CONFIG_PARSER_SERVER 	: {parseServerBlock(state, config, line); continue;}
			case CONFIG_PARSER_LOCATION : {parseLocationBlock(state, config, line); continue;}
			case CONFIG_PARSER_COMPLETE : break;
			default 					: throw std::exception(); break;
		}
	}
	return config;
}

/*
	Global attributes are : 

	server behaviour
		- timeout (read/write)
		- keepalive_timeout
		- client max body size
		- max connections (e.g. 1024)
		- log format
	
	server resources
		- index (can be overridden in server block and location block)
		- root (can be overridden in server block and location block)
		- error_pages
	
	log_format - format of logs (going too far?)
	log definitions - logs to be kept - Usually global or server level)
	default_type (MIME)
	character sets

*/
void	ConfigParser::parseGlobalAttributes(config_parser_state& state, Config& config, std::string& line){

	while (state == CONFIG_PARSER_GLOBAL) {

		if (line.empty() || line[0] == '#')
			continue;
		if ( line == "server {" || line == "server{" || line == "Server {" || line == "Server{")
			state == CONFIG_PARSER_SERVER;
		
	return config;
}

void	ConfigParser::parseServerBlock(config_parser_state& state, Config& config, std::ifstream& stream){
	ServerBlock* server_block = new ServerBlock;
	std::string line;
	
	while (state == CONFIG_PARSER_SERVER) {
		std::getline(stream, line);
if (line.find("listen") == 0)
			config.setPort(parseValue<int>(line));  // Access via pointer
		else if (line.find("server_name") == 0)
			config.setName(parseValue<std::string>(line));  // Access via pointer
		else if (line.find("root") == 0 && !currentRoute)
			config.setRootDirConfig(parseValue<std::string>(line));  // Access via pointer
		else if (line.find("client_max_body_size") == 0)
			config.setMaxBodySize(parseValue<int>(line));  // Access via pointer
		else if (line.find("index") == 0 && !currentRoute)
			config.setDefaultFile(parseValue<std::string>(line));  // Access via pointer
		else if (line.find("error_page") == 0)
		{
			std::istringstream iss(line);
			std::string key;
			int error_code;
			std::string error_page;
			iss >> key >> error_code >> error_page;
			config.setErrorPage(error_code, error_page);  // Access via pointer
		}
		else if (line.find("allow_methods") == 0)
		{
			std::vector<std::string> methods = parseMethods(line);
			if (currentRoute)
				currentRoute->setAllowedMethods(methods);
			else
				config[idx].setAllowedMethods(methods);  // Access via pointer
		} else if (line.find("location") == 0)
				parseLocation(config[idx], file, line);  // Pass pointer to parseLocation
	}
	if (currentRoute)
		config[idx].addRoute(currentRoute);  // Access via pointer	
		if (line == "}")
			CONFIG_PARSER_GLOBAL;
	}
}

void	ConfigParser::parseLocationBlock(config_parser_state& state, Config& config, std::ifstream& stream){
	location_t* location = new location_t;
	std::string line;
	
	while (state == CONFIG_PARSER_LOCATION) {
		std::getline(stream, line);

		if (line == "}")
			state == CONFIG_PARSER_SERVER;
	}
	std::string line;

}


/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<
>>>>>>	  LOCATION FUNCTIONS 		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<
*/

// Setters
void ConfigParser::setPath(const str_t &path) { this->path = path; }
void ConfigParser::setAllowedMethods(const std::vector<str_t> &allowed_methods) { this->allowed_methods = allowed_methods; }
void ConfigParser::setRootDirRoute(const str_t &root_dir) { this->root_dir = root_dir; }
void ConfigParser::setIndexFile(const str_t &index_file) { this->index_file = index_file; }

void ConfigParser::setAutoindex(const str_t &autoindex)
{
	autoindex_set = true;
	if (autoindex.find("on") == 0)
	{
		this->autoindex = true;
	}
	else if (autoindex.find("off") == 0)
	{
		this->autoindex = false;
	}
	else
		autoindex_set = false;
}
void ConfigParser::setRedirectStatus(int redirect_status) { this->redirect_status = redirect_status; }
void ConfigParser::setRedirectUrl(const str_t &redirect_url) { this->redirect_url = redirect_url; }

// Check if route contains the target path
bool ConfigParser::containsTarget(const str_t &requestStr) const
{
	if (path.compare(requestStr) == 0)
		return true;
	else if (_target.is_file)
		return false; // ONLY RETURN TRUE FOR A FILE WHEN AN EXACT MATCH IS FOUND
	else if (_target.is_dir){
		//NEED TO DECIDE IF WE ALLOW PARTIAL MATCHES OR NOT
		// 1. I've removed last segment of the path
		return true;
	}
	return false;
}

// Check if method is allowed
bool ConfigParser::allowsMethod(const str_t &method) const
{
	for (size_t i = 0; i < allowed_methods.size(); i++)
	{
		if (allowed_methods[i] == method)
			return true;
	}
	return false;
}

// Route validation check
int ConfigParser::checkRoute(Route *route)
{
	if (route->path.empty())
		return 0;
	return 1;
}

// Cleanup
void ConfigParser::cleanRoute(Route *route)
{
	route->allowed_methods.clear();
}

// Print allowed methods
std::ostream &ConfigParser::printPermittedMethods(std::ostream &os)
{
	for (size_t i = 0; i < allowed_methods.size(); i++)
	{
		os << allowed_methods[i];
		if (i < allowed_methods.size() - 1)
			os << ", ";
		else
			os << std::endl;
	}
	return os;
}

/* 
	Function to append endpoint to route->root 
	Saved to _target.path

	The _target_path is to be used when accessing endpoints during requests
*/

void ConfigParser::appendPathToRoot()
{
	str_t r = root_dir;
	str_t p = path;

	if (r[r.size() - 1] == '/')
		trimEndChar(r, '/');
	if (p[0] == '/')
		trimLeadingChar(p, '/');
	_target.path = r + '/' + p;
}

/* Route Validation */

/* 
	This is partial validation, to be performed on each Route.

	The overall purpose is to have upfront check to :
		: build the path for the endpoint (path)
		: check that the path is accessible
		: save path permissions
		: check if path is a file, or a directory

	If we modify or create files, maybe we have to refresh this for each Route
	
	Otherwise, it is done at outset, and doesn't need to be checked again for every
	request.

	If a Route is used with a DELETE or POST request, then we should refresh the Route
	Should we maintain a map of Routes for all servers to make this easier to manage?

*/

/*
	This struct has been added to the Route class
	An instance should be used as a private variable in the Route class
	
	struct targetInfo_s
	{
		bool inaccessible;		// set to true if path can"t be accessed
		bool is_dir;			// set to true if path is a dir
		bool is_file;			// set to true if path is a file
		bool not_found;			// set to true if file not found
		__mode_t permissions;	// the file permissions
		std::string path;		// the path (root + '/' endpoint )

		targetInfo_s() : inaccessible(false), is_dir(false), is_file(false), not_found(false), permissions(0), path("") {}
	};
*/
void ConfigParser::getTargetPathPermissions(){
	struct stat fileStat;
	
	if (stat(_target.path.c_str(), &fileStat) == 0)
	{
		_target.is_dir = S_ISDIR(fileStat.st_mode);
		_target.is_file = S_ISREG(fileStat.st_mode);
		_target.permissions = fileStat.st_mode;
	}
	else if(errno == ENOENT)
		_target.not_found = true;
	else if(errno == EACCES)
		_target.inaccessible = true;
}

void ConfigParser::setRouteTarget()
{
	appendPathToRoot();
	getTargetPathPermissions();
	/* 
		Need to decide if we throw error if path inaccessible or not found
		 or just log it?
	*/
	return;
}

template<typename T>
T ConfigParser::parseValue(const std::string &line)
{
	std::istringstream iss(line);
	std::string key;
	T value;
	iss >> key >> value;
	return value;
}