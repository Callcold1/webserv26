/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:13 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/22 22:19:20 by kmooney          ###   ########.fr       */
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

bool	Config::isAutoIndexSet(void) const{
	return _autoindex_set;
}

bool	Config::isCharsetSet(void) const{
	return _charset_set;
}

bool	Config::isErrorPageSet(void) const{
	return _error_pages_set;
}

bool	Config::isIndexSet(void) const{
	return _index_set;
}

bool	Config::isMaxBodySet(void) const{
	return _max_body_set;
}

bool	Config::isMaxConnectionsSet(void) const{
	return _max_connections_set;
}

bool	Config::isMaxHeaderSet(void) const{
	return _max_header_set;
}

bool	Config::isMethodsSet(void) const{
	return _methods_set;
}

bool	Config::isRootDirSet(void) const{
	return _root_set;
}

bool	Config::isServerSet(void) const{
	return _servers_set;
}

bool	Config::isTimeoutKASet(void) const{
	return _timeout_ka_set;
}

bool	Config::isTimeoutRWSet(void) const{
	return _timeout_rw_set;
}


/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		GETTERS 		<<<<<<
>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
*/

bool	Config::getAutoIndex(void) const{
	return _autoindex;
}

std::vector<std::string>	Config::getCharsets(void) const{
	return _charsets;
}

std::vector<errorPage_s>	Config::getErrorPages() const{
	return _error_pages;
}

std::vector<std::string>	Config::getIndex(void) const{
	return _index;
}

long long	Config::getMaxBodySize(void) const{
	return _max_body;
}

int	Config::getMaxConnections(void) const{
	return _max_connections;
}

long long	Config::getMaxHeaderSize(void) const{
	return _max_header_size;
}

std::vector<std::string>	Config::getMethods(void) const{
	return _methods;
}

std::string	Config::getRootDir(void) const{
	return _root_dir;
}

std::vector<CServer>	Config::getServers() const{
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

void 	Config::setAutoIndex(bool autoindex){
	_autoindex = autoindex;
}

void 	Config::setCharsets(std::vector<std::string>& charsets) {
	_charsets = charsets;
}

void 	Config::setErrorPages(std::vector<errorPage_s>& error_pages) {
	_error_pages = error_pages;
}

void 	Config::setIndex(const std::vector<std::string>& index){
	_index = index;
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

void 	Config::setMethods(const std::vector<std::string>& methods){
	_methods = methods;
}

void	Config::setRootDir(std::string& root_dir){
	_root_dir = root_dir;
}

void    Config::setTimeoutKA(int timeout_in_seconds){
	_timeout_ka = timeout_in_seconds;
}

void    Config::setTimeoutRW(int timeout_in_seconds){
	_timeout_rw = timeout_in_seconds;
}

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		CONFIG PROPAGATION 		<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

/*
	Pass variables from global config to each CServer if they are not set in the CServer

*/
// void    Config::propagateGlobalToServer(){

// 	size_t size = _servers.size();

// 	for (size_t i = 0; i < size; i++){
// 		CServer server = _servers[i];
// 		if (_autoindex && _servers[i].)
// 	}
// }

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		PRINT CONFIG INFO 		<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

std::ostream&	operator<<(std::ostream& os, const std::map<std::string, std::string>& cgi_map){

	std::map<std::string, std::string>::const_iterator it;

	for (it = cgi_map.begin(); it != cgi_map.end(); it++){
		os << "\tCGI type : " << it->first << " \tCGI Location : " << it->second << std::endl;
	}
	return os;
}

std::ostream&	operator<<(std::ostream& os, const std::vector<errorPage_s>& error_pages){

	size_t size = error_pages.size();

	for (size_t i = 0; i < size; i++){
		os << "\tError Num : " << error_pages[i]._error_num << " \tError Page Location : " << error_pages[i]._path << std::endl;
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

std::ostream&	operator<<(std::ostream &os, const std::vector<CLocation>& locations){

	std::vector<CLocation>::const_iterator it;

	os << "\nLocations\n=========\n";
	int i = 0;
	for (it = locations.begin(); it != locations.end(); it++, i++)
		os << "Location " << i << " : " <<  *it << '\n';
	return os;
}

std::ostream&	operator<<(std::ostream& os, const std::vector<CServer>& servers){

	size_t size = servers.size();

	for (size_t i = 0; i < size; i++)
		std::cout << "\n= Server " << i << " =\n" << servers[i];
	return os;
}

template<typename T>
std::ostream&	operator<<(std::ostream& os, const std::vector<T> vec){
	size_t size = vec.size();

	for (size_t i = 0; i < size; i++)
		os << vec[i];
	return os;
}

std::ostream& operator<<(std::ostream& os, const Config& config){
	os << "\nGlobal\n======"
		<< "\nMax Body size : " << config.getMaxBodySize()
		<< "\nMax Connections : " << config.getMaxConnections()
		<< "\nMax Header size : " << config.getMaxHeaderSize()
		<< "\nTimeout (keep-alive) : " << config.getTimeoutKA() << " seconds"
		<< "\nTimeout (read-write) : " << config.getTimeoutRW() << " seconds"
		<< "\n\nCharsets :\n" << config.getCharsets()
		<< "\nError Pages :\n" << config.getErrorPages()
		<< "\nServers\n=======\n" << config.getServers();
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
		<< "\nTimeout (keep-alive) : " << server.getTimeoutKA() << " seconds"
		<< "\nTimeout (read-write) : " << server.getTimeoutRW() << " seconds"
		<< std::endl;
	return os;
}

std::ostream&	operator<<(std::ostream& os, const CLocation& location){

	os << "Location\n=========\n"
		<< "\nLocation alias" << location.getAlias()
		<< "\nLocation autoindex" << location.getAutoIndex()
		<< "\nLocation index" << location.getIndex()
		<< "\nLocation path" << location.getURI()
		<< "\nLocation redirect status" << location.getRedirectStatus()
		<< "\nLocation redirect url" << location.getRedirectURL()
		<< "\nLocation root dir" << location.getRootDir()
		<< "\nLocation upload path" << location.getUploadPath()
		<< "\n\nLocation accepted CGI types" << location.getCGIMap()
		<< "\n\nLocation accepted charset(s)" << location.getCharsets()
		<< "\n\nLocation accepted method(s)" << location.getMethods()
		<< std::endl;

	return os;
}
