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
#include "CLocation.hpp"
#include <algorithm>

CServer::CServer( void ) :
	_autoindex(false), _max_body(DEFAULT_MAX_BODY),	_max_header_size(DEFAULT_MAX_HEAD),
	_timeout_ka(DEFAULT_TM_KA),	_timeout_rw(DEFAULT_TM_RW), _autoindex_set(false),
		_charset_set(false), _error_page_set(false), _index_set(false), _listen_set(false), 
		_max_body_set(false),_max_header_set(false),_method_set(false), _root_set(false), 
		_server_name_set(false), _timeout_ka_set(false), _timeout_rw_set(false){}

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
	_listen = other._listen;
	_root_dir = other._root_dir;
	_server_name = other._server_name;
	_timeout_ka = other._timeout_ka;
	_timeout_rw = other._timeout_rw;

	_autoindex_set = other._autoindex_set;
	_charset_set = other._charset_set;
	_error_page_set = other._error_page_set;
	_index_set = other._index_set;
	_listen_set = other._listen_set;
	_max_body_set = other._max_body_set;
	_max_header_set = other._max_header_set;
	_method_set = other._method_set;
	_root_set = other._root_set;
	_server_name_set = other._server_name_set;
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
		_listen = other._listen;
		_root_dir = other._root_dir;
		_server_name = other._server_name;
		_timeout_ka = other._timeout_ka;
		_timeout_rw = other._timeout_rw;

		_autoindex_set = other._autoindex_set;
		_charset_set = other._charset_set;
		_error_page_set = other._error_page_set;
		_index_set = other._index_set;
		_listen_set = other._listen_set;
		_max_body_set = other._max_body_set;
		_max_header_set = other._max_header_set;
		_method_set = other._method_set;
		_root_set = other._root_set;
		_timeout_ka_set = other._timeout_ka_set;
		_timeout_rw_set = other._timeout_rw_set;
		return *this;
	}
	return *this;
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		SORT FUNCTOR	<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

struct compareByLength_f {
    bool operator()(const CLocation& a, const CLocation& b) const {
        return a.getURI().size() > b.getURI().size();
    }
};

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		CHECKERS	<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

bool	CServer::isAutoIndexSet(void) const{
	return _autoindex_set;
}

bool	CServer::isCharsetSet(void) const{
	return _charset_set;
}

bool	CServer::isErrorPageSet(void) const{
	return _error_page_set;
}

bool	CServer::isIndexSet(void) const{
	return _index_set;
}

bool	CServer::isListenSet(void) const{
	return _listen_set;
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

bool	CServer::isRootDirSet(void) const{
	return _root_set;
}

bool	CServer::isServerNameSet(void) const{
	return _server_name_set;
}

bool	CServer::isTimeoutKASet(void) const{
	return _timeout_ka_set;
}

bool	CServer::isTimeoutRWSet(void) const{
	return _timeout_rw_set;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		GETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

bool	CServer::getAutoIndex(void) const{
	return _autoindex;
}

std::vector<std::string>	CServer::getCharset(void) const{
	return _charset;
}

std::map<int,std::string>	CServer::getErrorPages(void) const{
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

std::vector<listen_s>	CServer::getListen(void) const{
	return _listen;
}

const std::vector<std::string>	CServer::getMethod(void) const{
	return _method;
}

std::vector<std::string>	CServer::getServerName(void) const{
	return _server_name;
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

void 	CServer::setAutoIndex(bool autoindex, bool set_from_config_file){
	_autoindex_set = set_from_config_file;
	_autoindex = autoindex;
}

void	CServer::setCharset(std::vector<std::string>& charset, bool set_from_config_file){
	_charset_set = set_from_config_file;
	_charset = charset;
}

void	CServer::setErrorPage(const std::map<int, std::string>& error_pages, bool set_from_config_file){
	_error_page_set = set_from_config_file;
	_error_pages = error_pages;
}

void 	CServer::setIndex(const std::vector<std::string>& index, bool set_from_config_file){
	_index_set = set_from_config_file;
	_index = index;
}

void	CServer::setListen(const std::vector<listen_s>& ports, bool set_from_config_file){
	_listen_set = set_from_config_file;
	_listen = ports;
}

void	CServer::setLocations(const std::vector<CLocation>& locations, bool set_from_config_file){
	_location_set = set_from_config_file;
	_locations = locations;
	
	std::sort(_locations.begin(), _locations.end(), compareByLength_f());
}

void	CServer::setMaxBodySize(long long max_body_size, bool set_from_config_file){
	_max_body_set = set_from_config_file;
	_max_body = max_body_size;
}

void	CServer::setMaxHeaderSize(long long max_header_size, bool set_from_config_file){
	_max_header_set = set_from_config_file;
	_max_header_size = max_header_size;
}

void	CServer::setMethod(const std::vector<std::string>& method, bool set_from_config_file){
	_method_set = set_from_config_file;
	_method = method;
}

void	CServer::setRootDir(std::string& root_dir, bool set_from_config_file){
	_root_set = set_from_config_file;
	_root_dir = root_dir;
}

void	CServer::setServerName(const std::vector<std::string>& names, bool set_from_config_file){
	_server_name_set = set_from_config_file;
	_server_name = names;
}

void    CServer::setTimeoutKA(int timeout_in_seconds, bool set_from_config_file){
	_timeout_ka_set = set_from_config_file;
	_timeout_ka = timeout_in_seconds;
}

void    CServer::setTimeoutRW(int timeout_in_seconds, bool set_from_config_file){
	_timeout_rw_set = set_from_config_file;
	_timeout_rw = timeout_in_seconds;
}

const CLocation*	CServer::getLocationForPath(const std::string& uri_path) const {
	
	size_t size = _locations.size(); 
	
	for (size_t i = 0; i < size; i++) {
        
		const std::string& loc_path = _locations[i].getURI();

        if (uri_path.compare(0, loc_path.size(), loc_path) == 0)
            return &_locations[i];
	}
	return NULL;
}
