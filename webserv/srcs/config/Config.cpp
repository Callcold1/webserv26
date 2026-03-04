/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:13 by kmooney           #+#    #+#             */
/*   Updated: 2026/03/02 14:24:02 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config() : _max_body(DEFAULT_MAX_BODY), _max_connections(DEFAULT_MAX_CONN),
					_max_header_size(DEFAULT_MAX_HEAD), _timeout_ka(DEFAULT_TM_KA),
					_timeout_rw(DEFAULT_TM_RW), _autoindex_set(false), _charset_set(false),
					_error_page_set(false), _index_set(false), _max_body_set(false),
					_max_connections_set(false), _max_header_set(false), _method_set(false),
					 _root_set(false), _servers_set(false), _timeout_ka_set(false), _timeout_rw_set(false){}

Config::Config(const Config &other) { (void)other;}

Config Config::operator=(const Config &other) {
	if (this != &other)
		return *this;
	return *this;

}

Config::~Config(){}

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
	return _error_page_set;
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

std::map<int, std::string>	Config::getErrorPages() const{
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

int    Config::getServerPortCount(void) const{
	return _port_count;
}

std::map<listen_s, std::vector<CServer*> >	Config::getPortServerMap(void) const{
	return _port_server_map;
}

void	Config::checkForOverlappingPorts(void) const{

	std::map<listen_s, std::vector<CServer*> >::const_iterator it;
	std::map<listen_s, std::vector<CServer*> >::const_iterator stop;
	std::map<listen_s, std::vector<CServer*> >::const_iterator compare;

	for (it = _port_server_map.begin(); it != _port_server_map.end(); it++) {
		if (it->first.ip == 0){
			stop = it;
			stop++;
			continue;
		}
		for (compare = _port_server_map.begin(); compare != stop; compare++) {
			if (it->first.port == compare->first.port)
				throw std::runtime_error("Overlapping ip:port combination found between servers, please resolve!");
		}
	}
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		SETTERS 		<<<<<<
>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
*/

void 	Config::setServers(std::vector<CServer>& servers, bool set_from_config_file) {
	_servers_set = set_from_config_file;
	_servers = servers;
}

void 	Config::setAutoIndex(bool autoindex, bool set_from_config_file){
	_autoindex_set = set_from_config_file;
	_autoindex = autoindex;
}

void 	Config::setCharset(std::vector<std::string>& charset, bool set_from_config_file) {
	_charset_set = set_from_config_file;
	_charset = charset;
}

void 	Config::setErrorPage(std::map<int, std::string>& error_pages, bool set_from_config_file) {
	_error_page_set = set_from_config_file;
	_error_pages = error_pages;
}

void 	Config::setIndex(const std::vector<std::string>& index, bool set_from_config_file){
	_index_set = set_from_config_file;
	_index = index;
}

void 	Config::setMaxBodySize(long long max_body_size, bool set_from_config_file) {
	_max_body_set = set_from_config_file;
	_max_body = max_body_size;
}

void    Config::setMaxConnections(int max_connections, bool set_from_config_file) {
	_max_connections_set = set_from_config_file;
	_max_connections = max_connections;
}

void    Config::setMaxHeaderSize(long long max_header_size, bool set_from_config_file) {
	_max_header_set = set_from_config_file;
	_max_header_size = max_header_size;
}

void 	Config::setMethod(const std::vector<std::string>& method, bool set_from_config_file) {
	_method_set = set_from_config_file;
	_method = method;
}

void	Config::setRootDir(std::string& root_dir, bool set_from_config_file) {
	_root_set = set_from_config_file;
	_root_dir = root_dir;
}

void    Config::setTimeoutKA(int timeout_in_seconds, bool set_from_config_file) {
	_timeout_ka_set = set_from_config_file;
	_timeout_ka = timeout_in_seconds;
}

void    Config::setTimeoutRW(int timeout_in_seconds, bool set_from_config_file) {
	_timeout_rw_set = set_from_config_file;
	_timeout_rw = timeout_in_seconds;
}

void	Config::setServerPortCount(int count) {
	_port_count = count;
}

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		CONFIG PROPAGATION 		<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

/*
	Pass variables from global config to each CServer if they are not set in the CServer

*/
void    Config::propagateVarsToChildren(){
	size_t size = _servers.size();

	for (size_t i = 0; i < size; i++){
		if (_servers[i].isAutoIndexSet() == false)
			_servers[i].setAutoIndex(_autoindex, false);
		if (_servers[i].isCharsetSet() == false)
			_servers[i].setCharset(_charset, false);
		if (_servers[i].isIndexSet() == false)
			_servers[i].setIndex(_index, false);
		if (_servers[i].isMaxBodySet() == false)
			_servers[i].setMaxBodySize(_max_body, false);
		if (_servers[i].isMaxHeaderSet() == false)
			_servers[i].setMaxHeaderSize(_max_header_size, false);
		if (_servers[i].isMethodSet() == false)
			_servers[i].setMethod(_method, false);
		if (_servers[i].isRootDirSet() == false)
			_servers[i].setRootDir(_root_dir, false);
		if (_servers[i].isTimeoutKASet() == false)
			_servers[i].setTimeoutKA(_timeout_ka, false);
		if (_servers[i].isTimeoutRWSet() == false)
			_servers[i].setTimeoutRW(_timeout_rw, false);
		if (_servers[i].isErrorPageSet())
			_servers[i].setErrorPage(mergeErrorPage(_servers[i], _error_pages), false);
		else
			_servers[i].setErrorPage(mergeErrorPage(_servers[i], _error_pages), false);
		propagateServerToLocations(_servers[i]);
	}
}

void    Config::propagateServerToLocations(CServer& server){

	std::vector<CLocation> locations = server.getLocations();
	size_t size = locations.size();

	for (size_t i = 0; i < size; i++){

		if (locations[i].isAutoIndexSet() == false)
			locations[i].setAutoIndex(server.getAutoIndex(), false);
		if (locations[i].isCharsetSet() == false)
			locations[i].setCharset(server.getCharset(), false);
		if (locations[i].isIndexSet() == false)
			locations[i].setIndex(server.getIndex(), false);
		if (locations[i].isMaxBodySet() == false)
			locations[i].setMaxBodySize(server.getMaxBodySize(), false);
		if (locations[i].isMethodSet() == false)
			locations[i].setMethod(server.getMethod(), false);
		if (locations[i].isRootDirSet() == false)
			locations[i].setRootDir(server.getRootDir(), false);
		if (locations[i].isErrorPageSet())
			locations[i].setErrorPage(mergeErrorPage(locations[i], server.getErrorPages()), true);
		else
			locations[i].setErrorPage(mergeErrorPage(locations[i], server.getErrorPages()), false);
	}
	server.setLocations(locations, true);
}

template <typename T>
std::map<int, std::string>	Config::mergeErrorPage(T& child, std::map<int, std::string> _error_pages){

	std::map<int, std::string> child_error_pages = child.getErrorPages();
	std::map<int, std::string>::iterator it;

	for (it = child_error_pages.begin(); it != child_error_pages.end(); it++){
		_error_pages[it->first] = it->second;
	}
	return _error_pages;
}

bool	Config::mapServersToPorts(void){

	for (size_t i = 0; i < _servers.size(); i++) {

		CServer* s = &_servers[i];
		std::vector<listen_s> listens = s->getListen();

		for (size_t j = 0; j < s->getListen().size(); j++) {

			listen_s ls = listens[j];
			std::map<listen_s, std::vector<CServer*> >::iterator it = _port_server_map.find(ls);

			if (it != _port_server_map.end()) {

				if (ls.is_default && it->first.is_default)
					throw std::runtime_error("Two listen directives with same IP and port marked default");
				it->second.push_back(s);
			}
			else {
				_port_server_map[ls].push_back(s);
			}
		}
	}
	_port_count = _port_server_map.size();
	return true;
}

// Needed to compare listen_s structs as keys in map as map uses < operator to compare keys
bool operator<(const listen_s& a, const listen_s& b){
    if (a.ip != b.ip)
        return a.ip < b.ip;
    if (a.port != b.port)
        return a.port < b.port;
    return false;
}

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		PRINT CONFIG INFO 		<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

std::ostream& operator<<(std::ostream& os, const Config& config){

	os << std::boolalpha;
	PrintUtils::printHeader(os, 0, "Global");

	PrintUtils::printField(os, 0, "autoindex", config.getAutoIndex(), config.isAutoIndexSet());
	PrintUtils::printField(os, 0, "charset", config.getCharset(), config.isCharsetSet());
	PrintUtils::printField(os, 0, "error_pages", config.getErrorPages(), config.isErrorPageSet());
	PrintUtils::printField(os, 0, "index", config.getIndex(), config.isIndexSet());
	PrintUtils::printField(os, 0, "max_body_size", config.getMaxBodySize(), config.isMaxBodySet());
	PrintUtils::printField(os, 0, "max_connections", config.getMaxConnections(), config.isMaxConnectionsSet());
	PrintUtils::printField(os, 0, "max_header_size", config.getMaxHeaderSize(), config.isMaxHeaderSet());
	PrintUtils::printField(os, 0, "method", config.getMethod(), config.isMethodSet());
	PrintUtils::printField(os, 0, "root", config.getRootDir().empty() ? "( empty )" : config.getRootDir(), config.isRootDirSet());
	PrintUtils::printField(os, 0, "timeout_keep_alive", config.getTimeoutKA(), config.isTimeoutKASet());
	PrintUtils::printField(os, 0, "timeout_read_write", config.getTimeoutRW(), config.isTimeoutRWSet());

	os << config.getServers();

	os << std::noboolalpha;
	return os;
}

std::ostream& operator<<(std::ostream& os, const CServer& server)
{
	os << std::boolalpha;

	PrintUtils::printField(os, 1, "autoindex", server.getAutoIndex(), server.isAutoIndexSet());
	PrintUtils::printField(os, 1, "charset", server.getCharset(), server.isCharsetSet());
	PrintUtils::printField(os, 1, "error_pages", server.getErrorPages(), server.isErrorPageSet());
	PrintUtils::printField(os, 1, "index", server.getIndex(), server.isIndexSet());
	PrintUtils::printField(os, 1, "listen", server.getListen(), server.isListenSet());
	PrintUtils::printField(os, 1, "max_body_size", server.getMaxBodySize(), server.isMaxBodySet());
	PrintUtils::printField(os, 1, "max_header_size", server.getMaxHeaderSize(), server.isMaxHeaderSet());
	PrintUtils::printField(os, 1, "method", server.getMethod(), server.isMethodSet());
	PrintUtils::printField(os, 1, "root", server.getRootDir().empty() ? "( empty )" : server.getRootDir(), server.isRootDirSet());
	PrintUtils::printField(os, 1, "server_names", server.getServerName(), server.isServerNameSet());
	PrintUtils::printField(os, 1, "timeout_keep_alive", server.getTimeoutKA(), server.isTimeoutKASet());
	PrintUtils::printField(os, 1, "timeout_read_write", server.getTimeoutRW(), server.isTimeoutRWSet());

	os << server.getLocations();

	os << std::noboolalpha;
	return os;
}

std::ostream& operator<<(std::ostream& os, const CLocation& location)
{
	os << std::boolalpha;

	PrintUtils::printField(os, 2, "uri", location.getURI(), location.isURISet());
	PrintUtils::printField(os, 2, "alias", location.getAlias().empty() ? "( empty )" : location.getAlias(), location.isAliasSet());
	PrintUtils::printField(os, 2, "autoindex", location.getAutoIndex(), location.isAutoIndexSet());
	PrintUtils::printField(os, 2, "cgi_map", location.getCGIMap(), location.isCGIMapSet());
	PrintUtils::printField(os, 2, "charset", location.getCharset(), location.isCharsetSet());
	PrintUtils::printField(os, 2, "error_page", location.getErrorPages(), location.isErrorPageSet());
	PrintUtils::printField(os, 2, "index", location.getIndex(), location.isIndexSet());
	PrintUtils::printField(os, 2, "max_body_size", location.getMaxBodySize(), location.isMaxBodySet());
	PrintUtils::printField(os, 2, "method", location.getMethod(), location.isMethodSet());
	PrintUtils::printField(os, 2, "modifier", location.getModifier(), location.isModifierSet());
	PrintUtils::printField(os, 2, "redirect_status", location.getRedirectStatus(), location.isRedirectSet());
	PrintUtils::printField(os, 2, "redirect_url", location.getRedirectURL().empty() ? "( empty )" : location.getRedirectURL(), location.isRedirectSet());
	PrintUtils::printField(os, 2, "root", location.getRootDir().empty() ? "( empty )" : location.getRootDir(), location.isRootDirSet());
	PrintUtils::printField(os, 2, "try_files", location.getTryFiles(), location.isTryFilesSet());
	PrintUtils::printField(os, 2, "upload_path", location.getUploadPath().empty() ? "( empty )" : location.getUploadPath(), location.isUploadPathSet());

	os << std::noboolalpha << std::endl;
	return os;
}

/* VECTOR OF LOCATIONS */
std::ostream&	operator<<(std::ostream &os, const std::vector<CLocation>& locations){
	if (locations.empty()){
		os << PrintUtils::indent(1) << "Locations : [ empty ]\n";
		return os;
	}
	os << std::endl;
	for (size_t i = 0; i < locations.size(); ++i){
		os << PrintUtils::indent(2)
		   << "Location [" << i << "]\n\n";
		os << locations[i];
	}
	return os;
}

/* VECTOR OF SERVERS */
std::ostream& operator<<(std::ostream& os, const std::vector<CServer>& servers) {
	if (servers.empty()){
		os << PrintUtils::indent(2) << "servers : [ empty ]\n";
		return os;
	}
	os << std::endl;
	for (size_t i = 0; i < servers.size(); i++){
		os << PrintUtils::indent(1)
		   << "Server [" << i << "]\n\n";
		os << servers[i];
	}
	return os;
}

/* CGI MAP */
std::ostream&	operator<<(std::ostream& os, const std::map<std::string, std::string>& cgi_map){

	std::map<std::string, std::string>::const_iterator it;

	int count = 0;
	for (it = cgi_map.begin(); it != cgi_map.end(); it++, count++){
		os << "\n" << PrintUtils::indent(14) << "CGI type : " << it->first << " \tCGI Location : " << it->second << std::endl;
	}
	if (count == 0){
		os << PrintUtils::indent(0) << "CGI Map : [ empty ]\n";
		return os;
	}
	return os;
}

/* ERROR PAGES */
std::ostream&	operator<<(std::ostream& os, const std::map<int, std::string>& error_pages){

	std::map<int, std::string>::const_iterator it;

	int count = 0;
	for (it = error_pages.begin(); it != error_pages.end(); it++, count++){
		os << "\n" << PrintUtils::indent(14) << "Error Num : " << it->first << " \tError Page Location : " << it->second;
	}
	if (count == 0){
		os << PrintUtils::indent(0) << " Error Page : [ empty ]\n";
		return os;
	}
	os << "\n";
	return os;
}

/* PORT INFO (LISTEN) */
std::ostream&	operator<<(std::ostream& os, const listen_s& port_info){

	os
	<< " IP : "
	<< ( port_info.ip & 0xFF) << "."
	<< ((port_info.ip >> 8)  & 0xFF) << "."
	<< ((port_info.ip >> 16) & 0xFF) << "."
	<< ((port_info.ip >> 24) & 0xFF)
	<< ", Port : " << port_info.port
	<< ", is default : " << port_info.is_default;

	return os;
}

/* TRY FILES */
std::ostream&	operator<<(std::ostream& os, const tryFiles_s& try_files){

	size_t size = try_files.try_paths.size();

	if (size == 0){
		os << "[ empty ]";
		return os;
	}
	for (size_t i = 0; i < size; i++)
		os << "\ntry_file [" << i << "] path : " << try_files.try_paths[i];
	os << "try_file error_page : " << try_files.default_error;
	return os;
}

std::ostream&	operator<<(std::ostream& os, const std::map <listen_s, std::vector<CServer*> >& port_server_map){
	
	if (port_server_map.size() == 0){
		os << " no value(s)" << std::endl;
		return os;
	}
	
	std::map <listen_s, std::vector<CServer*> >::const_iterator it;

	for (it = port_server_map.begin(); it != port_server_map.end(); it++){
		
		os << "Port info : " << it->first << std::endl;
		
		for (size_t i = 0; i < it->second.size(); i++){
			os << "Server [" << i << "] " << std::endl;
		}
		os << std::endl;
	}
	return os;
}
