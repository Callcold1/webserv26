/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:28:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/24 13:52:20 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "RequestParser.hpp"
#include "RequestValidator.hpp" 

/* CONSTRUCTORS */
Request::Request(void) : _parser(NULL), _validator(NULL), _config(NULL), _method(""), _uri_str(""), _version(""),
		_errNum(200), _errMess(""), _p_state (P_PARSE), _r_state (P_REQLINE), _content_length(0),
		_unparsed_length(0), _hasBody(false), _isChunked(false), _isMultiPart(false), _bodyUTF8(false){}

Request::Request( Config* config )
	: _parser(NULL), _validator(NULL), _config(config), _method(""), _uri_str(""), _version(""),
		_errNum(200), _errMess(""), _p_state (P_PARSE), _r_state (P_REQLINE), _content_length(0),
		_unparsed_length(0), _hasBody(false), _isChunked(false), _isMultiPart(false), _bodyUTF8(false){}
				
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

void	Request::parseRequest(const char* rawData, size_t length) {
	
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
}

eParse	Request::getParseStatus(){
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

std::vector<field_t* >	Request::findRequestHeaderValue(const std::string& header_name){
	fieldMap_t::iterator header = _headers.find(header_name);
	return header->second;
}

// void	Request::iterateOverFieldT(std::vector<field_t*>& vec ){
	
// 	size_t size = vec.size();
	
// 	for (size_t i = 0; i < size; i++){
// 		const std::string&	 										val 	= vec[i]->val;
// 		const std::vector< std::pair<std::string, std::string> >&	options = vec[i]->params;
// 	}
// }