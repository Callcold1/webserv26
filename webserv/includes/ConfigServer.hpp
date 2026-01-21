/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:55:28 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 16:02:39 by kmooney          ###   ########.fr       */
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
											/*	CONSTRUCTORS	*/
											ConfigServer( void );
											ConfigServer( const ConfigServer& other );

											/*	DESTRUCTORS	*/
											~ConfigServer( void );
	
											/*	COPY ASSIGNMENT	*/
		ConfigServer&						operator=(const ConfigServer& other);

											/*	GETTERS	*/
		const std::map<int, std::string>&	getErrorPages(void) const;							// Return all error pages at server level
		const std::vector<std::string>&		getMethods(void) const;								// Return vector of methods available on server
		const std::string&					getName(void) const;								// Return name of server
		int									getPort(void) const;								// Return server listening port
		const std::string& 					getRootDir(void) const;								// Return server root directory
		const std::vector<ConfigLocation>&	getServerLocations(void) const;						// Return vector of locations on server
		const ConfigLocation&				getServerLocation(const std::string& path);			// Return single location if accessible on server

											/*	FINDERS	*/
		bool								findErrorPage(int error_code, std::string& path);	// Return bool to inidicate if error_code found in map - if true the path to the error_page is returned
		ConfigLocation*						findLocation(const std::string& uri);				// Identify ConfigLocation corresponding to the given name/path in Request URI

											/*	CHECKERS	*/
		bool								checkServerMethod(const std::string& method) const; // Check if server supports given method
		
											/*	SETTERS	*/
		void 								setErrorPage(int error_status, std::string error_page);
		void 								setLocation(ConfigLocation& location);
		void 								setMethods(const std::vector<std::string> &allowed_methods);
		void 								setName(std::string name);
		void 								setPort(int port);
		void 								setRootDirConfig(std::string root_dir);

											/*	VALIDATION	*/

};
								//Printing
std::ostream&	operator<<(std::ostream &os, const ConfigServer& server);
std::ostream&	operator<<(std::ostream &os, const std::map<int, std::string>& error_pages);