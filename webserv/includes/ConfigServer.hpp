/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:55:28 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/19 14:21:30 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ConfigLocation.hpp"

class ConfigServer {

		std::map<int, std::string>			_error_pages;
		std::vector<ConfigLocation> 		_locations;			// Vector of location structs
		std::vector<std::string>			_methods;			// Methods permitted for all locations on server (can be superceded by location_methods)
		std::string 						_name;				// Server name
		int 								_port;				// Port that server will listen on
		std::string 						_root_dir;			// Root directory for server - overrides global value

	public:
											/* CONSTRUCTORS */
											ConfigServer( void );
											ConfigServer( const ConfigServer& other );

											/* DESTRUCTORS */
											~ConfigServer( void );
	
											/* COPY ASSIGNMENT */
		ConfigServer&						operator=(const ConfigServer& other);

											//Getters
		const std::map<int, std::string>&	getErrorPages(void);		// Do we have these saved to specific paths or generate them? I would generate
		const std::vector<std::string>&		getMethods(void);
		const std::string&					getName(void);
		int									getPort(void);
		const std::string& 					getRootDir(void);
		const std::vector<ConfigLocation>&	getServerLocations(void);
											
		// QUERIES USED BY REQUEST CLASS 
		const ConfigLocation&				getLocation(const std::string& name); // Identify ConfigLocation corresponding to the given name/path in Request URI

											//Setters
		void 								setErrorPage(int error_status, std::string error_page);
		void 								setLocation(ConfigLocation& location);
		void 								setMethods(const std::vector<std::string> &allowed_methods);
		void 								setName(std::string name);
		void 								setPort(int port);
		void 								setRootDirConfig(std::string root_dir);

											//Validation

};
								//Printing
std::ostream&	operator<<(std::ostream &os, const ConfigServer& server);
std::ostream&	operator<<(std::ostream &os, const std::map<int, std::string>& error_pages);