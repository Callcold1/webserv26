/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CServer.cpp                       		            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 10:47:44 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/10 14:50:02 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CServer.hpp"

CServer::CServer( void ){}

CServer::~CServer( void ){}

CServer::CServer(const CServer &other){(void)other;}

CServer& CServer::operator=(const CServer& other) {
	(void) other;
	return *this;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		GETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

bool	CServer::isAutoIndexSet(void) const{
	return _autoindex_set;
}

bool	CServer::isCharsetSet(void) const{
	return _charset_set;
}

bool	CServer::isIndexSet(void) const{
	return _index_set;
}

bool	CServer::isMaxBodySet(void) const{
	return _max_body_set;
}

bool	CServer::isMaxHeaderSet(void) const{
	return _max_header_set;
}
bool	CServer::isMethodsSet(void) const{
	return _methods_set;
}
bool	CServer::isNamesSet(void) const{
	return _ports_set;
}
bool	CServer::isRootDirSet(void) const{
	return _root_set;
}
bool	CServer::isTimeoutKASet(void) const{
	return _timeout_ka_set;
}
bool	CServer::isTimeoutRWSet(void) const{
	return _timeout_rw_set;
}

bool	CServer::getAutoIndex(void) const{
	return _autoindex;
}

std::vector<std::string>	CServer::getCharsets(void) const{
	return _charsets;
}

std::vector<errorPage_s>	CServer::getErrorPages(void) const{
	return _error_pages;
}

std::vector<std::string>	CServer::getIndex(void) const{
	return _index;
}

std::vector<CLocation>	CServer::getLocations(void) const{
	return _locations;
}

long long	CServer::getMaxBodySize(void) const{
	return _max_body;
}

long long	CServer::getMaxHeaderSize(void) const{
	return _max_header_size;
}

const std::vector<std::string>	CServer::getMethods(void) const{
	return _methods;
}

std::vector<std::string>	CServer::getNames(void) const{
	return _server_names;
}

std::vector<listen_s>	CServer::getListen(void) const{
	return _ports;
}

std::string	CServer::getRootDir(void) const{
	return _root_dir;
}

int    CServer::getTimeoutKA (void) const{
    return _timeout_ka;
}

int    CServer::getTimeoutRW(void) const{
    return _timeout_rw;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		SETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/


void 	CServer::setAutoIndex(bool autoindex){
	_autoindex_set = true;
	_autoindex = autoindex;
}

void	CServer::setCharsets(std::vector<std::string>& charsets){
	_charset_set = true;
	_charsets = charsets;
}

void	CServer::setErrorPages(const std::vector<errorPage_s>& error_pages){
	_error_pages = error_pages;
}

void 	CServer::setIndex(const std::vector<std::string>& index){
	_index_set = true;
	_index = index;
}

void	CServer::setLocations(const std::vector<CLocation>& locations){
	_locations = locations;
}

void	CServer::setMaxBodySize(long long max_body_size){
	_max_body_set = true;
	_max_body = max_body_size;
}

void	CServer::setMaxHeaderSize(long long max_header_size){
	_max_header_set = true;
	_max_header_size = max_header_size;
}

void	CServer::setMethods(const std::vector<std::string>& methods){
	_methods_set = true;
	_methods = methods;
}

void	CServer::setServerNames(const std::vector<std::string>& names){
	_server_names = names;
}

void	CServer::setListen(const std::vector<listen_s>& ports){
	_ports_set = true;
	_ports = ports;
}

void	CServer::setRootDir(std::string& root_dir){
	_root_set = true;
	_root_dir = root_dir;
}

void    CServer::setTimeoutKA(int timeout_in_seconds){
	_timeout_ka_set = true;
	_timeout_ka = timeout_in_seconds;
}

void    CServer::setTimeoutRW(int timeout_in_seconds){
	_timeout_rw_set = true;
	_timeout_rw = timeout_in_seconds;
}

/*
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		CSERVER PROPAGATION 		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
*/
void	CServer::propagateServerToLocation(){

}
