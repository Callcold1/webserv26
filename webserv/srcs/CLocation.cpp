/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CLocation.cpp            		                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 10:47:58 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 13:15:09 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CLocation.hpp"

CLocation::CLocation( void ) : _autoindex(false), _modifier(NONE){}

CLocation::~CLocation( void ){}

CLocation::CLocation(const CLocation &other){(void)other;}

CLocation& CLocation::operator=(const CLocation &other) {
	(void) other;
	return *this;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		GETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/
std::string	CLocation::getAlias(void) const{
	return _alias;
}

bool	CLocation::getAutoIndex(void) const{
	return _autoindex;
}

bool	CLocation::getCGIEnabled(void) const{
	return _cgi_map.size() > 0;
}

std::map<std::string, std::string>	CLocation::getCGIMap(void) const{
	return _cgi_map;
}

std::vector<std::string>	CLocation::getCharsets(void) const{
	return _charsets;
}

std::vector<std::string>	CLocation::getIndex(void) const{
	return _index;
}

std::vector<std::string>	CLocation::getMethods(void) const{
	return _methods;
}

LocationMatchType_e	CLocation::getModifier(void) const{
	return _modifier;
}

std::string	CLocation::getName(void) const{
	return _name;
}

redirect_s	CLocation::getRedirect(void) const{
	return _redirect;
}

bool	CLocation::getRedirectStatus(void) const{
	return _redirect.status;
}

std::string	CLocation::getRedirectURL(void) const{
	return _redirect.url;
}

std::string	CLocation::getRootDir(void) const{
	return _root_dir;
}

std::vector<tryFiles_s>	CLocation::getTryFiles(void) const{
	return _try_files;
}

std::string	CLocation::getUploadPath(void) const{
	return _upload_path;
}

std::string	CLocation::getURI(void) const{
	return _uri;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		SETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

void	CLocation::setAlias(const std::string& alias){
	_alias = alias;
}

void 	CLocation::setAutoIndex(bool autoindex){
	_autoindex = autoindex;
}

void 	CLocation::setCGIMap(const std::map<std::string, std::string> cgi_map){
	_cgi_map = cgi_map;
}

void	CLocation::setCharsets(const std::vector<std::string>& charsets){
	_charsets = charsets;
}

void 	CLocation::setIndex(const std::vector<std::string>& index){
	_index = index;
}

void 	CLocation::setMethods(const std::vector<std::string>& methods){
	_methods = methods;
}

void 	CLocation::setName(const std::string& name){
	_name = name;
}

void 	CLocation::setModifier(const LocationMatchType_e modifier){
	_modifier = modifier;
}

void 	CLocation::setRedirect(const redirect_s& redirect){
	_redirect = redirect;
}

void 	CLocation::setRootDir(const std::string& root_dir){
	_root_dir = root_dir;
}

void	CLocation::setTryFiles(const std::vector<tryFiles_s>& try_files){
	_try_files = try_files;
}

void 	CLocation::setUploadPath(const std::string& upload_path){
	_upload_path = upload_path;
}

void	CLocation::setURI(const std::string& uri){
	_uri = uri;
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		CHECKERS		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

bool	CLocation::match_uri(const std::string& uri){
	//LOGIC
	if (uri.length() > 0)
		return true;
	return false;
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		PRINTING		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

// std::ostream&	operator<<(std::ostream &os, const CLocation& location)
// {
// 	os << "Autoindex: " << location.getAutoIndex() << "\n"
// 	<< "Index File: " << location.getIndex() << "\n"
// 	<< "redirect_s Status: " << location.getRedirectStatus() << "\n"
// 	<< "redirect_s URL: " << location.getRedirectURL() << "\n"
// 	<< "Root Dir: " << location.getRootDir() << "\n"
// 	<< "URI: " << location.getURI() << "\n";

// 	return os;
// }