/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLocation.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:55:25 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/19 16:49:53 by kmooney          ###   ########.fr       */
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
	std::string							_root_dir;				// Root directory for location

	public:
										/* CONSTRUCTORS */
										ConfigLocation( void );
										ConfigLocation( const ConfigLocation& other );

										/* DESTRUCTORS */
										~ConfigLocation( void );
	
										/* COPY ASSIGNMENT */
		ConfigLocation&					operator=(const ConfigLocation& other);

										// Getters
		const bool						getAutoIndex(void);
		const std::string&				getIndex(void);
		const std::vector<std::string>&	getLocationMethods(void);
		const std::string&				getPath(void);
		const bool						getRedirectStatus(void);
		const std::string&				getRedirectURL(void);
		const std::string&				getRootDir(void);

										// Setters
		void 							setAutoIndex(void);					
		void 							setIndex(void);					
		void 							setPath(void);					
		void 							setRedirectStatus(void);				
		void							setRedirectURL(void);
		void 							setRootDir(void);					

									// Validation
		// Check path is accessible
		// Check methods are valid
		// Check server name isn't duplicated? Check at higher level (in Config)

};

// PRINT LOCATION INFO FOR SINGLE LOCATION (From output stream)
std::ostream&	operator<<(std::ostream &os, const ConfigLocation& location);
