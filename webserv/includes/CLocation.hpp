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
	std::vector<std::string>				_charsets;			// charset(s) that can be handled
 	std::vector<std::string>				_index;				// ordered list of possible indexes - first one that works is returned
	std::vector<std::string>				_methods;			// methods permitted for this locations on server (supercedes by global/server methods)
	LocationMatchType_e						_modifier;			// need to add functionality - possibly use enum instead (Location = or Location ~ etc)
	std::string 							_name;				// location name
	redirect_s								_redirect;			// redirect_s url and redirect status
	std::string								_root_dir;			// root directory for location
	std::vector<tryFiles_s>					_try_files;			// files to serve + error code to return if no match found
	std::string								_upload_path;		// path to which files can be uploaded
	std::string								_uri;				// location uri

	public:
											/* CONSTRUCTORS */
											CLocation( void );
											CLocation( const CLocation& other );

											/* DESTRUCTORS */
											~CLocation( void );

											/* COPY ASSIGNMENT */
		CLocation&							operator=(const CLocation& other);

											/* GETTERS */
		std::string							getAlias(void) const;
		bool								getAutoIndex(void) const;
		bool								getCGIEnabled(void) const;
		std::map<std::string, std::string>	getCGIMap(void) const;
		std::vector<std::string>			getCharsets(void) const;
		std::vector<std::string>			getIndex(void) const;
		std::vector<std::string>			getMethods(void) const;
		LocationMatchType_e					getModifier(void) const;
		std::string							getName(void) const;
		redirect_s							getRedirect(void) const;
		bool								getRedirectStatus(void) const;
		std::string							getRedirectURL(void) const;
		std::string							getRootDir(void) const;
		std::vector<tryFiles_s>				getTryFiles(void) const;
		std::string							getUploadPath(void) const;
		std::string							getURI(void) const;

											/* SETTERS */
		void								setAlias(const std::string& alias);
		void 								setAutoIndex(bool autoindex);
		void 								setCGIMap(const std::map<std::string, std::string> cgi_map);
		void								setCharsets(const std::vector<std::string>& charsets);
		void 								setIndex(const std::vector<std::string>& index);
		void 								setMethods(const std::vector<std::string>& methods);
		void 								setModifier(const LocationMatchType_e modifier);
		void 								setName(const std::string& name);
		void 								setRedirectStatus(bool redirectStatus);
		void								setRedirect(const redirect_s& redirect);
		void 								setRootDir(const std::string& root);
		void								setTryFiles(const std::vector<tryFiles_s>& try_files);
		void								setUploadPath(const std::string& path);
		void								setURI(const std::string& uri);
		
											/* CHECKERS */
		bool								match_uri(const std::string& uri);

};

// PRINT LOCATION INFO FOR SINGLE LOCATION (From output stream)
//std::ostream&	operator<<(std::ostream &os, const CLocation& location);
