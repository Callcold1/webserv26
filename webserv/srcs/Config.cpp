/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:13 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/19 13:57:25 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(){}

Config::Config(const Config &other) {}

Config Config::operator=(const Config &other) {
    if (this != &other) {
    }
    return *this;
}

Config::~Config() {
}

// GETTERS
int	Config::getNumberOfServers(void) {
	return _servers.size();
}

// std::string Config::getRootDirConfig() const { return root_dir; }
// int Config::getMaxBodySize() const { return max_body_size; }
// std::string Config::getDefaultFile() const { return default_file; }
// const std::map<int, std::string> &Config::getErrorPages() const { return error_pages; }

// Get Location for Target
ConfigLocation *Config::getLocationForTarget(const std::string &target) {
    for (size_t i = 0; i < location.size(); i++) {
        if (location[i]->containsTarget(target))
            return location[i];
    }
    return NULL;
}

// Utility Functions
int Config::initialisedCheck() {
    if (_global_port == 0 || root_dir.empty()) return 0;
    return 1;
}

void Config::printConfig() const {
    std::cout << "Server Name: " << server_name << "\n"
              << "Port: " << server_port << "\n"
              << "Root Directory: " << root_dir << "\n"
              << "Max Body Size: " << max_body_size << "\n"
              << "Default File: " << default_file << "\n\n";

    std::cout << "Error Pages:\n";
    for (std::map<int, std::string>::const_iterator err = error_pages.begin(); err != error_pages.end(); err++)
        std::cout << "  " << err->first << ": " << err->second << "\n";

    std::cout << "\nAllowed Methods: ";
    for (size_t i = 0; i < allowed_methods.size(); i++)
        std::cout << allowed_methods[i] << (i + 1 < allowed_methods.size() ? ", " : "\n");

    std::cout << "\nLocations:\n";
    
    if (location.empty()) {
        std::cout << "  No location found!\n";
    }

    for (size_t i = 0; i < location.size(); i++) {
        std::cout << "  Location " << i + 1 << ":\n";
        if (location[i]) {
            location[i]->printLocation();
        } else {
            std::cout << "  Location is NULL\n";
        }
    }
}

void Config::freeConfig() {
    for (size_t i = 0; i < location.size(); i++) {
        delete location[i];
    }
    location.clear();
}

std::ostream&	Config::outputPermittedMethods(std::ostream& os){
	std::cout << "\nAllowed Methods: ";
	for (size_t i = 0; i < allowed_methods.size(); i++)
		os << allowed_methods[i] << (i + 1 < allowed_methods.size() ? ", " : "\n");
	return os;
}

std::ostream& Config::outputLocations(std::ostream& os){
	for (size_t i = 0; i < location.size(); i++) {
		os << std::endl;
        os << "Location " << i + 1 << "\n======\n";
        if (location[i]) {
            location[i]->printLocation(os);
        } else {
            os << "  Location is NULL\n";
        }
    }
	os << std::endl;
	return os;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		GETTERS 		<<<<<<
>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
*/

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		SETTERS 		<<<<<<
>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
*/

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		PRINT CONFIG INFO 		<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

// PRINT ALL CONFIG INFO
std::ostream& operator<<(std::ostream& os, Config* config){
	os << "Path: " << location.getPath() << "\n"
	   << "Root Dir: " << location.getRootDir() << "\n"
	   << "Index File: " << location.getIndex() << "\n"
	   << "Autoindex: " << location.getAutoIndex() << "\n"
	   << "Redirect Status: " << location.getRedirectStatus() << "\n"
	   << "Redirect URL: " << location.getRedirectURL() << "\n";
	printLocation(os, location);
	//printPermittedMethods(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, Config* config){
	
	config->outputPermittedMethods(os);
	int server_count = config->getNumberOfServers();
	
	for (int i = 0; i < server_count; i++){
		os << config->_servers[i];
	}
	return os;
}

//for (int j = 0; j < config->_servers->locations[i].size(), j++) {
//	config->printLocation(os, config->_servers->locations[j]);
//}
