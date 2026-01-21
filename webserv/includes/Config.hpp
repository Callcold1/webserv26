/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 09:37:52 by kmooney          ###   ########.fr       */
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

	std::string 						_config_file; 			// config file name	(if not provided, revert to DEFAULT_CONF)
	std::map<int, std::string> 			_error_pages;			// Map of error pages to generate
	std::vector<std::string>			_log_names;				// Vector of log names to be generated
	long long							_max_body;				// Max acceptable body size in bytes
	int									_max_connections; 		// (e.g. 1024)
	std::vector<ConfigServer>			_servers;				// Vector of servers
	struct tm							_timeout_ka;			// Length of time before keep-alive timeout
	struct tm							_timeout_rw;			// Length of time before rw timeout

	public:

										// Constructors/Destructor/Copy
										Config();
										Config(const Config &other);
										Config operator=(const Config &other);
										~Config();
										
										// Getters
   	// ConfigLocation*					getLocationForTarget(const std::string &target);
	// GETTERS
	const std::string&					getConfigFile(void) const;			// return name of config file
	std::string 						getErrorPage(int error_code);		// return path to error_page corresponding to error_code	
	const long long						getMaxBodySize(void) const;			// Max body size in bytes
	int									getNumberOfServers(void) const;		// Max body size in bytes
	std::string 						getRootDir() const;					// Max body size in bytes

	// SETTERS

	void 								setConfigFile(const std::string& config_file);
	void 								setErrorPage(int error_status, std::string error_page);
	void 								setMaxBodySize(long long max_body_size);
		

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