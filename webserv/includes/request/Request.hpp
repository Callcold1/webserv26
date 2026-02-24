
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:57:35 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/10 11:25:50 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core_def.hpp"
#include "utilityFunctions.hpp"
#include "RequestTypes.hpp"
#include "Config.hpp"

class RequestParser;
class RequestValidator;

class Request {
	
	public:
						Request(Config* config);
						~Request(void);
	void	    		clear();
	
	void				parseRequest(const char* rawData, size_t length);

	eParse				getParseStatus();
	eReqState			getReqStatus();
	std::string			getRequestStr();
	std::string			getUnparsedStr();

	void 				setRstate(eReqState r_state);
	void 				setPstate(eParse p_state);

	bool 				setState(eParse p_state, std::string err_mess, unsigned int err_num );
	bool 				setState(eParse p_state, eReqState r_state, std::string err_mess, unsigned int err_num );
	
	void				setBodyType( bool hasBody, bool isChunked );
	void				setBodyEncoding( const std::string& encoding );
	void				setBoundaryString( const std::string& boundary );
	void				setDefaultMultiPartType( const std::string& type );

	std::vector<field_t* >		findRequestHeaderValue(const std::string& header_name);
	
	RequestParser*		_parser;
	RequestValidator*	_validator;
	//RequestHandler*		_requestHandler;
	Config*			    _config;// really the server block info
	std::deque<char>	_buffer;
	token_s				_unparsed;

	std::string 		_method, _uri_str, _version, _req_line;
	uri_t				_uri;
	fieldMap_t 			_headers;
	fieldMap_t 			_trailers;
	std::string 		_body;
	//Body				_newBody;
	std::string 		_boundary;
	std::string 		_defaultMultiType;

	void                resetParser();
	void                resetValidator();
	unsigned int		_errNum;
	std::string			_errMess;

	eParse				_p_state;
	eReqState			_r_state;
	
	uint64_t			_content_length, _body_length, _unparsed_length; //_content_length is used when content length is provided. body_length is used to track kegnth parsed

	bool 				_hasBody, _isChunked, _isMultiPart, _bodyUTF8;
};

std::ostream& l14(std::ostream& os);
std::ostream& r3(std::ostream& os);
std::ostream& operator<<(std::ostream& os, Request& request);
