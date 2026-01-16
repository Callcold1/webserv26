/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 15:18:30 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/16 17:49:09 by kmooney          ###   ########.fr       */
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

Config* ConfigParser::parseConfigFile(std::string& filename)
{
	std::ifstream stream(filename.c_str());

	if (!stream.is_open())
		throw std::runtime_error("Could not open configuration file.");
	
	Config* config = new Config[];
	
	std::string line;
	
	while (true) {

		std::getline(stream, line);
		trimWS(line);
		if (line.empty() || line[0] == '#')
			continue;
		if ( line == "server {" || line == "server{" || line == "Server {" || line == "Server{")
			parseServerBlock(config, stream);
		else if (line.find("listen") == 0)
			config[idx].setPort(parseValue<int>(line));  // Access via pointer
		else if (line.find("server_name") == 0)
			config[idx].setName(parseValue<std::string>(line));  // Access via pointer
		else if (line.find("root") == 0 && !currentRoute)
			config[idx].setRootDirConfig(parseValue<std::string>(line));  // Access via pointer
		else if (line.find("client_max_body_size") == 0)
			config[idx].setMaxBodySize(parseValue<int>(line));  // Access via pointer
		else if (line.find("index") == 0 && !currentRoute)
			config[idx].setDefaultFile(parseValue<std::string>(line));  // Access via pointer
		else if (line.find("error_page") == 0)
		{
			std::istringstream iss(line);
			std::string key;
			int error_code;
			std::string error_page;
			iss >> key >> error_code >> error_page;
			config[idx].setErrorPage(error_code, error_page);  // Access via pointer
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
	file.close();
	return config;
}

void	ConfigParser::parseGlobalAttributes(Config* config, std::ifstream& stream){
	server_t* server = new server_t;

	std::string line;
		
}

void	ConfigParser::parseServerBlock(Config* config, std::ifstream& stream){
	server_t* server = new server_t;
	std::string line;
	
	while (true) {
		std::getline(stream, line);

		if (line == "}")
	}
}

void	ConfigParser::parseLocation(Config* config, std::ifstream& file, std::string& oldLine){
	location_t* location = new location_t;

	std::string line;

}