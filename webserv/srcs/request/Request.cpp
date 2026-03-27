/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junruh <junruh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:28:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/03/24 14:59:42 by junruh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "RequestParser.hpp"
#include "RequestValidator.hpp" 

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
>>>>>>		CONSTRUCTORS		<<<<<<
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/

Request::Request(void) : _parser(NULL), _validator(NULL), _listen_instance(NULL), 
		 _matched_server(NULL), _method(""), _uri_str(""), _version(""),
		_errNum(200), _errMess(""), _p_state (P_PARSE), _r_state (P_REQLINE), _content_length(0),
		_unparsed_length(0), _hasBody(false), _isChunked(false), _isMultiPart(false), _bodyUTF8(false){}

Request::Request( listenInstance_s* listen_instance ) : _parser(NULL), _validator(NULL), _listen_instance(listen_instance),
		_matched_server(NULL),  _method(""), _uri_str(""), _version(""),
		_errNum(200), _errMess(""), _p_state (P_PARSE), _r_state (P_REQLINE), _content_length(0),
		_unparsed_length(0), _hasBody(false), _isChunked(false), _isMultiPart(false), _bodyUTF8(false){}

Request::Request(const Request &other) : _parser(NULL), _validator(NULL), _listen_instance(other._listen_instance),
	_matched_location(other._matched_location), _matched_server(other._matched_server),  _method(other._method),
	_uri_str(other._uri_str), _version(other._version), _req_line(other._req_line), _uri(other._uri), _headers(other._headers), 
	_body(other._body),_boundary(other._boundary), _default_mp_content_type(other._default_mp_content_type), _mp_m(other._mp_m),
	_errNum(other._errNum), _errMess(other._errMess), _p_state(other._p_state), _r_state(other._r_state), 
	_content_length(other._content_length), _body_length(other._body_length), _unparsed_length(other._unparsed_length),
	_hasBody(other._hasBody), _isChunked(other._isChunked), _isMultiPart(other._isMultiPart), _bodyUTF8(other._bodyUTF8){}
				
Request& Request::operator=(const Request &other){
	if (this != &other){
		_listen_instance = other._listen_instance;
		_matched_location = other._matched_location;
		_matched_server = other._matched_server;
		_method = other._method;
		_uri_str = other._uri_str;
		_version = other._version;
		_req_line = other._req_line;
		_uri = other._uri;
		_headers = other._headers;
		_body = other._body;
		_boundary = other._boundary;
		_default_mp_content_type = other._default_mp_content_type;
		_mp_m = other._mp_m;
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

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		DESTRUCTOR		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

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
	_default_mp_content_type.clear();

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

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		PARSE REQ		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

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

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		GETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

const std::string&	Request::getErrMsg(void) const{
	return (_errMess);
}

const std::string&	Request::getMethod()const{
	return _method;
}

const CLocation* Request::getMatchedLocation()const{
	return _matched_location;
}

const CServer* Request::getMatchedServer(void) const {
	return _matched_server;
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

std::string	Request::getRequestStr(){
	_req_line = _parser->getParsedString();
	return _req_line;
}

std::string	Request::getUnparsedStr(){
	return _parser->tokenToString(_unparsed);
}

std::string Request::get_root_dir()
{
	return(_matched_location->getRootDir());
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		SETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

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
	_default_mp_content_type = type;
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
void	Request::setHostname(const std::string& hostname){
	_host_header = hostname;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		FIND MATCHING SERVER/LOCATION		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
*/

bool	Request::matchServerToHostHeader(void){
	
	std::string hostname;

	size_t pos = _host_header.find(':');

	if (pos != std::string::npos){
		int port;

	    hostname = _host_header.substr(0, pos);
	    port = atoi(_host_header.substr(pos + 1).c_str());
		if (!(port >= 0 && port <= 65535))
			return this->setState(P_ERROR, "Malformed Request : Invalid Port Number", 400);
	}
	else
	    hostname = _host_header;

	if (_listen_instance->name_to_srv.find(hostname) == _listen_instance->name_to_srv.end() &&
			_listen_instance->name_to_srv.find(_host_header) == _listen_instance->name_to_srv.end())
		if (_version == "HTTP/1.0")
			_matched_server = _listen_instance->def_srv;
		else
			return false;
	else
		_matched_server = (_listen_instance->name_to_srv.find(hostname))->second;
	return true;		
}

void	Request::matchLocationToURIPath(void){
	
	char	buffer[1024];
	
	getcwd(buffer, sizeof(buffer));
	_matched_location = _matched_server->getLocationForPath(_uri.path, _re.leftover_path);
	_re.location_path = _matched_location->getURI();// temporary (testing respond_t)
	_re.location_root = _matched_location->getRootDir(); // temporary (testing respond_t)
	appendPathToRoot(&_re.target, _re.leftover_path, _re.location_root); // temporary (testing respond_t)
	_re.appended_root = _re.target.path;
	appendPathToRoot(&_re.target, _re.appended_root, buffer);
	getFilePermissions(_re.target);
	std::cout << "TARGET PATH" << _re.target.path << std::endl;

}
