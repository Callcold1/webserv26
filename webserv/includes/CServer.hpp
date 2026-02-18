/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:55:28 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 16:02:39 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CLocation.hpp"
#include "Config.hpp"

struct errorPage_s;
struct listen_s;

class CLocation;

class CServer {

	std::vector<std::string>	_charsets;		// charset(s) that can be handled
	std::vector<errorPage_s>	_error_pages;		// Vector of error_page structs
	std::vector<CLocation>	 	_locations;			// Vector of location class objects
	long long					_max_body;			// Max acceptable body size in bytes
	long long					_max_header_size;	// Max acceptable header size in bytes
	std::vector<std::string>	_methods;			// Methods permitted for all locations on server (can be superceded by location_methods)
	std::vector<listen_s> 		_ports;				// Connection info (port/ip/default_flag)
	std::string	 				_root_dir;			// Root directory for server - overrides global value
	std::vector<std::string> 	_server_names;		// Server name

	public:
											/*	CONSTRUCTORS	*/
											CServer( void );
											CServer( const CServer& other );

											/*	DESTRUCTORS	*/
											~CServer( void );
	
											/*	COPY ASSIGNMENT	*/
		CServer&							operator=(const CServer& other);

											/*	GETTERS	*/
		std::vector<std::string>			getCharsets(void) const;
		std::vector<errorPage_s>			getErrorPages(void) const;					// Return all error pages at server level
		std::vector<listen_s>				getListen(void) const;						// Return server listening ports
		std::vector<CLocation>				getLocations(void) const;					// Return vector of locations on server
		long long							getMaxBodySize(void) const;					// return max body size in bytes
		int									getMaxConnections(void) const;				// return max server connections
		long long							getMaxHeaderSize(void) const;				// return max body size in bytes
		const std::vector<std::string>		getMethods(void) const;						// Return vector of methods available on server
		std::vector<std::string>			getNames(void) const;						// Return vector of server names
		std::string		 					getRootDir(void) const;						// Return server root directory
		

													/*	SETTERS	*/
		void								setCharsets(std::vector<std::string>& charsets);
		void 								setErrorPages(const std::vector<errorPage_s>& error_pages);
		void 								setLocations(const std::vector<CLocation>& locations);
		void 								setMaxBodySize(long long max_body_size);			// set max body size in bytes - if higher than SYS_MAX_BODY, it will be reduced to SYS_MAX_BODY
		void 								setMaxConnections(int max_connections);				// set max server connections - if higher than SYS_MAX_CONN, it will be reduced to SYS_MAX_CONN
		void 								setMaxHeaderSize(long long max_header_size);		// set max header size in bytes - if higher than SYS_MAX_HEAD, it will be reduced to SYS_MAX_HEAD
		void 								setMethods(const std::vector<std::string>& methods);
		void 								setNames(const std::vector<std::string>& names);	
		void 								setListen(const std::vector<listen_s>& ports);
		void 								setRootDir(std::string& root);
	
		// 									/*	FINDERS	*/
		// bool								findErrorPage(int error_code, std::string& path);	// Return bool to inidicate if error_code found in map - if true the path to the error_page is returned
		// bool								matchLocation(const std::string& path, size_t& pos);// Check if a CLocation corresponding to the given name/path in Request URI

		// 									/*	CHECKERS	*/
		// bool								checkServerMethod(const std::string& method) const; // Check if server supports given method
};
