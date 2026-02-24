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

CServer::CServer( void ) :
	_autoindex(false), _max_body(DEFAULT_MAX_BODY),	_max_header_size(DEFAULT_MAX_HEAD),
	_timeout_ka(DEFAULT_TM_KA),	_timeout_rw(DEFAULT_TM_RW), _autoindex_set(false),
		_charset_set(false), _index_set(false),	_max_body_set(false),_max_header_set(false),
		_method_set(false),	_ports_set(false), _root_set(false), _timeout_ka_set(false),
		_timeout_rw_set(false){}

CServer::~CServer( void ){}

CServer::CServer(const CServer &other){
	_autoindex = other._autoindex;
	_charset = other._charset;
	_error_pages = other._error_pages;
	_index = other._index;
	_locations = other._locations;
	_max_body = other._max_body;
	_max_header_size = other._max_header_size;
	_method = other._method;
	_ports = other._ports;
	_root_dir = other._root_dir;
	_server_names = other._server_names;
	_timeout_ka = other._timeout_ka;
	_timeout_rw = other._timeout_rw;

	_autoindex_set = other._autoindex_set;
	_charset_set = other._charset_set;
	_index_set = other._index_set;
	_max_body_set = other._max_body_set;
	_max_header_set = other._max_header_set;
	_method_set = other._method_set;
	_ports_set = other._ports_set;
	_root_set = other._root_set;
	_timeout_ka_set = other._timeout_ka_set;
	_timeout_rw_set = other._timeout_rw_set;
}

CServer& CServer::operator=(const CServer& other) {
	if (this != &other){
		_autoindex = other._autoindex;
		_charset = other._charset;
		_error_pages = other._error_pages;
		_index = other._index;
		_locations = other._locations;
		_max_body = other._max_body;
		_max_header_size = other._max_header_size;
		_method = other._method;
		_ports = other._ports;
		_root_dir = other._root_dir;
		_server_names = other._server_names;
		_timeout_ka = other._timeout_ka;
		_timeout_rw = other._timeout_rw;

		_autoindex_set = other._autoindex_set;
		_charset_set = other._charset_set;
		_index_set = other._index_set;
		_max_body_set = other._max_body_set;
		_max_header_set = other._max_header_set;
		_method_set = other._method_set;
		_ports_set = other._ports_set;
		_root_set = other._root_set;
		_timeout_ka_set = other._timeout_ka_set;
		_timeout_rw_set = other._timeout_rw_set;
		return *this;
	}
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

bool	CServer::isCharset(void) const{
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
bool	CServer::isMethodSet(void) const{
	return _method_set;
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

std::vector<std::string>	CServer::getCharset(void) const{
	return _charset;
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

const std::vector<std::string>	CServer::getMethod(void) const{
	return _method;
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

void	CServer::setCharset(std::vector<std::string>& charset){
	_charset_set = true;
	_charset = charset;
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

void	CServer::setMethod(const std::vector<std::string>& method){
	_method_set = true;
	_method = method;
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
