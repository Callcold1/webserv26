/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 10:47:58 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/19 14:34:04 by kmooney          ###   ########.fr       */
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
	return ;
}

const std::string&	ConfigLocation::getRootDir(void) const{
	return ;
}	

const std::string&	ConfigLocation::getIndex(void) const{
	return ;
}	

const bool	ConfigLocation::getAutoIndex(void) const{
	return ;
}	

const bool	ConfigLocation::getRedirectStatus(void) const{
	return ;
}	

const std::string&	ConfigLocation::getRedirectURL(void) const{
	return ;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		SETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

void ConfigLocation::setAutoIndex(void){

}	

void ConfigLocation::setIndex(void){

}	

void ConfigLocation::setPath(void){

}

void ConfigLocation::setRedirectStatus(void){

}	

void ConfigLocation::setRedirectURL(void){

}

void ConfigLocation::setRootDir(void){

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