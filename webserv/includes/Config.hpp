/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/19 10:39:46 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>

#include "Parser.hpp"
#include "configDefs.hpp"
#include "utilityFunctions.hpp"
#include "CLocation.hpp"
#include "CServer.hpp"

class CServer;
class CLocation;

class Config {
	private:

	std::vector<std::string>	_charsets;				// charset(s) that can be handled
	std::vector<errorPage_s> 	_error_pages;			// Map of error pages to generate
	long long					_max_body;				// Max acceptable body size in bytes
	int							_max_connections; 		// Max connections to all servers
	long long					_max_header_size;		// Max acceptable header size in bytes
	std::vector<CServer>		_servers;				// Vector of servers
	int							_timeout_ka;			// Length of time before keep-alive timeout (seconds)
	int							_timeout_rw;			// Length of time before rw timeout (seconds)

	public:

								// Constructors/Destructor/Copy
								Config();
								Config(const std::string& filename);
								Config(const Config &other);
								Config operator=(const Config &other);
								~Config();

	std::ifstream				openFile(void);

								// Getters
	std::vector<std::string>	getCharsets(void) const;
	std::string 				getErrorPage(int error_code) const;							// return path to error_page corresponding to error_code
	long long					getMaxBodySize(void) const;									// return max body size in bytes
	int							getMaxConnections(void) const;								// return max server connections
	long long					getMaxHeaderSize(void) const;								// return max body size in bytes
	int							getTimeoutKA(void) const;									// return length of time before keep-alive timeout (seconds)
	int							getTimeoutRW(void) const;									// return length of time before read-write timeout (seconds)

	std::vector<errorPage_s>	getErrorPages(void) const;									// return all error pages
	std::vector<CServer>		getServers(void) const;										// return vector of CServer class objects

								// SETTERS
	void						setCharsets(std::vector<std::string>& charsets);
	void 						setMaxBodySize(long long max_body_size);				// set max body size in bytes - if higher than SYS_MAX_BODY, it will be reduced to SYS_MAX_BODY
	void 						setMaxConnections(int max_connections);						// set max server connections - if higher than SYS_MAX_CONN, it will be reduced to SYS_MAX_CONN
	void 						setMaxHeaderSize(long long max_header_size);				// set max header size in bytes - if higher than SYS_MAX_HEAD, it will be reduced to SYS_MAX_HEAD
	void						setTimeoutKA(int timeout_in_seconds);						// set length of time before keep-alive timeout (seconds) - if higher than SYS_TM_KA, it will be reduced to SYS_TM_KA
	void						setTimeoutRW(int timeout_in_seconds);						// set length of time before read-write timeout (seconds) - if higher than SYS_TM_RW, it will be reduced to SYS_TM_RW

	void						setServers(std::vector<CServer>& servers);					// set servers
	void						setErrorPages(std::vector<errorPage_s>& error_pages);			// set ErrorPages

	// Check server name isn't duplicated? Check at higher level (in Config)
	// Check port is in range 1000 - 65xxx
	// Check error_pages are valid errors and are accessible (or do we just generate based on config values)
};

std::ostream&	operator<<(std::ostream& os, const Config& config);
std::ostream&	operator<<(std::ostream&, const std::vector<errorPage_s>& error_pages);
std::ostream&	operator<<(std::ostream& os, const CLocation& location);
std::ostream&	operator<<(std::ostream& os, const std::vector<CLocation>& locations);
std::ostream&	operator<<(std::ostream& os, const listen_s& port_info);
std::ostream&	operator<<(std::ostream& os, const std::vector<CServer>& servers);
std::ostream&	operator<<(std::ostream& os, const CServer& server);

template<typename T>
std::ostream&	operator<<(std::ostream& os, const std::vector<T> vec);
