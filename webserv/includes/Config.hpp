/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/14 15:56:38 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <map>

// DEFAULT CONFIGURATION FILE
#define DEFAULT_CONF 	"../default.conf"

// LOCATION STRUCT
typedef struct location_s {

	bool								autoindex;				// Indicates if the location has autoindex ()
	std::string							index_page;
	std::vector<std::string>			location_methods;
	std::string 						name;
	std::string							path;
	int									redirect_status;
	std::string							redirect_url;		
	std::string							location_root_dir;		// Root directory for location

} location_t;

// SERVER STRUCT
typedef struct server_s {

	std::map<int, std::string>			error_pages;
	std::vector<location_t> 			locations;				// Vector of location structs
	std::string 						name;					// Server name
	int 								port;					// Port that server will listen on
	std::string 						root_dir;				// Root directory for server
	std::vector<std::string>			server_methods;			// Methods permitted for all locations on server (can be superceded by location_methods)

} server_t;

class Config {
	private:

	std::vector<server_t>				_servers;				// Vector of servers
	long long							_max_body;				// Max acceptable body size in bytes
	std::string 						_default_config; 		// Default config file	-> defined above in DEFAULT_CONF
	std::map<int, std::string> 			_error_pages;			// Default server error pages

	public:

										// Constructors/Destructor/Copy
										Config();
										Config(const Config &other);
										Config operator=(const Config &another);
										~Config();

										// Setters
	void								setPort(int port);
	void								setName(std::string name);
	void								setRootDirConfig(std::string root_dir);
	void								setMaxBodySize(int max_body_size);
	void								setDefaultFile(std::string default_file);
	void								setErrorPage(int error_status, std::string error_page);
	void								setMethods(const std::vector<std::string>& server_methods);
	void								addLocation(location_t* location);
										
										// Getters
	int									getPort() const;
	std::string 						getName() const;
	std::string 						getRootDirConfig() const;
	int 								getMaxBodySize() const;
	std::string 						getDefaultFile() const;
	const std::map<int, std::string>& 	getErrorPages() const;
	const std::vector<std::string>& 	getAllowedMethods() const;
	const std::vector<location_t>& 		getLocations();
   	 location_t*						getLocationForTarget(const std::string &target);
										
	 									// Printing
	std::ostream&						outputPermittedMethods(std::ostream& os);
	std::ostream&						outputRoutes(std::ostream& os);
										
										// Utils
	int 								initialisedCheck();
	void 								printConfig() const;
	void 								freeConfig();
};

std::ostream& operator<<(std::ostream& os, Config* config);