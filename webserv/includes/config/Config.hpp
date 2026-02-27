/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/24 10:29:11 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>

#include "PrintUtils.hpp"
#include "Parser.hpp"
#include "configDefs.hpp"
#include "utilityFunctions.hpp"
#include "CLocation.hpp"
#include "CServer.hpp"

class CServer;
class CLocation;

class Config {
	
	private:

		int							_server_socket_max;
		int							_client_socket_max;

		bool						_autoindex;				// indicates global autoindex setting (true = on, false = off)
		std::vector<std::string>	_charset;				// charset(s) that can be handled
		std::map<int, std::string> 	_error_pages;			// Map of error pages to generate
 		std::vector<std::string>	_index;					// ordered list of possible index filenames - first one that works is returned
		long long					_max_body;				// Max acceptable body size in bytes
		int							_max_connections; 		// Max connections to all servers
		long long					_max_header_size;		// Max acceptable header size in bytes
 		std::vector<std::string>	_method;				// list of method
		std::string	 				_root_dir;				// Root directory at global level
		std::vector<CServer>		_servers;				// Vector of servers
		int							_timeout_ka;			// Length of time before keep-alive timeout (seconds)
		int							_timeout_rw;			// Length of time before rw timeout (seconds)

		bool						_autoindex_set;
		bool						_charset_set;
		bool						_error_page_set;
		bool						_index_set;
		bool						_max_body_set;
		bool						_max_connections_set;
		bool						_max_header_set;
		bool						_method_set;
		bool						_root_set;
		bool						_servers_set;
		bool						_timeout_ka_set;
		bool						_timeout_rw_set;
		
	public:
									// Constructors/Destructor/Copy
									Config();
									Config(const std::string& filename);
									Config(const Config &other);
									Config operator=(const Config &other);
									~Config();

									/* CHECK IF VARIABLE SET*/
		bool						isAutoIndexSet(void) const;
		bool						isCharsetSet(void) const;
		bool						isErrorPageSet(void) const;
		bool						isIndexSet(void) const;
		bool						isMaxBodySet(void) const;
		bool						isMaxConnectionsSet(void) const;
		bool						isMaxHeaderSet(void) const;
		bool						isMethodSet(void) const;
		bool						isRootDirSet(void) const;
		bool						isServerSet(void) const;
		bool						isTimeoutKASet(void) const;
		bool						isTimeoutRWSet(void) const;

									/*	GETTERS	*/
		bool						getAutoIndex(void) const;
		std::vector<std::string>	getCharset(void) const;
		std::map<int, std::string>	getErrorPages(void) const;
		std::vector<std::string>	getIndex(void) const;
		long long					getMaxBodySize(void) const;
		int							getMaxConnections(void) const;
		long long					getMaxHeaderSize(void) const;
		std::vector<std::string>	getMethod(void) const;
		std::string					getRootDir(void) const;
		std::vector<CServer>		getServers(void) const;
		int							getTimeoutKA(void) const;
		int							getTimeoutRW(void) const;

		int							getClientSocketMax(void) const;
		int							getServerSocketMax(void) const;

									/*	SETTERS	*/
		void 						setAutoIndex(bool autoindex, bool set_from_config_file);
		void						setCharset(std::vector<std::string>& charset, bool set_from_config_file);
		void						setErrorPage(std::map<int, std::string>& error_pages, bool set_from_config_file);
		void						setIndex(const std::vector<std::string>& index, bool set_from_config_file);
		void 						setMaxBodySize(long long max_body_size, bool set_from_config_file);
		void 						setMaxConnections(int max_connections, bool set_from_config_file);
		void 						setMaxHeaderSize(long long max_header_size, bool set_from_config_file);
		void 						setMethod(const std::vector<std::string>& method, bool set_from_config_file);
		void 						setRootDir(std::string& root, bool set_from_config_file);
		void						setServers(std::vector<CServer>& servers, bool set_from_config_file);
		void						setTimeoutKA(int timeout_in_seconds, bool set_from_config_file);
		void						setTimeoutRW(int timeout_in_seconds, bool set_from_config_file);

									/*	PROPAGATE INHERITABLE VARIABLES	*/
		void						propagateVarsToChildren();
		void						propagateServerToLocations(CServer& server);
		
		template <typename T>
		std::map<int, std::string>	mergeErrorPage(T&, std::map<int, std::string> error_pages);
};

std::ostream&	operator<<(std::ostream& os, const Config& config);
std::ostream&	operator<<(std::ostream& os, const std::vector<CServer>& servers);
std::ostream&	operator<<(std::ostream& os, const std::vector<CLocation>& locations);

std::ostream&	operator<<(std::ostream& os, const CServer& server);
std::ostream&	operator<<(std::ostream& os, const CLocation& location);

std::ostream&	operator<<(std::ostream& os, const std::map<std::string, std::string>& cgi_map);
std::ostream&	operator<<(std::ostream& os, const std::map<int, std::string>& error_pages);
std::ostream&	operator<<(std::ostream& os, const listen_s& port_info);
std::ostream&	operator<<(std::ostream& os, const tryFiles_s& try_files);