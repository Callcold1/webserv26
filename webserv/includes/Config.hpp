/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/19 13:57:24 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>

#include "ConfigServer.hpp"
#include "ConfigLocation.hpp"

// DEFAULT CONFIGURATION FILE
#define DEFAULT_CONF 	"../default.conf"

class Config {
	private:

	std::string 						_default_config; 		// Default config file	-> defined above in DEFAULT_CONF
	std::map<int, std::string> 			_error_pages;			// Map of error pages to generate
	std::vector<std::string>			_log_names;				// Vector of log names to be generated
	long long							_max_body;				// Max acceptable body size in bytes
	int									_max_connections; 		// (e.g. 1024)
	std::vector<ConfigServer>			_servers;				// Vector of servers
	struct tm							_timeout_rw;			// Length of time before rw timeout
	struct tm							_timeout_ka;			// Length of time before keep-alive timeout

	public:

										// Constructors/Destructor/Copy
										Config();
										Config(const Config &other);
										Config operator=(const Config &other);
										~Config();
										
										// Getters
   	// ConfigLocation*					getLocationForTarget(const std::string &target);
	
	// SETTERS

	void 								setDefaultFile(void) { _default_config = DEFAULT_CONF; }
	void 								setErrorPage(int error_status, std::string error_page) { _error_pages[error_status] = error_page; }
	void 								setMaxBodySize(int max_body_size) { _max_body = max_body_size; }
	
	// GETTERS
	void 								getDefaultFile(void) { _default_config = DEFAULT_CONF; }
	void 								getErrorPage(int error_status, std::string error_page) { _error_pages[error_status] = error_page; }
	long long							getMaxBodySize(int max_body_size) { _max_body = max_body_size; } // Max body size in bytes
	int									getNumberOfServers(void);
	std::string 						getRootDir() const;
	
	// QUERIES USED BY SERVER CLASS 
	const ConfigServer&					getServer(const std::string& name); // Return ConfigServer corresponding to given name

	// Validation
	
	// Check config file is accessible
	// Check server name isn't duplicated? Check at higher level (in Config)
	// Check port is in range 1000 - 65xxx
	// Check root dir is valid and accessible
	// Check error_pages are valid errors and are accessible


	//Location attributes
	 									// Printing
	std::ostream&						outputPermittedMethods(std::ostream& os);
	std::ostream&						outputRoutes(std::ostream& os);
										
										// Utils
	int 								initialisedCheck();
	void 								printConfig() const;
	void 								freeConfig();

	std::ostream&						printLocation(std::ostream &os, ConfigLocation& location);
	void								printLocation(ConfigLocation& location);
};

std::ostream&	operator<<(std::ostream& os, Config* config);