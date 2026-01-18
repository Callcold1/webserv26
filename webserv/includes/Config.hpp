/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/18 17:24:10 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <map>
#include <iostream>

// DEFAULT CONFIGURATION FILE
#define DEFAULT_CONF 	"../default.conf"

// LOCATION STRUCT
typedef struct location_s {

	bool								_autoindex;				// Indicates if the location has autoindex ()
	std::string							_index_file;
	std::vector<std::string>			_location_methods;
	std::string 						_name;
	std::string							_path;
	int									_redirect_status;
	std::string							_redirect_url;		
	std::string							_root_dir;		// Root directory for location

} location_t;

// SERVER STRUCT
typedef struct server_s {

	std::map<int, std::string>			_error_pages;
	std::vector<location_t> 			_locations;				// Vector of location structs
	std::string 						_name;					// Server name
	int 								_port;					// Port that server will listen on
	std::string 						_root_dir;				// Root directory for server
	std::vector<std::string>			_server_methods;			// Methods permitted for all locations on server (can be superceded by location_methods)

} server_t;

class Config {
	private:

	std::string 						_default_config; 		// Default config file	-> defined above in DEFAULT_CONF
	std::map<int, std::string> 			_error_pages;			// Default server error pages
	long long							_max_body;				// Max acceptable body size in bytes
	std::vector<server_t>				_servers;				// Vector of servers

	public:

										// Constructors/Destructor/Copy
										Config();
										Config(const Config &other);
										Config operator=(const Config &another);
										~Config();
										
										// Getters
	int									getPort() const;
	std::string 						getName() const;
	std::string 						getRootDirConfig() const;
	int 								getMaxBodySize() const;
	std::string 						getDefaultFile() const;
	const std::map<int, std::string>& 	getErrorPages() const;
	const std::vector<std::string>& 	getAllowedMethods() const;
	const std::vector<location_t>& 		getLocations();
   	location_t*							getLocationForTarget(const std::string &target);
										
	// Setters

	//Global attributes
	void 								setMaxBodySize(int max_body_size) { this->_max_body = max_body_size; }
	void 								setDefaultFile(std::string default_file) { this->default_file = default_file; }
	void 								setErrorPage(int error_status, std::string error_page) { error_pages[error_status] = error_page; }
	void 								setGlobalMethods(const std::vector<std::string> &allowed_methods) { this->allowed_methods = allowed_methods; } // setAllowedMethods
		
	//Server attributes		
	void 								setPort(int port) { server_port = port; }
	void 								setName(std::string name) { server_name = name; }
	void 								setRootDirConfig(std::string root_dir) { this->root_dir = root_dir; }
	void 								setErrorPage(int error_status, std::string error_page) { error_pages[error_status] = error_page; }
	void 								setServerMethods(const std::vector<std::string> &allowed_methods) { this->allowed_methods = allowed_methods; } //setAllowedMethods
	void 								addLocation(location_t *route) { location.push_back(route); }

	//Location attributes
	 									// Printing
	std::ostream&						outputPermittedMethods(std::ostream& os);
	std::ostream&						outputRoutes(std::ostream& os);
										
										// Utils
	int 								initialisedCheck();
	void 								printConfig() const;
	void 								freeConfig();

	std::ostream&						printLocation(std::ostream &os, location_t& location);
	void								printLocation(location_t& location);
};

std::ostream& operator<<(std::ostream& os, Config* config);