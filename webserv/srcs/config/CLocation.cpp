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

bool	CLocation::isAliasSet(void) const{
	return _alias_set;
}

bool	CLocation::isAutoIndexSet(void) const{
	return _autoindex_set;
}

bool	CLocation::isCGIMapSet(void) const{
	return _cgi_map_set;
}

bool	CLocation::isCharsetSet(void) const{
	return _charsets_set;
}

bool	CLocation::isErrorPagesSet(void) const{
	return _error_pages_set;
}

bool	CLocation::isIndexSet(void) const{
	return _index_set;
}

bool	CLocation::isMaxBodySet(void) const{
	return _max_body_set;
}

bool	CLocation::isMethodsSet(void) const{
	return _methods_set;
}

bool	CLocation::isModifierSet(void) const{
	return _modifier_set;
}

bool	CLocation::isRedirectSet(void) const{
	return _redirect_set;
}

bool	CLocation::isRootDirSet(void) const{
	return _root_dir_set;
}

bool	CLocation::isTryFilesSet(void) const{
	return _try_files_set;
}

bool	CLocation::isUploadPathSet(void) const{
	return _upload_path_set;
}

bool	CLocation::isURISet(void) const{
	return _uri_set;
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

std::vector<errorPage_s>	CLocation::getErrorPages(void) const{
	return _error_pages;

}

std::vector<std::string>	CLocation::getIndex(void) const{
	return _index;
}

long long	CLocation::getMaxBodySize(void) const{
	return _max_body;
}

std::vector<std::string>	CLocation::getMethods(void) const{
	return _methods;
}

LocationMatchType_e	CLocation::getModifier(void) const{
	return _modifier;
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

tryFiles_s	CLocation::getTryFiles(void) const{
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
	_alias_set = true;
	_alias = alias;
}

void 	CLocation::setAutoIndex(bool autoindex){
	_autoindex_set = true;
	_autoindex = autoindex;
}

void 	CLocation::setCGIMap(const std::map<std::string, std::string> cgi_map){
	_cgi_map_set = true;
	_cgi_map = cgi_map;
}

void	CLocation::setCharsets(const std::vector<std::string>& charsets){
	_charsets_set = true;
	_charsets = charsets;
}

void	CLocation::setErrorPages(const std::vector<errorPage_s>& error_pages){
	_error_pages_set = true;
	_error_pages = error_pages;
}

void 	CLocation::setIndex(const std::vector<std::string>& index){
	_index_set = true;
	_index = index;
}

void	CLocation::setMaxBodySize(long long max_body_size){
	_max_body_set = true;
	_max_body = max_body_size;
}

void 	CLocation::setMethods(const std::vector<std::string>& methods){
	_methods_set = true;
	_methods = methods;
}

void 	CLocation::setModifier(const LocationMatchType_e modifier){
	_modifier_set = true;
	_modifier = modifier;
}

void 	CLocation::setRedirect(const redirect_s& redirect){
	_redirect_set = true;
	_redirect = redirect;
}

void 	CLocation::setRootDir(const std::string& root_dir){
	_root_dir_set = true;
	_root_dir = root_dir;
}

void	CLocation::setTryFiles(const tryFiles_s& try_files){
	_try_files_set = true;
	_try_files = try_files;
}

void 	CLocation::setUploadPath(const std::string& upload_path){
	_upload_path_set = true;
	_upload_path = upload_path;
}

void	CLocation::setURI(const std::string& uri){
	_uri_set = true;
	_uri = uri;
}
