/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 10:47:58 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 13:15:09 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigLocation.hpp"

ConfigLocation::ConfigLocation( void ){}

ConfigLocation::~ConfigLocation( void ){}

ConfigLocation::ConfigLocation(const ConfigLocation &other){}

ConfigLocation& ConfigLocation::operator=(const ConfigLocation &other) {
	(void) other;
	return *this;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		GETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

const std::string&	ConfigLocation::getPath(void) const{
	return _path;
}

const std::string&	ConfigLocation::getRootDir(void) const{
	return _root_dir;
}	

const std::string&	ConfigLocation::getIndex(void) const{
	return _index ;
}	

const bool	ConfigLocation::getAutoIndex(void) const{
	return _autoindex;
}	

const bool	ConfigLocation::getRedirectStatus(void) const{
	return _redirect_status;
}	

const std::string&	ConfigLocation::getRedirectURL(void) const{
	return _redirect_url;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		SETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

void	ConfigLocation::setAutoIndex(bool autoindex){
	_autoindex = autoindex;
}	

void	ConfigLocation::setIndex(const std::string& index){
	_index = index;
}	

void	ConfigLocation::setPath(const std::string& path){
	_path = path;
}

void	ConfigLocation::setRedirectStatus(bool redirect_status){
	_redirect_status = redirect_status;
}	

void	ConfigLocation::setRedirectURL(const std::string& redirect_url){
	_redirect_url = redirect_url;
}

void	ConfigLocation::setRootDir(const std::string& root_dir){
	_root_dir = root_dir;
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		CHECKERS		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

bool	ConfigLocation::match_uri(const std::string& uri){
	//LOGIC
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		PRINTING		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

std::ostream&	operator<<(std::ostream &os, const ConfigLocation& location)
{
	os << "Autoindex: " << location.getAutoIndex() << "\n"
	<< "Index File: " << location.getIndex() << "\n"
	<< "Path: " << location.getPath() << "\n"
	<< "Redirect Status: " << location.getRedirectStatus() << "\n"
	<< "Redirect URL: " << location.getRedirectURL() << "\n"
	<< "Root Dir: " << location.getRootDir() << "\n";
	printLocation(os, location);
	//printPermittedMethods(os);
	return os;
}