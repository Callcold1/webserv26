/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junruh <junruh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:28:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/03/06 16:11:00 by junruh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "RequestParser.hpp"
#include "RequestValidator.hpp" 

/* CONSTRUCTORS */
Request::Request(void) : _parser(NULL), _validator(NULL), _config(NULL), _listen_instance(NULL), _method(""), _uri_str(""), _version(""),
		_errNum(200), _errMess(""), _p_state (P_PARSE), _r_state (P_REQLINE), _content_length(0),
		_unparsed_length(0), _hasBody(false), _isChunked(false), _isMultiPart(false), _bodyUTF8(false){}

Request::Request( Config* config )
	: _parser(NULL), _validator(NULL), _config(config), _listen_instance(NULL), _method(""), _uri_str(""), _version(""),
		_errNum(200), _errMess(""), _p_state (P_PARSE), _r_state (P_REQLINE), _content_length(0),
		_unparsed_length(0), _hasBody(false), _isChunked(false), _isMultiPart(false), _bodyUTF8(false){}

Request::Request( listenInstance_s* listen_instance )
	: _parser(NULL), _validator(NULL), _listen_instance(listen_instance), _method(""), _uri_str(""), _version(""),
		_errNum(200), _errMess(""), _p_state (P_PARSE), _r_state (P_REQLINE), _content_length(0),
		_unparsed_length(0), _hasBody(false), _isChunked(false), _isMultiPart(false), _bodyUTF8(false){}

Request::Request(const Request &other) : _parser(NULL), _validator(NULL), _config(other._config), _listen_instance(other._listen_instance), _method(other._method),
	_uri_str(other._uri_str), _version(other._version), _req_line(other._req_line), _uri(other._uri), _headers(other._headers), _body(other._body),
	_boundary(other._boundary), _defaultMultiType(other._defaultMultiType), _multipart(other._multipart), _errNum(other._errNum), _errMess(other._errMess), 
	_p_state(other._p_state), _r_state(other._r_state), _content_length(other._content_length), _body_length(other._body_length), _unparsed_length(other._unparsed_length),
	 _hasBody(other._hasBody), _isChunked(other._isChunked), _isMultiPart(other._isMultiPart), _bodyUTF8(other._bodyUTF8){}
				
Request& Request::operator=(const Request &other){
	if (this != &other){
		_config = other._config;
		_listen_instance = other._listen_instance;
		_method = other._method;
		_uri_str = other._uri_str;
		_version = other._version;
		_req_line = other._req_line;
		_uri = other._uri;
		_headers = other._headers;
		_body = other._body;
		_boundary = other._boundary;
		_defaultMultiType = other._defaultMultiType;
		_multipart = other._multipart;
		_errNum = other._errNum;
		_errMess = other._errMess;
		_p_state = other._p_state;
		_r_state = other._r_state;
		_content_length = other._content_length;
		_body_length = other._body_length;
		_unparsed_length = other._unparsed_length;
		_hasBody = other._hasBody;
		_isChunked = other._isChunked;
		_isMultiPart = other._isMultiPart;
		_bodyUTF8 = other._bodyUTF8;
	}	
	return *this;
}



/* DESTRUCTORS */
Request::~Request( void ){
	if (_parser){
		for (fieldMap_t::iterator it = _headers.begin(); it != _headers.end(); it++){
			size_t size = it->second.size();
			
			for (size_t i = 0; i < size; i++){ 
				delete it->second[i];
			}
		}
		if (_parser)
			delete _parser;
		_parser = NULL;
		if (_validator)
			delete _validator;
		_validator = NULL;
	}
}

void	Request::clear() {
	_method.clear();
	_uri_str.clear();
	_version.clear();
	_req_line.clear();
	_uri = uri_t();
	_headers.clear();
	_body.clear();
	_boundary.clear();
	_defaultMultiType.clear();

	_errNum = 200;
	_errMess.clear();

	_p_state = P_PARSE;
	_r_state = P_REQLINE;

	_content_length = 0;

	_hasBody = false;
	_isChunked = false;
	_isMultiPart = false;
	_bodyUTF8 = false;

	if (_parser)
		_parser->clear();
	// if (_validator) {
	// 	_validator->clear();
	//}
}

void	Request::resetParser() {
    if (_parser)
        _parser->clear();
}

eParse	Request::parseRequest(const char* rawData, size_t length) {
	
	if (!_parser)
		_parser = new RequestParser(this);
	if (!_validator)
		_validator = new RequestValidator(this);
	_parser->parse(rawData, length);
	while (!(_p_state == P_AWAIT || _p_state == P_ERROR || _p_state == P_DONE)){
		_validator->validate();
		if (_p_state == P_PARSE)
			_parser->parseRequest();
	}
	return _p_state;
}

eParse	Request::getParseStatus(){
	return _p_state;
}

eParse	Request::getParseStatus(void) const {
	return _p_state;
}

eReqState	Request::getReqStatus(){
	return _r_state;
}

std::string	Request::getUnparsedStr(){
	return _parser->tokenToString(_unparsed);
}

void	Request::setRstate(eReqState r_state){
	_r_state = r_state;
}

void	Request::setPstate(eParse p_state){
	_p_state = p_state;
}

bool	Request::setState(eParse p_state, std::string err_mess, unsigned int err_num){
	_p_state = p_state;
	_errMess = err_mess;
	_errNum = err_num;
	return false;
}

bool	Request::setState(eParse p_state, eReqState r_state, std::string err_mess, unsigned int err_num){
	_p_state = p_state;
	_r_state = r_state;
	_errMess = err_mess;
	_errNum = err_num;
	return false;
}

void	Request::setBodyType( bool hasBody, bool isChunked ){
	
	_hasBody = hasBody;
	_isChunked = isChunked;
}

void	Request::setBodyEncoding( const std::string& encoding ){ // CAN ALTER FUNCTION TO UPDATE ENUM IF WE SUPPORT MORE ENCODINGS
																// JUST NOW ASSUME UTF-8 or OCTET
	if (encoding.compare("utf-8") == 0)
		_bodyUTF8 = true;
}

void	Request::setBoundaryString( const std::string& boundary ){
	_boundary = boundary;
}

void	Request::setDefaultMultiPartType( const std::string& type ){
	_defaultMultiType = type;
}

std::string	Request::getRequestStr(){
	_req_line = _parser->getParsedString();
	return _req_line;
}

std::vector<field_t* >	 Request::findHeader(const std::string& header_name){
	fieldMap_t::iterator header = _headers.find(header_name);
	
	if (header == _headers.end())
        return std::vector<field_t*>(); 
	
	return header->second;
}

// void	 Request::iterateOverFieldT(std::vector<field_t*>& vec ){

// 	size_t size = vec.size();

// 	for (size_t i = 0; i < size; i++){
// 	const std::string&	 val	 = vec[i]->val;
// 	const std::vector< std::pair<std::string, std::string> >&	 options = vec[i]->params;
// 	}
// }

std::string Request::findQuery(const std::string id)  //function to get specific query value 
{
    for (query_t::iterator it = _uri.queries.begin(); it != _uri.queries.end(); ++it) {
        if (it->first == id) {
            return it->second;
        }
    }
    return "";
}
// void	Request::iterateOverFieldT(std::vector<field_t*>& vec ){
	
// 	size_t size = vec.size();
	
// 	for (size_t i = 0; i < size; i++){
// 		const std::string&	 										val 	= vec[i]->val;
// 		const std::vector< std::pair<std::string, std::string> >&	options = vec[i]->params;
// 	}
// }

void	Request::setHostname(const std::string& hostname){
	_host_header = hostname;
}

// Possibly best to put this in the parser for control/consistency?
void	Request::matchServerToHostHeader(void){
	if (_host_header.length() == 0){
		Log::instance().log(Log::ERROR, Log::TYPE_REQUEST_PARSING, "No host header provided : server set to default_server"); // Not an error for HTTP 1.0 but is for HTTP 1.1?
	}
	size_t size = _listen_instance->server_configs.size();
	if (size == 0){
		Log::instance().log(Log::ERROR, Log::TYPE_ERROR, "No server config for ip:port ");
		//throw exception
	}

	for (size_t i = 0; i < size; i++){
		
		CServer* server = _listen_instance->server_configs[i];
		std::vector<std::string> server_names = server->getServerName();
		size_t name_size = server_names.size();
		
		for (size_t j = 0; j < name_size; j++){
			
			if (server_names[j].compare(_host_header) == 0){
				_matched_server = server;
				return;
			}
		}
	}
	_matched_server = _listen_instance->default_server;
	Log::instance().log(Log::INFO, Log::TYPE_REQUEST_PARSING, "matching server = " + printstream(_matched_server));
}

// Possibly best to put this in the parser for control/consistency?
void	Request::matchLocationToURIPath(void){
	
	if (_matched_server == NULL){
		 //return error
	}
	
	int 					longest;
	std::string				remainder;
	CLocation*				best_location = NULL;
	std::vector<CLocation> 	locations = _matched_server->getLocations();
	size_t					size = locations.size();
	
	for (size_t i = 0; i < size; i++){
		
		std::string	leftover;
		int			match_len = 0;

		match_len = comparePaths(_uri.path, locations[i].getURI(), leftover);
		if (match_len > longest){
			longest = match_len;
			remainder = leftover;
			best_location = &locations[i];
		}
	}
	_matched_location = best_location;
	Log::instance().log(Log::INFO, Log::TYPE_REQUEST_PARSING, "matching location = " + printstream(_matched_location));
	Log::instance().log(Log::INFO, Log::TYPE_REQUEST_PARSING, "remaining uri = " + remainder );
}

int	Request::comparePaths(std::string uri_path, std::string location_path, std::string& leftover){
	
	int matching_segments = 0;

	std::istringstream uri_stream(uri_path);
	std::istringstream location_stream(location_path);

	std::vector<std::string> uri_segments = split_path(uri_stream);
	std::vector<std::string> location_segments = split_path(location_stream);

	int uri_size = uri_segments.size();
	int location_size = location_segments.size();
	for (int i = 0; i < uri_size && i < location_size; i++, matching_segments++){
		if (uri_segments[i].compare(location_segments[i]) != 0){
			break;
		}
	}
	if (matching_segments < uri_size){
		leftover = rebuild_path(uri_segments.begin() + matching_segments, uri_segments.end());
	}
	return matching_segments;
}
