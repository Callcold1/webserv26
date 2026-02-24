/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:13 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/24 15:14:37 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config() : _max_body(DEFAULT_MAX_BODY), _max_connections(DEFAULT_MAX_CONN),
					_max_header_size(DEFAULT_MAX_HEAD), _timeout_ka(DEFAULT_TM_KA),
						_timeout_rw(DEFAULT_TM_RW){
}

Config::Config(const Config &other) { (void)other;}

Config Config::operator=(const Config &other) {
	if (this != &other) 
		return *this;
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

bool	Config::isCharset(void) const{
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

bool	Config::isMethodSet(void) const{
	return _method_set;
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

std::vector<std::string>	Config::getCharset(void) const{
	return _charset;
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

std::vector<std::string>	Config::getMethod(void) const{
	return _method;
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

void 	Config::setCharset(std::vector<std::string>& charset) {
	_charset = charset;
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

void 	Config::setMethod(const std::vector<std::string>& method){
	_method = method;
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

std::ostream& operator<<(std::ostream& os, const Config& config){
	os << "\nGlobal\n======"
		<< "\nGlobal : autoindex : " << std::boolalpha << config.getAutoIndex()							//	Bool
		<< "\nGlobal : charset :\n" << config.getCharset()												//	Vector of strings
		<< "\nGlobal : Error Pages :\n" << config.getErrorPages()										//	Error Pages
		<< "\nGlobal : index : \n" << config.getIndex()													//	Vector of strings
		<< "\nGlobal : Max Body size : " << config.getMaxBodySize()										//	long long						(No function required)
		<< "\nGlobal : Max Connections : " << config.getMaxConnections()								//	int								(No function required)
		<< "\nGlobal : Max Header size : " << config.getMaxHeaderSize()									//	long long						(No function required)
		<< "\nGlobal : method : \n" << config.getMethod()												// 	Vector of strings
		<< "\nGlobal : root : " << config.getRootDir()													//	string 							(No function required)
		<< "\nGlobal : Timeout (keep-alive) : " << config.getTimeoutKA() << " seconds"					//	int
		<< "\nGlobal : Timeout (read-write) : " << config.getTimeoutRW() << " seconds"					//	int
		<< std::noboolalpha
		<< "\n\nServers\n=======\n" << config.getServers();
	return os;
}

std::ostream&	operator<<(std::ostream& os, const CServer& server){

	os	
		<< "\nServer : autoindex : " << std::boolalpha << server.getAutoIndex()							//	Bool
		<< "\nServer : charset :\n" << server.getCharset()												//	Vector of strings
		<< "\nServer : Error Pages :\n" << server.getErrorPages()										//	Error Pages
		<< "\nServer : index : \n" << server.getIndex()													//	Vector of strings
		<< "\n\nServer : port(s)\n" << server.getListen()
		<< "\nServer : Max Body size : " << server.getMaxBodySize()										//	long long						(No function required)
		<< "\nServer : Max Header size : " << server.getMaxHeaderSize()									//	long long						(No function required)
		<< "\nServer : method : \n" << server.getMethod()												// 	Vector of strings
		<< "\nServer : root : " << server.getRootDir()													//	string 							(No function required)
		<< "\n\nServer : name(s)\n" << server.getNames()
		<< "\nServer : Timeout (keep-alive) : " << server.getTimeoutKA() << " seconds"					//	int
		<< "\nServer : Timeout (read-write) : " << server.getTimeoutRW() << " seconds"					//	int
		<< "\n\nServer : location(s)\n" << server.getLocations()
		<< std::endl;
	return os;
}

std::ostream&	operator<<(std::ostream& os, const CLocation& location){

	std::string alias = location.getAlias();
	if (alias.length() == 0)
		alias = "No value";
	std::string redirect_url = location.getRedirectURL();
	if (redirect_url.length() == 0)
		redirect_url = "No value";
	std::string upload_path = location.getUploadPath();
	if (upload_path.length() == 0)
		upload_path = "No value";
	std::string uri = location.getURI();
	if (uri.length() == 0)
		uri = "No value";

	os
	<< "\nLocation : alias : " << alias 													//	string							(No function required)
	<< "\nLocation : autoindex : " << std::boolalpha << location.getAutoIndex()							//	Bool							(No function required)
	<< "\nLocation : cgi_path : \n" << location.getCGIMap()												//	CGIMap							
	<< "\nLocation : charset : \n" << location.getcharset()												//	Vector of strings
	<< "\nLocation : error_page : " << location.getErrorPages()											//	Error Pages
	<< "\nLocation : index : \n" << location.getIndex()													//	Vector of strings
	<< "\nLocation : max_body : " << location.getMaxBodySize()											//	long long						(No function required)
	<< "\nLocation : method : \n" << location.getMethod()												// 	Vector of strings
	<< "\nLocation : modifier : " << location.getModifier()												//	Character (n = none)			(No function required)
	<< "\nLocation : redirect_status : " << std::boolalpha << location.getRedirectStatus()				//  bool 							(No function required)
	<< "\nLocation : redirect_url : " << redirect_url													//	string 							(No function required)
	<< "\nLocation : root : " << location.getRootDir()													//	string 							(No function required)
	<< "\nLocation : try_files : \n" << location.getTryFiles()											//	Try Files
	<< "\nLocation : upload_path : " << location.getUploadPath()										//	string							(No function required) 
	<< "\nLocation : uri : " << location.getURI()														//	string							(No function required) 
	<< std::noboolalpha	<< std::endl;

	return os;
}

/* VECTOR OF LOCATIONS */
std::ostream&	operator<<(std::ostream &os, const std::vector<CLocation>& locations){

	std::vector<CLocation>::const_iterator it;

	os << "\nLocations\n=========\n";
	int i = 0;
	for (it = locations.begin(); it != locations.end(); it++, i++)
		os << "Location " << i << " : " <<  *it << '\n';
	if (i == 0){
		os << "\tNo value(s)" << std::endl;
		return os;
	}
	return os;
}

/* VECTOR OF SERVERS */
std::ostream&	operator<<(std::ostream& os, const std::vector<CServer>& servers){

	size_t size = servers.size();

	if (size == 0){
		os << "\tNo value(s)" << std::endl;
		return os;
	}
	for (size_t i = 0; i < size; i++)
		std::cout << "\n= Server " << i << " =\n" << servers[i];
	return os;
}

/* CGI MAP */
std::ostream&	operator<<(std::ostream& os, const std::map<std::string, std::string>& cgi_map){

	std::map<std::string, std::string>::const_iterator it;

	int count = 0;
	for (it = cgi_map.begin(); it != cgi_map.end(); it++, count++){
		os << "\tCGI type : " << it->first << " \tCGI Location : " << it->second << std::endl;
	}
	if (count == 0){
		os << "\tNo value(s)" << std::endl;
		return os;
	}
	return os;
}

/* ERROR PAGES */
std::ostream&	operator<<(std::ostream& os, const std::vector<errorPage_s>& error_pages){

	size_t size = error_pages.size();

	if (size == 0){
		os << "\tNo value(s)" << std::endl;
		return os;
	}
	for (size_t i = 0; i < size; i++){
		os << "\tError Num : " << error_pages[i]._error_num << " \tError Page Location : " << error_pages[i]._path << std::endl;
	}
	return os;
}

/* PORT INFO (LISTEN) */
std::ostream&	operator<<(std::ostream& os, const listen_s& port_info){

	os << "\tIP address : " << port_info.ip
	<< "\tPort : " << port_info.port
	<< "\tis default : " << port_info.is_default << std::endl;

	return os;
}

/* TRY FILES */
std::ostream&	operator<<(std::ostream& os, const tryFiles_s& try_files){

	size_t size = try_files.try_paths.size();

	if (size == 0){
		os << "\tNo value(s)" << std::endl;
		return os;
	}
	for (size_t i = 0; i < size; i++)
		os << "\ttry_file [" << i << "] path : " << try_files.try_paths[i] << std::endl;
	os << "\ttry_file error_page : " << try_files.default_error << std::endl;
	return os;
}

/* VECTOR OF ANYTHING */
template<typename T>
std::ostream&	operator<<(std::ostream& os, const std::vector<T> vec){
	size_t size = vec.size();

	if (size == 0){
		os << "\tNo value(s)" << std::endl;
		return os;
	}
	for (size_t i = 0; i < size; i++)
		os << vec[i];
	return os;
}

/* VECTOR OF STRINGS */
std::ostream&	operator<<(std::ostream& os, const std::vector<std::string>& strings){

	size_t size = strings.size();

	if (size == 0){
		os << "\tNo value(s)" << std::endl;
		return os;
	}
	for (size_t i = 0; i < size; i++){
		os << "\t" << strings[i] << std::endl;
	}
	return os;
}
