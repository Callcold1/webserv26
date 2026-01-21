/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLocation.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:55:25 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 13:15:27 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"

class ConfigLocation {

	bool								_autoindex;			// Indicates if the location has autoindex ()
	std::string							_index;
	std::vector<std::string>			_location_methods;
	std::string 						_name;
	std::string							_path;
	bool								_redirect_status;
	std::string							_redirect_url;
	std::string							_root_dir;			// Root directory for location

	public:
										/* CONSTRUCTORS */
										ConfigLocation( void );
										ConfigLocation( const ConfigLocation& other );

										/* DESTRUCTORS */
										~ConfigLocation( void );
	
										/* COPY ASSIGNMENT */
		ConfigLocation&					operator=(const ConfigLocation& other);

										/* GETTERS */
		const bool						getAutoIndex(void) const;
		const std::string&				getIndex(void) const;
		const std::vector<std::string>&	getLocationMethods(void) const;
		const std::string&				getPath(void) const ;
		const bool						getRedirectStatus(void) const;
		const std::string&				getRedirectURL(void) const;
		const std::string&				getRootDir(void) const;

										/* SETTERS */
		void 							setAutoIndex(bool autoindex);	
		void 							setIndex(const std::string& index);
		void 							setPath(const std::string& path);
		void 							setRedirectStatus(bool redirectStatus);
		void							setRedirectURL(const std::string& redirectURL);
		void 							setRootDir(const std::string& root_dir);
		
										/* CHECKERS */
		bool							match_uri(const std::string& uri);

									// Validation
		// Check path is accessible
		// Check methods are valid
		// Check server name isn't duplicated? Check at higher level (in Config)

};

// PRINT LOCATION INFO FOR SINGLE LOCATION (From output stream)
std::ostream&	operator<<(std::ostream &os, const ConfigLocation& location);
