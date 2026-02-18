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

std::vector<std::string>	CServer::getCharsets(void) const{
	return _charsets;
}

std::vector<errorPage_s>	CServer::getErrorPages(void) const{
	return _error_pages;
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

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		SETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

void	CServer::setCharsets(std::vector<std::string>& charsets){
	_charsets = charsets;
}

void	CServer::setErrorPages(const std::vector<errorPage_s>& error_pages){
	_error_pages = error_pages;
}

void	CServer::setLocations(const std::vector<CLocation>& locations){
	_locations = locations;
}

void	CServer::setMaxBodySize(long long max_body_size){
	_max_body = max_body_size;
}

void	CServer::setMaxHeaderSize(long long max_header_size){
	_max_header_size = max_header_size;
}

void	CServer::setMethods(const std::vector<std::string>& methods){
	_methods = methods;
}

void	CServer::setNames(const std::vector<std::string>& names){
	_server_names = names;
}

void	CServer::setListen(const std::vector<listen_s>& ports){
	_ports = ports;
}

void	CServer::setRootDir(std::string& root_dir){
	_root_dir = root_dir;
}

// /*
// >>>>>>>>>>>>>>><<<<<<<<<<<<<<<
// >>>>>>		FINDERS		<<<<<<
// >>>>>>>>>>>>>>><<<<<<<<<<<<<<<
// */

// bool	CServer::findErrorPage(int error_code, std::string& path) {

// 	std::map<int, std::string>::iterator it = _error_pages.find(error_code);

// 	if (it == _error_pages.end())
// 		return false;
// 	path = it->second;
// 	return true;
// }

// bool	CServer::matchLocation(const std::string& path, size_t& pos){
// 	size_t size = _locations.size();
// 	for (size_t i = 0; i < size; i++){
// 		if (_locations[i].getPath() == path){
// 			return true;
// 		}
// 	}
// 	return false;
// }

// /*
// >>>>>>>>>>>>>>><<<<<<<<<<<<<<<
// >>>>>>		CHECKERS	<<<<<<
// >>>>>>>>>>>>>>><<<<<<<<<<<<<<<
// */

// bool	CServer::checkServerMethod(const std::string& method) const{

// 	std::vector<std::string>::const_iterator it;
// 	std::vector<std::string>::const_iterator end = _methods.end();

// 	for (it = _methods.begin(); it != end; it++){
// 		if (method.compare(*it) == 0)
// 			return true;
// 	}
// 	return false;
// }
