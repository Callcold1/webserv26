/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:13 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/16 17:41:10 by kmooney          ###   ########.fr       */
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

// Setters
void Config::setPort(int port) { server_port = port; }
void Config::setName(std::string name) { server_name = name; }
void Config::setRootDirConfig(std::string root_dir) { this->root_dir = root_dir; }
void Config::setMaxBodySize(int max_body_size) { this->max_body_size = max_body_size; }
void Config::setDefaultFile(std::string default_file) { this->default_file = default_file; }
void Config::setErrorPage(int error_status, std::string error_page) { error_pages[error_status] = error_page; }
void Config::setAllowedMethods(const std::vector<std::string> &allowed_methods) { this->allowed_methods = allowed_methods; }
void Config::addRoute(Route *route) { routes.push_back(route); }

// Getters
int Config::getPort() const { return server_port; }
std::string Config::getName() const { return server_name; }
std::string Config::getRootDirConfig() const { return root_dir; }
int Config::getMaxBodySize() const { return max_body_size; }
std::string Config::getDefaultFile() const { return default_file; }
const std::map<int, std::string> &Config::getErrorPages() const { return error_pages; }

// Get Route for Target
Route *Config::getRouteForTarget(const std::string &target) {
    for (size_t i = 0; i < routes.size(); i++) {
        if (routes[i]->containsTarget(target))
            return routes[i];
    }
    return NULL;
}

// Utility Functions
int Config::initialisedCheck() {
    if (server_port == 0 || root_dir.empty()) return 0;
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

    std::cout << "\nRoutes:\n";
    
    if (routes.empty()) {
        std::cout << "  No routes found!\n";
    }

    for (size_t i = 0; i < routes.size(); i++) {
        std::cout << "  Route " << i + 1 << ":\n";
        if (routes[i]) {
            routes[i]->printRoute();
        } else {
            std::cout << "  Route is NULL\n";
        }
    }
}

void Config::freeConfig() {
    for (size_t i = 0; i < routes.size(); i++) {
        delete routes[i];
    }
    routes.clear();
}

std::ostream&	Config::outputPermittedMethods(std::ostream& os){
	std::cout << "\nAllowed Methods: ";
	for (size_t i = 0; i < allowed_methods.size(); i++)
		os << allowed_methods[i] << (i + 1 < allowed_methods.size() ? ", " : "\n");
	return os;
}

std::ostream& Config::outputRoutes(std::ostream& os){
	for (size_t i = 0; i < routes.size(); i++) {
		os << std::endl;
        os << "Route " << i + 1 << "\n======\n";
        if (routes[i]) {
            routes[i]->printRoute(os);
        } else {
            os << "  Route is NULL\n";
        }
    }
	os << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, Config* config){
	config->outputPermittedMethods(os);
	config->outputRoutes(os);
	return os;
}

