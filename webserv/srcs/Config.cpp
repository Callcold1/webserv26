/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:13 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/18 20:56:31 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config() : _max_body(DEFAULT_MAX_BODY), _max_connections(DEFAULT_MAX_CONN),
					_max_header_size(DEFAULT_MAX_HEAD), _timeout_ka(DEFAULT_TM_KA),
                    	_timeout_rw(DEFAULT_TM_RW){
}

Config::Config(const Config &other) { (void)other;}

Config Config::operator=(const Config &other) {
    if (this != &other) {
    }
    return *this;
}

Config::~Config(){
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		GETTERS 		<<<<<<
>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
*/

long long	Config::getMaxBodySize(void) const {
	return _max_body;
}

int	Config::getMaxConnections(void) const {
	return _max_connections;
}

long long	Config::getMaxHeaderSize(void) const {
	return _max_header_size;
}

std::vector<errorPage_s>	Config::getErrorPages() const {
    return _error_pages;
}

std::vector<CServer>	Config::getServers() const {
    return _servers;
}

int    Config::getTimeoutKA (void) const{
    return _timeout_ka;
}

int    Config::getTimeoutRW(void) const{
    return _timeout_rw;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<

>>>>>>		SETTERS 		<<<<<<
>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
*/

void 	Config::setServers(std::vector<CServer>& servers) {
	_servers = servers;
}

void 	Config::setCharsets(std::vector<std::string>& charsets) {
	_charsets = charsets;
}

void 	Config::setErrorPages(std::vector<errorPage_s>& error_pages) {
	_error_pages = error_pages;
}

void 	Config::setMaxBodySize(long long max_body_size) {
	 _max_body = max_body_size;
}

void    Config::setMaxConnections(int max_connections) {
	_max_connections = max_connections;
}

void    Config::setMaxHeaderSize(long long max_header_size) {
	_max_header_size = max_header_size;
}

void    Config::setTimeoutKA(int timeout_in_seconds){
    _timeout_ka = timeout_in_seconds;
}

void    Config::setTimeoutRW(int timeout_in_seconds){
    _timeout_rw = timeout_in_seconds;
}

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		PRINT CONFIG INFO 		<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

std::ostream&	operator<<(std::ostream& os, const std::vector<errorPage_s>& error_pages){

	size_t size = error_pages.size();

    for (size_t i = 0; i < size; i++){
        os << "\tError Num : " << error_pages[i]._error_num << " \tError Page Location : " << error_pages[i]._path << std::endl;
	}
	return os;
}

std::ostream&	operator<<(std::ostream& os, const std::map<std::string, fileInfo_s*>& cgi_map){

	std::map<std::string, fileInfo_s*>::const_iterator it;

    for (it = cgi_map.begin(); it != cgi_map.end(); it++){
        os << "\tCGI type : " << it->first << " \tCGI Location : " << it->second->path << " \tCGI Executable : " << it->second->is_exec << std::endl;
	}
	return os;
}

std::ostream&	operator<<(std::ostream& os, const listen_s& port_info){

	os << "IP address : " << port_info.ip
	<< "Port : " << port_info.port
	<< "is default : " << port_info.is_default;

	return os;
}

std::ostream&	operator<<(std::ostream& os, const std::vector<std::string>& strings){

	size_t size = strings.size();

    for (size_t i = 0; i < size; i++){
        os << "\t" << strings[i] << std::endl;
	}
	return os;
}

std::ostream&	operator<<(std::ostream& os, const std::vector<CLocation*>& locations){

    size_t size = locations.size();

    for (size_t i = 0; i < size; i++)
        std::cout << "\n= Location " << i << " =\n" << locations[i];
    return os;
}

std::ostream&	operator<<(std::ostream& os, const std::vector<CServer>& servers){

    size_t size = servers.size();

    for (size_t i = 0; i < size; i++)
		std::cout << "\n= Server " << i << " =\n" << servers[i];
    return os;
}

std::ostream& operator<<(std::ostream& os, const Config& config){
	os << "\nGlobal\n======"
    	<< "\nMax Body size : " << config.getMaxBodySize()
    	<< "\nMax Connections : " << config.getMaxConnections()
    	<< "\nTimeout (keep-alive) : " << config.getTimeoutKA() << " seconds"
    	<< "\nTimeout (read-write) : " << config.getTimeoutRW() << " seconds"
    	<< "\n\nGlobal Error Pages :\n" << config.getErrorPages()
    	<< "\n\nServers\n=======\n" << config.getServers();
	return os;
}

std::ostream&	operator<<(std::ostream& os, const CServer& server){

	os	<< "\nServer Max Body Size : " << server.getMaxBodySize()
    	<< "\nServer Max Header : " << server.getMaxHeaderSize()
		<< "\nServer root dir : " << server.getRootDir()
		<< "\n\nServer error page(s)\n" << server.getErrorPages()
		<< "\n\nServer accepted method(s)\n" << server.getMethods()
		<< "\n\nServer name(s)\n" << server.getNames()
		<< "\n\nServer port(s)\n" << server.getListen()
		<< "\n\nServer location(s)\n" << server.getLocations()
		<< std::endl;
	return os;
}

std::ostream&	operator<<(std::ostream& os, const CLocation& location){

	os << "Location\n=========\n"
		<< "\nLocation alias" << location.getAlias()
		<< "\nLocation autoindex" << location.getAutoIndex()
		<< "\nLocation index" << location.getIndex()
		<< "\nLocation name" << location.getName()
		<< "\nLocation path" << location.getURI()
		<< "\nLocation redirect status" << location.getRedirectStatus()
		<< "\nLocation redirect url" << location.getRedirectURL()
		<< "\nLocation root dir" << location.getRootDir()
		<< "\nLocation upload path" << location.getUploadPath()
		//<< "\n\nLocation accepted CGI types" << location.getCGIMap()	// Need to create << function
		<< "\n\nLocation accepted charset(s)" << location.getCharsets()
		<< "\n\nLocation accepted method(s)" << location.getMethods()
		<< std::endl;

	return os;
}

template<typename T>
std::ostream&	operator<<(std::ostream& os, const std::vector<T> vec){
	size_t size = vec.size();

    for (size_t i = 0; i < size; i++)
        os << vec[i];
    return os;
}

// std::ostream&	operator<<(std::ostream &os, const std::map<int, std::string>& error_pages){

// 	std::map<int, std::string>::const_iterator it;

// 	os << " Server Error Pages\n=================\n";
// 	for (it = error_pages.begin(); it != error_pages.end(); it++)
// 		os << "Error Code : " << it->first << " Error Page Path : " << it->second << '\n';
// 	return os;
// }

// std::ostream&	operator<<(std::ostream &os, const std::vector<std::string>& methods){

// 	std::vector<std::string>::const_iterator it;

// 	os << " Server Permitted Methods: " << *it << '\n';
// 	for (it = methods.begin(); it != methods.end(); it++)
// 		os << "Methods  : " << *it << '\n';
// 	return os;
// }

std::ostream&	operator<<(std::ostream &os, const std::vector<CLocation>& locations){

	std::vector<CLocation>::const_iterator it;

	os << " Locations\n=========\n";
	int i = 0;
	for (it = locations.begin(); it != locations.end(); it++, i++)
		os << "Location " << i << " : " <<  *it << '\n';
	return os;
}