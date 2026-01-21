/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 10:47:44 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 16:02:30 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigServer.hpp"

ConfigServer::ConfigServer( void ){}

ConfigServer::~ConfigServer( void ){}

ConfigServer::ConfigServer(const ConfigServer &other){}

ConfigServer& ConfigServer::operator=(const ConfigServer& other) {
	(void) other;
	return *this;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		GETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

const std::map<int, std::string>&	ConfigServer::getErrorPages(void) const {
	return _error_pages;
}

const std::vector<std::string>&	ConfigServer::getMethods(void) const {
	return _methods;
}

const std::string&	ConfigServer::getName(void) const {
	return _name;
}

int	ConfigServer::getPort(void) const {
	 return _port;
}

const std::string&	ConfigServer::getRootDir(void) const {
	return _root_dir;
}

const std::vector<ConfigLocation>&	ConfigServer::getServerLocations(void) const { //NEED SEARCH TERM AS PARAMETER?
	return _locations;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		FINDERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

bool	ConfigServer::findErrorPage(int error_code, std::string& path) {
	
	std::map<int, std::string>::iterator it = _error_pages.find(error_code);
	
	if (it == _error_pages.end())
		return false;
	path = it->second;
	return true;
}

ConfigLocation*	ConfigServer::findLocation(const std::string& uri) {
	
	std::vector<ConfigLocation>::iterator it;

	for ( it = _locations.begin(); it != _locations.end(); it++) {
		if (it->match_uri(uri))
			return &(*it);
	}
	return NULL;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		CHECKERS	<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

bool	ConfigServer::checkServerMethod(const std::string& method) const{
	
	std::vector<std::string>::const_iterator it;
	std::vector<std::string>::const_iterator end = _methods.end();

	for (it = _methods.begin(); it != end; it++){
		if (method.compare(*it) == 0)
			return true;
	}
	return false;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		SETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

void	ConfigServer::setErrorPage(int error_status, std::string error_page){
	_error_pages[error_status] = error_page; 
}

void	ConfigServer::setLocation(ConfigLocation& location){
	_locations.push_back(location);
}

void	ConfigServer::setMethods(const std::vector<std::string> &allowed_methods){
	_methods = allowed_methods; 
}

void	ConfigServer::setName(std::string name){
	_name = name; 
}

void	ConfigServer::setPort(int port){
	if (port <= 0 || port > 65535)
		throw std::runtime_error("Invalid port");
	 _port = port;
}

void	ConfigServer::setRootDirConfig(std::string root_dir){
	_root_dir = root_dir;
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		PRINTING		<<<<<<\n
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

std::ostream&	operator<<(std::ostream &os, const ConfigServer& server)
{
	os << "Server Error Pages: " << server.getErrorPages() << "\n"
	<< "Server Methods: " << server.getMethods() << "\n"
	<< "Server Name: " << server.getName() << "\n"
	<< "Server Port: " << server.getPort() << "\n"
	<< "Server Root Dir: " << server.getRootDir() << "\n"
	<< "Server Locations: " << server.getServerLocations() << "\n";
	printLocation(os, location);
	//printPermittedMethods(os);
	return os;
}

std::ostream&	operator<<(std::ostream &os, const std::map<int, std::string>& error_pages){

	std::map<int, std::string>::const_iterator it;

	os << " Server Error Pages\n=================\n";
	for (it = error_pages.begin(); it != error_pages.end(); it++){
		os << "Error Code : " << it->first << " Error Page Path : " << it->second << '\n';
	}
}

std::ostream&	operator<<(std::ostream &os, const std::vector<std::string>& methods){

	std::vector<std::string>::const_iterator it;

	os << " Server Permitted Methods: " << *it << '\n';
	for (it = methods.begin(); it != methods.end(); it++){
		os << "Methods  : " << *it << '\n';
	}
}

std::ostream&	operator<<(std::ostream &os, const std::vector<ConfigLocation>& locations){

	std::vector<ConfigLocation>::const_iterator it;

	os << " Locations\n=========\n";
	int i = 0;
	for (it = locations.begin(); it != locations.end(); it++, i++){
		os << "Location " << i << " : " <<  *it << '\n';
	}
}