
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
#include "configDefs.hpp"
#include "Log.hpp"

class RequestParser;
class RequestValidator;

class Request {
	
	public:
						Request();
						Request( listenInstance_s* listen_instance);
						Request(const Request &other);
						Request& operator=(const Request &other);
						~Request(void);

	eParse				parseRequest(const char* rawData, size_t length);
	bool				checkSizes(void);
	void	    		clear();
	void				deleteHeaders(void);
	
						/* GETTERS */
	const std::string&	getErrMsg(void) const;
	const std::string&	getMethod()const;
	const CLocation*	getMatchedLocation()const;
	const CServer*		getMatchedServer(void) const;
	eParse				getParseStatus(void);
	eParse				getParseStatus(void) const;
	eReqState			getReqStatus();
	std::string			getRequestStr();
	std::string			getUnparsedStr();
	std::string			get_root_dir();

						/* SETTERS */

	void				setBodyType( bool hasBody, bool isChunked);
	void				setBodyEncoding( const std::string& encoding);
	void				setBoundaryString( const std::string& boundary);
	void				setDefaultMultiPartType( const std::string& type);
	std::vector<field_t* >	findHeader(const std::string& header_name);
	void	 			iterateOverFieldT(std::vector<field_t*>& vec);
	std::string			findQuery(const std::string id);
	
	void				setHostname( const std::string& hostname); 
	void 				setPstate(eParse p_state);
	void 				setRstate(eReqState r_state);
	bool 				setState(eParse p_state, std::string err_mess, unsigned int err_num);
	bool 				setState(eParse p_state, eReqState r_state, std::string err_mess, unsigned int err_num);

	bool				matchServerToHostHeader(void);
	bool				matchLocationToURIPath(void);
	bool				matchLocationToURIPath(std::string& str);
	void				updateTargetPathInfo(std::string& leftover);
	void				updateErrorPagePath(int errnum);
	void				updateRe(void);

	std::vector<field_t* >		findRequestHeaderValue(const std::string& header_name);

	RequestParser*		_parser;
	RequestValidator*	_validator;

	listenInstance_s*	_listen_instance;
	const CLocation*	_matched_location;
	CServer*			_matched_server;
	
	std::deque<char>	_buffer;
	token_s				_unparsed;

	std::string 		_method, _uri_str, _version, _req_line, _host_header;
	uri_t				_uri;
	fieldMap_t 			_headers;
	fieldMap_t 			_trailers;

	std::string			_content_length;
	std::string 		_body;
	std::string			_body_filename;
	fileInfo_s			_upload;
	std::string 		_boundary;
	std::string 		_default_mp_content_type;
	mpMessage_t			_mp_m;
	mpState*			_mp_state;

	unsigned int		_errNum;
	std::string			_errMess;

	eParse				_p_state;
	eReqState			_r_state;
	
	parsedSizes_s		_sizes;
	
	bool 				_hasBody, _isCGI, _isChunked, _isMultiPart, _bodyUTF8, _isRedirect, _bodyInFile, _body_complete;
	
	respond_t			_re;
	eBodyType			_body_type;
	std::string 		_leftover;
	
	
	private : 
};

std::ostream& l14(std::ostream& os);
std::ostream& r3(std::ostream& os);
std::ostream& operator<<(std::ostream& os, Request& request);