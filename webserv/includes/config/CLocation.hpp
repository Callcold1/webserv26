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
	std::vector<errorPage_s>				_error_pages;		// Vector of error_page structs
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
	bool									_error_pages_set;
	bool									_index_set;
	bool									_max_body_set;
	bool									_method_set;
	bool									_modifier_set;
	bool									_redirect_set;
	bool									_root_dir_set;
	bool									_try_files_set;
	bool									_upload_path_set;
	bool									_uri_set;

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
		bool								ischarset(void) const;
		bool								isErrorPagesSet(void) const;
		bool								isIndexSet(void) const;
		bool								isMaxBodySet(void) const;
		bool								isMethodSet(void) const;
		bool								isModifierSet(void) const;
		bool								isRedirectSet(void) const;
		bool								isRootDirSet(void) const;
		bool								isTryFilesSet(void) const;
		bool								isUploadPathSet(void) const;
		bool								isURISet(void) const;

											/* GETTERS */
		std::string							getAlias(void) const;
		bool								getAutoIndex(void) const;
		bool								getCGIEnabled(void) const;
		std::map<std::string, std::string>	getCGIMap(void) const;
		std::vector<std::string>			getcharset(void) const;
		std::vector<errorPage_s>			getErrorPages(void) const;
		std::vector<std::string>			getIndex(void) const;
		long long							getMaxBodySize(void) const;
		std::vector<std::string>			getMethod(void) const;
		char								getModifier(void) const;
		redirect_s							getRedirect(void) const;
		bool								getRedirectStatus(void) const;
		std::string							getRedirectURL(void) const;
		std::string							getRootDir(void) const;
		tryFiles_s							getTryFiles(void) const;
		std::string							getUploadPath(void) const;
		std::string							getURI(void) const;

											/* SETTERS */
		void								setAlias(const std::string& alias);
		void 								setAutoIndex(bool autoindex);
		void 								setCGIMap(const std::map<std::string, std::string> cgi_map);
		void								setcharset(const std::vector<std::string>& charset);
		void								setErrorPages(const std::vector<errorPage_s>& error_pages);
		void 								setIndex(const std::vector<std::string>& index);
		void 								setMaxBodySize(long long max_body_size);
		void 								setMethod(const std::vector<std::string>& method);
		void 								setModifier(const LocationMatchType_e modifier);
		void 								setName(const std::string& name);
		void								setRedirect(const redirect_s& redirect);
		void 								setRootDir(const std::string& root);
		void								setTryFiles(const tryFiles_s& try_files);
		void								setUploadPath(const std::string& path);
		void								setURI(const std::string& uri);
};
