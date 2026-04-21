/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:28:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/04/21 19:08:09 by kmooney          ###   ########.fr       */
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

Request::Request(void) : 
	_parser(NULL), 
	_validator(NULL), 
	_listen_instance(NULL), 
	_matched_server(NULL), 
	_method(""), 
	_uri_str(""), 
	_version(""),
	_errNum(200), 
	_errMess(""), 
	_p_state (P_PARSE), 
	_r_state (P_REQLINE),
	_hasBody(false), 
	_isCGI(false), 
	_isChunked(false), 
	_isMultiPart(false),
	_bodyUTF8(false),
	_isRedirect(false),
	_body_complete(false){}

Request::Request( listenInstance_s* listen_instance ) : 
	_parser(NULL), 
	_validator(NULL), 
	_listen_instance(listen_instance),
	_matched_server(NULL), 
	_method(""), 
	_uri_str(""), 
	_version(""),
	_errNum(200), 
	_errMess(""), 
	_p_state (P_PARSE),
	_r_state (P_REQLINE), 
	_hasBody(false), 
	_isCGI(false), 
	_isChunked(false), 
	_isMultiPart(false), 
	_bodyUTF8(false),
	_isRedirect(false),
	_body_complete(false){}

Request::Request(const Request &other) : _parser(NULL), 
	_validator(NULL), 
	_listen_instance(other._listen_instance),
	_matched_location(other._matched_location), 
	_matched_server(other._matched_server), 
	_method(other._method),
	_uri_str(other._uri_str), 
	_version(other._version), 
	_req_line(other._req_line), 
	_uri(other._uri), 
	_headers(other._headers),
	_content_length(other._content_length),
	_body(other._body),
	_boundary(other._boundary), 
	_default_mp_content_type(other._default_mp_content_type), 
	_mp_m(other._mp_m),
	_errNum(other._errNum), 
	_errMess(other._errMess), 
	_p_state(other._p_state), 
	_r_state(other._r_state),
	_sizes(other._sizes),
	_hasBody(other._hasBody), 
	_isCGI(other._isCGI), 
	_isChunked(other._isChunked), 
	_isMultiPart(other._isMultiPart), 
	_bodyUTF8(other._bodyUTF8),
	_isRedirect(other._isRedirect),
	_body_complete(other._body_complete),
	_re(other._re),
	_leftover(other._leftover){}
				
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
		_content_length = other._content_length;
		_body = other._body;
		_boundary = other._boundary;
		_default_mp_content_type = other._default_mp_content_type;
		_mp_m = other._mp_m;
		_errNum = other._errNum;
		_errMess = other._errMess;
		_p_state = other._p_state;
		_r_state = other._r_state;
		_sizes = other._sizes;
		_hasBody = other._hasBody;
		_isCGI = other._isCGI;
		_isChunked = other._isChunked;
		_isMultiPart = other._isMultiPart;
		_bodyUTF8 = other._bodyUTF8;
		_isRedirect = other._isRedirect;
		_body_complete = other._body_complete;
		_re = other._re;
		_leftover = other._leftover;
	}
	return *this;
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		DESTRUCTOR		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

Request::~Request( void ){
	
	deleteHeaders();
	if (_parser)
		delete _parser;
	_parser = NULL;
	if (_validator)
		delete _validator;
	_validator = NULL;
}

void	Request::clear() {

	size_t remainder = _unparsed.end - _unparsed.begin;

	if (remainder)
		_buffer.erase(_buffer.begin(), _buffer.begin() + remainder);
	else
		_buffer.erase(_buffer.begin(), _buffer.end());

	_unparsed.begin = 0;
	_unparsed.end = remainder;
	
	_matched_location = NULL;
	_method.clear();
	_uri_str.clear();
	_version.clear();
	_req_line.clear();
	_host_header.clear();
	_uri.reset();
	deleteHeaders();
	_content_length.clear();
	_body.clear();
	_body_filename.clear();
	_boundary.clear();
	_default_mp_content_type.clear();
	_sizes.reset(remainder);

	_errNum = 200;
	_errMess.clear();

	_p_state = P_PARSE;
	_r_state = P_REQLINE;

	_hasBody = false;
	_isCGI = false; 
	_isChunked = false;
	_isMultiPart = false;
	_bodyUTF8 = false;
	_body_complete = false;
	
	deleteHeaders();
	_re.reset();

    if (_parser)
        _parser->clear();
}

void	Request::deleteHeaders(void){

	for (fieldMap_t::iterator it = _headers.begin(); it != _headers.end(); it++){
		
		size_t size = it->second.size();
		
		for (size_t i = 0; i < size; i++){
			delete it->second[i];
			it->second[i] = NULL;
		}
	}
	_headers.clear();
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		PARSE REQ		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

eParse	Request::parseRequest(const char* rawData, size_t length) {
	_sizes.bytes_received += length;
	
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
	if (_matched_server)
		checkSizes();
	updateRe();
	return _p_state;
}

/*
>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<
>>>>>>		SIZE CHECKS		<<<<<<
>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<
*/
bool	Request::checkSizes(void){

	if (_r_state <= V_HEADERS && _sizes.bytes_read > (size_t)_matched_server->getMaxHeaderSize())
		return this->setState(P_ERROR, "Malformed Request : Header size > maximum header size", 400);

	if (_r_state > V_HEADERS && (_sizes.body_bytes_expected > (size_t)_matched_location->getMaxBodySize()
		 || _sizes.body_bytes_read > (size_t)_matched_location->getMaxBodySize())){
		return this->setState(P_ERROR, "Malformed Request : Body size > maximum body size", 400);
	}
	return true;
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

std::string Request::get_root_dir(){
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

	if (hasBody == false) // BODY IS NOT EXPECTED 
		_body_type = BODY_NONE;
	// BODY IS EXPECTED
	else if (isChunked)
		_body_type = BODY_CHUNKED;
	else if (_sizes.body_bytes_expected > 0)
		_body_type = BODY_CONTENT_LEN;
	else if (_version == "HTTP/1.0")
		_body_type = BODY_EOF;
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
	Log::instance().log(Log::INFO, Log::TYPE_REQUESTS, err_mess + toString(err_num), false);
	return false;
}

bool	Request::setState(eParse p_state, eReqState r_state, std::string err_mess, unsigned int err_num){
	_p_state = p_state;
	_r_state = r_state;
	_errMess = err_mess;
	_errNum = err_num;
	Log::instance().log(Log::INFO, Log::TYPE_REQUESTS, err_mess + ": " + toString(err_num), false);
	Log::instance().log(Log::ERROR, Log::TYPE_ERROR, err_mess + ": " + toString(err_num), true);
	return false;
}

std::vector<field_t* >	 Request::findHeader(const std::string& header_name){
	fieldMap_t::iterator header = _headers.find(header_name);
	
	if (header == _headers.end())
        return std::vector<field_t*>();
	
	return header->second;
}

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
			_listen_instance->name_to_srv.find(_host_header) == _listen_instance->name_to_srv.end()){
		if (_version == "HTTP/1.0")
			_matched_server = _listen_instance->def_srv;
		else
			return false;
	}
	else
		_matched_server = (_listen_instance->name_to_srv.find(hostname))->second;
	_re.server = _matched_server;
	return true;
}

bool	Request::matchLocationToURIPath(void){
	
	_matched_location = _matched_server->getLocationForPath(_uri.path, _leftover);
	if (!_matched_location)
		return false;
	if (_matched_location->isCGIMapSet())
		_isCGI = true;
	
	_re.location = _matched_location;
	updateTargetPathInfo(_leftover);

	return true;
}

/*
>>>>>>		UPDATE _RE		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

void	Request::updateTargetPathInfo(std::string& leftover){

	char	cwd[2048] = {0};
	cwd[0] = '\0';
	//getcwd(cwd, sizeof(cwd));

	_re.leftover_path = leftover;
	std::string fs_root = appendPathToRoot(_matched_location->getRootDir(), cwd); 			// base path : i.e. cwd + location_root
	appendPathToRoot(&_re.paths.resource_path, leftover, fs_root); 				  			// base path + leftover uri (i.e. target resource)
	getFilePermissions(_re.paths.resource_path);
	
	if (_matched_location->isRedirectSet()){
		_re.paths.redirect_url = _matched_location->getRedirectURL();				   		// redirect URI from location (no interaction with filesystem)
		_re.status = _matched_location->getRedirectStatus();
		_isRedirect = true;
	}
	_re.paths.upload_base = appendPathToRoot(_matched_location->getUploadPath(), cwd);		//  upload path is not appended to location root
}

void	Request::updateErrorPagePath(int errnum){
	
	char	cwd[2048];
	//getcwd(cwd, sizeof(cwd));
	cwd[0] = '\0';
	
	std::string target;
	std::string root;
	
	if (_matched_location){
		target = _matched_location->getErrorPage(errnum);
		target = _matched_location->getErrorPage(errnum);
		root = _matched_location->getRootDir();
	}
	else if (_matched_server){
		target = _matched_server->getErrorPage(errnum);
		target = _matched_server->getErrorPage(errnum);
		root = _matched_server->getRootDir();
	}
	if (target.length() > 0 && root.length() > 0){
		std::string	fs_root = appendPathToRoot(root, cwd);
		std::string	error_page = appendPathToRoot(target, fs_root);
		getFileInfo(error_page, _re.paths.error_path);
	}
	else
		Log::instance().log(Log::INFO, Log::TYPE_CLASS, "No default error page found", true);
}

void	Request::updateRe(void){

	_re.status = this->_errNum;
	_re.message = this->_errMess;
	_re.query_pairs = this->_uri.queries;
	_re.body_in_file = this->_bodyInFile;
	
	if (_p_state == P_ERROR){
		_re.type = RESPOND_ERROR;
		updateErrorPagePath(_errNum);
		updateErrorPagePath(_errNum);
	}
	else if (_p_state == P_DONE){

		if (_isMultiPart && _re.paths.upload_paths.size() > 0){
			_re.type = RESPOND_MP_UP;
			//update upload pages if file uploaded
			//i.e. need to include URL of upload path/filename in response
		}
		else if (!_isMultiPart && _method == "POST"){
			for (size_t i = 0; i < _re.query_pairs.size(); i++){
				if (_re.query_pairs[i].first == "importfile" || _bodyInFile ){ 			// ADD || _isCGI?, or what if no filename?, or what if no importfile?  
					if (_matched_location->isUploadPathSet()){
						std::string tmp_path = appendPathToRoot(_re.leftover_path, _re.paths.upload_base);
						std::string upload_path = appendPathToRoot(_re.query_pairs[i].second, tmp_path); //possibly need to normalise/decode if URL encoded?
						fileInfo_s upload_finfo;
						getFileInfo(upload_path, upload_finfo);
						_re.paths.upload_paths.push_back(std::pair<fileInfo_s, std::string>(upload_finfo, _body_filename));
					}
				}
			}
		}
		else if (_isRedirect){
			_re.type = RESPOND_REDIR;
			_re.status = _matched_location->getRedirectStatus(); // duplication?
		}
		else if (_isChunked){
			//_re.type = RESPOND_CHUNKED;
			//update upload page if saved to file?
		}
		else if (_isCGI){
			_re.type = RESPOND_CGI;
			_re.status = this->_errNum;
		}
		else {
			_re.type = RESPOND_STATIC;
		}
	}
}