/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CLocation.hpp                           	        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:55:25 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 13:15:27 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"

class CLocation {

	std::string								_alias;				// actual path for url
	bool									_autoindex;			// indicates if the location has autoindex ()
	std::map<std::string, std::string>		_cgi_map;			// cgi map (extension/path)
	std::vector<std::string>				_charset;			// charset(s) that can be handled
	std::map<int, std::string>				_error_pages;		// Map of error_page_number and corresponding path
	std::vector<std::string>				_index;				// ordered list of possible index filenames - first one that works is returned
	long long								_max_body;			// Max acceptable body size in bytes
	std::vector<std::string>				_method;			// method permitted for this locations on server (supercedes by global/server method)
	LocationMatchType_e						_modifier;			// need to add functionality - possibly use enum instead (Location = or Location ~ etc)
	redirect_s								_redirect;			// redirect_s url and redirect status
	std::string								_root_dir;			// root directory for location
	tryFiles_s								_try_files;			// files to serve + error code to return if no match found
	std::string								_upload_path;		// path to which files can be uploaded
	std::string								_uri;				// location uri

	bool									_alias_set;
	bool									_autoindex_set;
	bool									_cgi_map_set;
	bool									_charset_set;
	bool									_error_page_set;
	bool									_index_set;
	bool									_max_body_set;
	bool									_method_set;
	bool									_modifier_set;
	bool									_redirect_set;
	bool									_root_dir_set;
	bool									_try_files_set;
	bool									_upload_path_set;
	bool									_uri_set;

	std::string								_no_uri;

	public:
											/* CONSTRUCTORS */
											CLocation( void );
											CLocation( const CLocation& other );

											/* DESTRUCTORS */
											~CLocation( void );

											/* COPY ASSIGNMENT */
		CLocation&							operator=(const CLocation& other);

											/* CHECK IF VARIABLE SET*/
		bool								isAliasSet(void) const;
		bool								isAutoIndexSet(void) const;
		bool								isCGIMapSet(void) const;
		bool								isCharsetSet(void) const;
		bool								isErrorPageSet(void) const;
		bool								isIndexSet(void) const;
		bool								isMaxBodySet(void) const;
		bool								isMethodSet(void) const;
		bool								isModifierSet(void) const;
		bool								isRedirectSet(void) const;
		bool								isRootDirSet(void) const;
		bool								isTryFilesSet(void) const;
		bool								isUploadPathSet(void) const;
		bool								isURISet(void) const;
											/* CHECK IF VARIABLE SET*/

											/* OTHER CHECKS*/
		bool								isMethodSupported(const std::string& method) const;

											/* GETTERS */
		std::string							getAlias(void) const;
		bool								getAutoIndex(void) const;
		bool								getCGIEnabled(void) const;
		std::map<std::string, std::string>	getCGIMap(void) const;
		std::vector<std::string>			getCharset(void) const;
		std::string							getErrorPage(int errNum) const;
		std::map<int, std::string>			getErrorPages(void) const;
		std::vector<std::string>			getIndex(void) const;
		long long							getMaxBodySize(void) const;
		std::vector<std::string>			getMethod(void) const;
		std::string							getModifier(void) const;
		redirect_s							getRedirect(void) const;
		bool								getRedirectStatus(void) const;
		std::string							getRedirectURL(void) const;
		std::string							getRootDir(void) const;
		tryFiles_s							getTryFiles(void) const;
		std::string							getUploadPath(void) const;
		const std::string&					getURI(void) const;

											/* SETTERS */
		void								setAlias(const std::string& alias, bool set_from_config_file);
		void 								setAutoIndex(bool autoindex, bool set_from_config_file);
		void 								setCGIMap(const std::map<std::string, std::string> cgi_map, bool set_from_config_file);
		void								setCharset(const std::vector<std::string>& charset, bool set_from_config_file);
		void								setErrorPage(const std::map<int, std::string>& error_pages, bool set_from_config_file);
		void 								setIndex(const std::vector<std::string>& index, bool set_from_config_file);
		void 								setMaxBodySize(long long max_body_size, bool set_from_config_file);
		void 								setMethod(const std::vector<std::string>& method, bool set_from_config_file);
		void 								setModifier(const LocationMatchType_e modifier, bool set_from_config_file);
		void 								setName(const std::string& name, bool set_from_config_file);
		void								setRedirect(const redirect_s& redirect, bool set_from_config_file);
		void 								setRootDir(const std::string& root, bool set_from_config_file);
		void								setTryFiles(const tryFiles_s& try_files, bool set_from_config_file);
		void								setUploadPath(const std::string& path, bool set_from_config_file);
		void								setURI(const std::string& uri, bool set_from_config_file);
};
