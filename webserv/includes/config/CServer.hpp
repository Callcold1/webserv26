/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CServer.hpp                                	   :+:      :+:    :+:    */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:55:28 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 16:02:39 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"

class CLocation;

class CServer {

	private:

		bool								_autoindex;			// indicates server autoindex setting (true = on, false = off)
		std::vector<std::string>			_charset;			// charset(s) that can be handled
		std::map<int, std::string>			_error_pages;		// Map of error_page_number and corresponding path
 		std::vector<std::string>			_index;				// ordered list of possible index filenames - first one that works is returned
		 std::vector<listen_s> 				_listen;			// Connection info (port/ip/default_flag)
		std::vector<CLocation>	 			_locations;			// Vector of location class objects
		long long							_max_body;			// Max acceptable body size in bytes
		long long							_max_header_size;	// Max acceptable header size in bytes
		std::vector<std::string>			_method;			// Method permitted for all locations on server (can be superceded by location_method)
		std::map<std::string, std::string> 	_mime;				// Mime permitted for all locations on server 
		std::string	 						_root_dir;			// Root directory for server - overrides global value
		std::vector<std::string> 			_server_name;		// Server names
		int									_timeout_ka;		// Length of time before keep-alive timeout (seconds)
		int									_timeout_rw;		// Length of time before rw timeout (seconds)

		bool								_autoindex_set;
		bool								_charset_set;
		bool								_error_page_set;
		bool								_index_set;
		bool								_listen_set;
		bool								_location_set;
		bool								_max_body_set;
		bool								_max_header_set;
		bool								_method_set;
		bool								_mime_set;
		bool								_root_set;
		bool								_server_name_set;
		bool								_timeout_ka_set;
		bool								_timeout_rw_set;

	public:
											/*	CONSTRUCTORS	*/
											CServer( void );
											CServer( const CServer& other );

											/*	DESTRUCTORS	*/
											~CServer( void );

											/*	COPY ASSIGNMENT	*/
		CServer&							operator=(const CServer& other);

											/* CHECK IF VARIABLE SET*/
		bool								isAutoIndexSet(void) const;
		bool								isCharsetSet(void) const;
		bool								isErrorPageSet(void) const;
		bool								isIndexSet(void) const;
		bool								isListenSet(void) const;
		bool								isMaxBodySet(void) const;
		bool								isMaxConnectionsSet(void) const;
		bool								isMaxHeaderSet(void) const;
		bool								isMethodSet(void) const;
		bool								isRootDirSet(void) const;
		bool								isServerNameSet(void) const;
		bool								isTimeoutKASet(void) const;
		bool								isTimeoutRWSet(void) const;

											/*	GETTERS	*/
		bool								getAutoIndex(void) const;
		std::vector<std::string>			getCharset(void) const;
		std::string							getErrorPage(int errNum) const;
		std::map<int, std::string>			getErrorPages(void) const;
		std::vector<std::string>			getIndex(void) const;
		std::vector<listen_s>				getListen(void) const;
		std::vector<CLocation>				getLocations(void) const;
		long long							getMaxBodySize(void) const;
		long long							getMaxHeaderSize(void) const;
		const std::vector<std::string>		getMethod(void) const;
		std::map<std::string, std::string>	getMime(void) const;
		std::string		 					getRootDir(void) const;
		std::vector<std::string>			getServerName(void) const;
		int									getTimeoutKA (void) const;
		int									getTimeoutRW(void) const;

											/*	SETTERS	*/
		void 								setAutoIndex(bool autoindex, bool set_from_config_file);
		void								setCharset(std::vector<std::string>& charset, bool set_from_config_file);
		void 								setErrorPage(const std::map<int, std::string>& error_pages, bool set_from_config_file);
		void								setIndex(const std::vector<std::string>& index, bool set_from_config_file);
		void 								setListen(const std::vector<listen_s>& ports, bool set_from_config_file);
		void 								setLocations(const std::vector<CLocation>& locations, bool set_from_config_file);
		void 								setMaxBodySize(long long max_body_size, bool set_from_config_file);
		void 								setMaxHeaderSize(long long max_header_size, bool set_from_config_file);
		void 								setMethod(const std::vector<std::string>& method, bool set_from_config_file);
		void								setMime(const std::map<std::string, std::string>& mime, bool set_from_config_file);
		void 								setServerName(const std::vector<std::string>& names, bool set_from_config_file);
		void 								setRootDir(std::string& root, bool set_from_config_file);
		void								setTimeoutKA(int timeout_in_seconds, bool set_from_config_file);
		void								setTimeoutRW(int timeout_in_seconds, bool set_from_config_file);

											/*	PROPAGATE VARIABLES TO LOWER LEVEL (CSERVER -> CLOCATION)	*/
		void								propagateServerToLocation();

											/*	LOCATION MATCHING (FOR REQUEST URI FIND BEST FIT CLOCATION )	*/
		const CLocation*					getLocationForPath(const std::string& uri_path, std::string& leftover);
};
