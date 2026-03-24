/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestValidator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junruh <junruh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:26:19 by kmooney           #+#    #+#             */
/*   Updated: 2026/03/24 13:45:08 by junruh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "RequestValidator.hpp"
#include "RequestTypes.hpp"
#include "RequestParser.hpp"

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>				 CONSTRUCTION/DESTRUCTION/COPY						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

RequestValidator::RequestValidator(Request* request) : _req() { 
	_req = request;
}

RequestValidator::~RequestValidator(){}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>				 REQUEST VALIDTION CONTROL  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

void	RequestValidator::validate() {
	eReqState state = _req->getReqStatus();
	
	switch (state) {
		case V_REQLINE	: { validateReqLine() ; break;}
		case V_HEADERS	: { validateHeaders() ; break;}
		case V_BODY		: { validateBody() ; break;}
		case V_TRAILERS	: { validateTrailers() ; break;}
		default 		: { break; } 
	}
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>				 		REQUEST LINE VALIDATION 					   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
*/

void	RequestValidator::validateReqLine(){
	if (!validateMethod()) return;
	if (!validateURI()) return;
	if (!validateVersion()) return;
	_req->setRstate(P_HEADERS);
}

bool	RequestValidator::isValidMethod(const std::string& str){
	if (str.compare("GET") == 0 ||
		str.compare("POST") == 0 ||
		str.compare("PUT") == 0 ||
		str.compare("DELETE") == 0 ||
		str.compare("HEAD") == 0 ||
		str.compare("OPTIONS") == 0
	)
		return true;
	else
		return false;
}

bool	RequestValidator::validateMethod(){

	str_t method = _req->getMethod();

	if (_req->_r_state != V_REQLINE) {
		if (!_req->_matched_location->isMethodSupported(method))
			return _req->setState(P_ERROR, _req->_r_state, ERR_RL_METHOD_NOT_PERMITTED, 501);
		else if (_req->_matched_location->isMethodSupported(to_upper(method)))
			return _req->setState(P_ERROR, V_REQLINE, ERR_RL_METHOD_CASE, 400);
	}
	else if (!isValidMethod(method))
		return _req->setState(P_ERROR, V_REQLINE, ERR_RL_METHOD_NOT_SUPPORTED, 501);
	return true;
}

bool	RequestValidator::validateVersion()
{
	str_t v = _req->_version;

	if ( v == "HTTP/1.1" || v == "HTTP/1.0")
		return true;
	else if (v == "HTTP/0.9" || v == "HTTP/2.0" || v == "HTTP/3.0")
		return _req->setState(P_ERROR, V_REQLINE, ERR_RL_VERSION_NOT_SUPPORTED, 505);
	else
		return _req->setState(P_ERROR, V_REQLINE, ERR_RL_VERSION_NOT_RECOGNISED, 400);
}

/*
>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>			URI VALIDATION 	  	   <<<<<<
>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

bool	RequestValidator::validateURI(){

	if (_req->_uri.uri_type == ABSOLUTE_FORM){
		if (!validateScheme()){ return false; }
		if (!validateUser())  { return false; }
		if (!validatePass())  { return false; }
		if (!validateHost())  { return false; }
		if (!validatePort())  { return false; }
	}
	if (!validatePath()) { return false; }
	if (!validateQuery()){ return false; }
	if (!validateFrag()) { return false; }
	return true;
}

/*  URI SCHEME VALIDATION  */
bool	RequestValidator::validateScheme(){
	str_t s = _req->_uri.scheme;
	
	if (s.empty())
		return _req->setState(P_ERROR, V_REQLINE, ERR_RL_MISSING_SCHEME, 400);
	to_lower_ref(s);
	if (s == "https" || s == "http")
		return true;
	return _req->setState(P_ERROR, V_REQLINE, ERR_RL_UNSUPPORTED_SCHEME, 400);
}

/*  URI USER VALIDATION  */
bool	RequestValidator::validateUser(){
	/* INCOMPLETE */
		/* userinfo = *( unreserved / pct-encoded / sub-delims / ":" ) */
	/* THIS SHOULD PROBABLY RETURN ERROR 400 or 404
	 	USERNAME / PASS SHOULD NOT APPEAR IN THE
	*/
	if (!percentDecode( _req->_uri.user ))
		return _req->setState(P_ERROR, ERR_RL_PERCENT_ENCODING_U, 400);
	if (!isValidUTF8( _req->_uri.user ))
		return _req->setState(P_ERROR, ERR_RL_UTF8_ENCODING_U, 400);
	return true;
}

/*  URI PASS VALIDATION  */
bool	RequestValidator::validatePass(){
	if (!percentDecode( _req->_uri.pass ))
		return _req->setState(P_ERROR, ERR_RL_PERCENT_ENCODING_P, 400);
	if (!isValidUTF8( _req->_uri.user ))
		return _req->setState(P_ERROR, ERR_RL_UTF8_ENCODING_P, 400);
	return true;
}

/*  URI HOST VALIDATION  */
bool	RequestValidator::validateHost() {
	if ( _req->_uri.host.empty())
		return _req->setState(P_ERROR, ERR_RL_HOST_MISSING, 400);
	if (!percentDecode( _req->_uri.host ))
		return _req->setState(P_ERROR, ERR_RL_PERCENT_ENCODING_HOST, 400);
	if (!isValidUTF8( _req->_uri.host ))
		return _req->setState(P_ERROR, ERR_RL_UTF8_ENCODING_HOST, 400);
	/* INCOMPLETE */
/* 	
	1) IP literal in [],  

	2) IPv4 address in dotted decimal form
		IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet

	  dec-octet   = DIGIT                 ; 0-9
				  / %x31-39 DIGIT         ; 10-99
				  / "1" 2DIGIT            ; 100-199
				  / "2" %x30-34 DIGIT     ; 200-249
				  / "25" %x30-35          ; 250-255

	3) a registered name
		https://www.rfc-editor.org/rfc/rfc1034#section-3.5 or https://www.rfc-editor.org/rfc/rfc1123#section-2
		IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
		IPvFuture  = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
	If a URI containing an IP-literal that starts with "v" return ""address mechanism not supported" - i.e. error 501 Not Implemented
	IPv5
	*/
	return true;
}

/*  URI PORT VALIDATION  */

bool	RequestValidator::validatePort(){
	if (!_req->_uri.port.empty())
		{_req->_uri.port_int = str_to_int(_req->_uri.port);}
	if (!(_req->_uri.port_int >= 0 && _req->_uri.port_int <= 65535))
		return _req->setState(P_ERROR, "Malformed Request : Invalid Port Number", 400);
	return true;
}

/*  URI PATH VALIDATION  */

bool	RequestValidator::validatePath() {
/* 
	NGINX LOCATION MATCHING : 
  	Sets configuration depending on a request URI.

	The matching is performed against a normalized URI, after decoding the text encoded in the “%XX” form, 
	resolving references to relative path components “.” and “..”, and possible compression of two or more 
	adjacent slashes into a single slash.

	A location can either be defined by a prefix string, or by a regular expression
*/

	/* 1. 
		if ( _req->__path.str.empty() && _

   If a URI contains an authority component, then the path component
   must either be empty or begin with a slash ("/") character.  If a URI
   does not contain an authority component, then the path cannot begin
   with two slash characters ("//").

*/
	if (_req->_uri.uri_type == ABSOLUTE_FORM && (!(_req->_uri.path.empty() || _req->_uri.path[0] == '/')))
		return _req->setState(P_ERROR, ERR_RL_PATH_SYNTAX, 400);
	_req->_uri.path = normalisePath(_req->_uri.path);
	_req->_re.normalised_path = _req->_uri.path; // temporary (testing respond_t)
	if (!percentDecode( _req->_uri.path ))
		return _req->setState(P_ERROR, ERR_RL_PERCENT_ENCODING_PATH, 400);
	if (!isValidUTF8( _req->_uri.path ))
		return _req->setState(P_ERROR, ERR_RL_UTF8_ENCODING_PATH, 400);
	return true;
}

/*  URI QUERY VALIDATION  */

bool	RequestValidator::validateQuery(){
	if (!percentDecode( _req->_uri.query ))
		return _req->setState(P_ERROR, ERR_RL_PERCENT_ENCODING_QUERY, 400);
	if (!isValidUTF8( _req->_uri.query ))
		return _req->setState(P_ERROR, ERR_RL_UTF8_ENCODING_QUERY, 400);
	return true;
}

/*  URI FRAGMENT VALIDATION  */

bool	RequestValidator::validateFrag(){
	if (!percentDecode( _req->_uri.frag ))
		return _req->setState(P_ERROR, ERR_RL_PERCENT_ENCODING_FRAG, 400);
	if (!isValidUTF8( _req->_uri.frag ))
		return _req->setState(P_ERROR, ERR_RL_UTF8_ENCODING_FRAG, 400);
	return true;
}

/*
>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		URI VALIDATION UTILS  	   <<<<<<
>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

bool	RequestValidator::percentDecode( str_t& encoded){

	/* URI 7.3
	Note, however, that the "%00" percent-encoding
   (NUL) may require special handling and should be rejected if the
   application is not expecting to receive raw data within a component. */
	 str_t	decoded;
	 size_t	len = encoded.length();
	 size_t	i = 0;
	 
	 decoded.reserve(len);
	 while ( i < len ){
		 if (encoded[i] == '%'){
			 if ((i + 2) < len && (isxdigit(encoded[i + 1]) && isxdigit(encoded[i + 2]))) {
		 
				 unsigned char converted = hexCharToUnsignedChar(encoded[i + 1]);
				 converted = converted << 4;
				 converted += hexCharToUnsignedChar(encoded[i + 2]);		
				 decoded += converted;
				 i += 3;
			 }
			 else
			 	return false;
		 }
		 else
			 decoded += encoded[i++];
	 }
	 encoded = decoded;
	 return true;
 }

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>				 		HEADER VALIDATION  					  		   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

void	RequestValidator::validateHeaders( void ){
	if (!validateHostHeader())
		return;
	if (!validateRepresentation())
		return; 
	//headerDefinitions();
//	validateAllHeaders(_req->_headers); // USE IF STATEMENT TO RETURN IF NOT TRUE
	// FURTHER VALIDATION AND SYNTAX CHECKS
	if (!_req->matchServerToHostHeader()){
		_req->setState(P_ERROR, V_HEADERS, ERR_NO_MATCHING_SERVER, 404);
		return;
	}
	_req->matchLocationToURIPath();
	if (_req->_hasBody)
		_req->setRstate(P_BODY);
	else {
		_req->setRstate(REQ_DONE);
		_req->setPstate(P_DONE);
	}
}

/*
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		VALIDATE HOST HEADER		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
*/

bool	RequestValidator::validateHostHeader() {
	/* 
		Host header rules 
		=================
		1.1 : Must be present
			: Must not be empty
			: Exactly one host header
			:
			: Syntax : 
			:	Allowed Form : Host: example.com
			:	Allowed Form : Host: example.com:8080
			:	Allowed Form : Host: sub.domain.org
			:	Allowed Form : Host: 127.0.0.1
			:	Allowed Form : Host: 127.0.0.1:8000
			:	Allowed Form : Host: [2001:db8::1]
			:	Allowed Form : Host: [2001:db8::1]:443
			:
			:	Invalid Form : Host: http://example.com
			:	Invalid Form : Host: example .com
			:	Invalid Form : Host: exa$mple.com
			:	Invalid Form : Host: :80
			: 
			: No whitespace or control characters
			: Port must be valid if present (i.e. 0-65535)
			: IPv6 must use brackets

		1.0 : Voluntary
			: Not present : serve default_server
			: If exists, and invalid : return 400 Bad request

	*/
	/* 	When an origin server receives a request with an absolute-form of request-target,
		the origin server MUST ignore the received Host header field (if any) and instead 
		use the host information of the request-target. Note that if the request-target does
	 	not have an authority component, an empty Host header field will be sent in this case.
	  	A server MUST accept the absolute-form in requests even though most HTTP/1.1 clients 
	  	will only send the absolute-form to a proxy.
	*/

	fieldMap_t::const_iterator it = _req->_headers.find("host");

	if (it != _req->_headers.end()){					
		int count = it->second.size();
		if (_req->_uri.uri_type == ABSOLUTE_FORM && count == 1)
			return (validateAbsoluteForm(it));
		if ( count == 0)
			return _req->setState(P_ERROR, V_HEADERS, "Missing 'Host' header value", 400);
		else if ( count > 1)
			return _req->setState(P_ERROR, V_HEADERS, "Multiple 'Host' header values", 400);
		else
			_req->setHostname(it->second[0]->val); // need to check syntax (valid port if present, doesn't start with port etc)
	}
	else {
		if (_req->_version.compare("HTTP/1.1") == 0)
			return _req->setState(P_ERROR, V_HEADERS, "Missing 'Host' header", 400);
		else if (_req->_version.compare("HTTP/1.0") == 0)
			Log::instance().log(Log::INFO, Log::TYPE_REQUEST_PARSING,
				"Request Parsing : Missing host header : using default server", true);
	}
	return true;
}

bool	RequestValidator::validateAbsoluteForm(fieldMap_t::const_iterator& it){
		
	if (_req->_uri.auth.compare(it->second[0]->val) == 0){
		_req->setHostname(_req->_uri.auth); // need to check syntax (valid port if present, doesn't start with port etc)
		return true;
	}
	else
		return _req->setState(P_ERROR, V_HEADERS, "'Host' header value mismatch (Absolute Form)", 400);
}

/*
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		VALIDATE REPRESENTATION		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
*/

bool	RequestValidator::validateRepresentation(){

	if (!identifyLengthHeader())
		return false;
	if (!validateContentEncoding())
			return false;
	if (!validateContentType())
		return false;
	return true;
}

bool	RequestValidator::validateContentType(){ // CHECK SERVER SUPPORTS TYPE

	int count = countHeaderValues(_req->_headers, "content-type");

	// NEED TO DECIDE WHETHER OR NOT TO ASSUME A DEFAULT CONTENT TYPE OR TREAT AS ERROR ( IT IS ACCEPTABLE/SAFER? TO TREAT AS AN ERROR)
	// if (count == 0 && _req->_method == "POST" && (_req->_hasBody)) // - ASK MINA FOR OPINION
	// 	return _req->setState(P_ERROR, V_HEADERS, "Missing 'Content-Type' header (POST)", 400);
	// else if (count == 0)

	if (count == 0)
		return true;
	else if ( count > 1)
		return _req->setState(P_ERROR, V_HEADERS, "Multiple 'Content-Type' received", 400);
	
	fieldMap_t::iterator										header 			= _req->_headers.find("content-type");
	const std::string& 											val 			= header->second[0]->val;
	const std::vector< std::pair<std::string, std::string> >&	options 		= header->second[0]->params;
	//int 														options_size 	= options.size();

	//const std::vector<std::string>  validTypes = config.getValidTypes(route); // PSEUDO SERVER CHECK CODE - TO REPLACE TEMP
	if (!temporaryIsValidContentType(val))
		return _req->setState(P_ERROR, V_HEADERS, "Content-Type not supported", 415);

	if (val.find("multipart/") != std::string::npos)
		return validateMultiPartHeader(val, options);
	if (val.find("charset") != std::string::npos)
		return validateCharset(options);
	return true;
}

bool	temporaryIsValidContentType(const std::string& name){ // THESE ARE TEST VALUES TO BE REPLACED WITH SERVER CHECK

	static std::vector<std::string> validTypes;

	if (validTypes.empty()){
		validTypes.push_back("application/javascript");
		validTypes.push_back("application/json");
		validTypes.push_back("application/octet-stream");
		validTypes.push_back("application/xml");
		validTypes.push_back("multipart/form-data");
		validTypes.push_back("image/gif");
		validTypes.push_back("image/jpeg");
		validTypes.push_back("image/png");
		validTypes.push_back("image/svg+xml");
		validTypes.push_back("image/webp");
		validTypes.push_back("image/x-icon");
		validTypes.push_back("text/css");
		validTypes.push_back("text/html");
		validTypes.push_back("text/plain");
		validTypes.push_back("font/woff");
		validTypes.push_back("font/woff2");
	}
	std::vector<str_t>::iterator it = find(validTypes.begin(), validTypes.end(), name);
	if (it == validTypes.end())
		return false;
	return true;
}


bool	RequestValidator::validateMultiPartHeader(const std::string& val, const std::vector< std::pair<std::string, std::string> >& options){
	size_t		size = options.size();
	std::string	boundary; //MUST NOT BE MORE THAN 70 CHARS?
	std::string	charset;
	bool 		has_boundary = 0;
	bool 		has_charset = 0;

	if (val != "multipart/form-data")
		return _req->setState(P_ERROR, V_HEADERS, ERR_MP_TYPE_NOT_SUPPORTED, 415);

	if (size != 0) {

		for (size_t i = 0; i < size; i++){
			const std::string& option_key = options[i].first;
			const std::string& option_val = options[i].second;
			
			if (option_key == "boundary"){
				if (option_val.length() > 70)
					return _req->setState(P_ERROR, V_HEADERS, ERR_MP_BOUNDARY_LENGTH, 415); 
				boundary = option_val;
				has_boundary = true;
			}
			else if (option_key == "charset"){
				charset = option_val;
				if (!temporaryIsValidContentType(charset))
					return _req->setState(P_ERROR, V_HEADERS, ERR_MP_BOUNDARY_LENGTH, 415);
				has_charset = true;
			}
		}
	}
	if (!has_boundary)
		return _req->setState(P_ERROR, V_HEADERS, ERR_MP_BOUNDARY_MISSING, 400);
	_req->setBoundaryString("--" + boundary);
	if (has_charset)
		_req->setDefaultMultiPartType(charset);
	_req->_isMultiPart = true;
	return true;
}

/*
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		VALIDATE MULTIPART PART		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
*/

/*
	MULTIPART HEADERS 
	=================

	form-data
	=========

		Content-Disposition
		===================
		Each part MUST contain a Content-Disposition header field
		The Content-Disposition header field MUST also contain an additional parameter of "name"
			(possibly encoded)
		The file name isn't mandatory for cases where the file name isn't available or is meaningless or
   		private (e.g. file drag and dropped)

		Filenames can be percent-encoded
		Each form-data field (or part) contains a 'name' parameter

		Content-Type
		============
		Each part MAY have an (optional) "Content-Type" header field, which
   		defaults to "text/plain".  If the contents of a file are to be sent,
   		the file data SHOULD be labeled with an appropriate media type, if
   		known, or "application/octet-stream".

		Character Set
		=============
		If there is a Content-Disposition field with name: "__charset__", then the body of that part
		should contain the default charset for the form-data

		Form parts with identical field names MUST NOT be coalesced
*/
bool	RequestValidator::validateMPheaders(){

	mpPart_t&	part = _req->_parser->currentPart();

	const std::vector<field_t*>* cont_disp = getHeader(part.headers, "content-disposition");
	if (cont_disp && !validateMPContentDisposition(part, *cont_disp))
		return false;

	const std::vector<field_t*>* cont_type = getHeader(part.headers, "content-type");
	if (cont_type && (!validateMPContentType(part, *cont_type)))
		return false;

	return true;
}

/* 
	CONTENT DISPOSITION : 
		HEADER FORMAT CHECKS : MANDATORY FIELD, SINGLE VALUE, SINGLE INSTANCE (SHOULD BE FOUND DURING PARSING)
*/
bool	RequestValidator::validateMPContentDisposition(mpPart_t& part, const std::vector<field_t*>& header){

	if (header.size() == 0)
		return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_MISSING, 400); // CODE CHECK REQUIRED
	if (header.size() > 1)
		return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_MULTI_VALS, 400); // CODE CHECK REQUIRED

	if (header[0]->val != "form-data")
		_req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_UNEXPECTED_VALUE, 400); // CODE CHECK REQUIRED
	
	part.disposition_type = header[0]->val;

	if (duplicateOptionCheck(header[0]->params))
		_req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_DUPLICATE_PARAMS, 400); // CODE CHECK REQUIRED

	if (!updateMPparams(part, header[0]->params))
		return false;

	if (part.name.length() == 0)
		return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_NO_NAME, 400); // CODE CHECK REQUIRED
	else if (part.name == "file" && part.filename.length() == 0)
		return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_FILENAME_MISSING, 400); // CODE CHECK REQUIRED

	// filename character check?
	// filename accessibility check?
	// filename suffix (type) check?
	return true;
}

bool	RequestValidator::updateMPparams(mpPart_t& part, const std::vector<std::pair< str_t, str_t> >& params){

	for (size_t i = 0; i < params.size(); i++) {
		if (params[i].first == "name")
			part.name = params[i].second;
		else if (params[i].first == "filename")
			part.filename = params[i].second;
		part.disposition_parameters.push_back(params[i]);
	}
	if (part.name == "file")
		part.isFile = true;
	return true;
}

bool	RequestValidator::validateMPContentType(mpPart_t& part, const std::vector<field_t*>& header){

	for (size_t i = 0; i < header.size(); i++){
		if (!temporaryIsValidContentType(header[i]->val))
			continue;
		if (validateParamsForContentType(header[i]->val, header[i]->params)){
			part.supported_content.push_back(header[i]);
		}
	}
	if (header.size() == 0){
		field_t*  field = new field_t;
		field->val = "application/octet-stream"; //default - might need to change type?
		part.supported_content.push_back(field);
	}
	else if (part.supported_content.size() == 0)
		return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_TYPE_NOT_SUPPORTED, 415); // CODE CHECK REQUIRED
	return true;
}

bool	RequestValidator::validateParamsForContentType(const std::string& type, const std::vector<std::pair<str_t, str_t> > params){
	
	/* This should take pre-populated context for a header type and value, 
		and provide a map?? of value to acceptable parameter type/syntax/rules
		that can then be used to check validity of params
	
		//getContentTypeContext(type);
	*/
	for (size_t i = 0; i < params.size(); i++){ // content of this loop is currrently meaningless
		if (type =="name")
			break;
		continue;
	}
	return true;
}

bool	RequestValidator::validateMultiPartBody(const mpMessage_t& multipart){
	
	// file signatures (magic bytes) - body check
	// size limits - parser check?
	// allowed types - map file type to content type?

	std::vector<mpPart_t>::const_iterator it = multipart.parts.begin();

	/* 
		Multipart body rules :
		
			Headers : 	content-disposition
						: 	mandatory
						:	name="file" then filename must be provided
						:	filename : needs to be sanitised : normalise and dot removal - no absolute paths?
										  	
			Headers : 	content-type 
						:	optional
						:	if none provided, revert to default (application/octet-stream)?		
						:		if content-type must be valid MIME type and matches expectations (e.g. file upload)
			
			Body 	:	length	
						:	Can be empty (but must be delimited properly)
						:	Check max body size (whilst parsing)?

			Body 	:	filetype 
						:	inspect file signature (magic bytes)


			Field name 		:	Matches expected schema
							:	No injection patterns
							:	Length limits
								
			Size limit 		:	Total request size
							:	Per-part size
							:	Number of parts

			Encoding   		: 	Proper handling of binary data
							:  	No unintended UTF-8 decoding
							:  	No truncation
								
			Duplicate fields:

								Expected fields	:	Required fields present: e.g. username, file
								
								Field Format	: 	Strings: length, charset
												:	Files: type, extension, size

								Server rules	:	only one file allowed
												:	size (e.g. max 5MB)
												:	allowed MIME types

	*/
	for (;it != multipart.parts.end(); it++){
		if (it->name.length() == 0)
			_req->setState(P_ERROR, P_BODY, "Malformed Multipart: Invalid multipart header: expected 'name' parameter", 400); // CODE CHECK REQUIRED
		else if (it->name == "file"){
			if (it->isFile == 0 || it->filename.length() == 0)
				_req->setState(P_ERROR, P_BODY, "Malformed Multipart: Invalid multipart header: expected 'filename' parameter", 400); // CODE CHECK REQUIRED
			// check filename is of correct format
		} 
		if (it->disposition_type != "form-data")
			_req->setState(P_ERROR, P_BODY, "Malformed Multipart: Invalid multipart header: multipart type unsupported", 400); // CODE CHECK REQUIRED
		
		// 
	}
	return true;
}

bool	RequestValidator::validateCharset(const std::vector< std::pair<std::string, std::string> >& options){
	
	const std::string& option_key = options[0].first;
	const std::string& option_val = options[0].second;

	if (option_key.compare("charset") == 0 && !option_val.empty()){
		if (!temporaryIsValidContentType(option_val))
			return _req->setState(P_ERROR, V_HEADERS, "Content-Type not supported", 415);
		else
			_req->setBodyEncoding(option_val);
	}
	return true;
}

bool	RequestValidator::duplicateOptionCheck(const std::vector< std::pair<std::string, std::string> >& options){
	
	size_t		size = options.size();  // ONLY COMPARING FIRST AT MOMENT - NEED TO HANDLE VALUE AND HAVE LOGIC FOR DIFFERENCES/DUPLICATES
	
	std::set<std::string> seen;

	for (size_t i = 0; i < size; i++) {
		if (seen.find(options[i].first) != seen.end())
			return true;
		seen.insert(options[i].first);
	}
	return false;
}

// bool	RequestValidator::duplicateValueCheck(const std::vector<field_t*>& values){
// 	return true;
// }

bool	RequestValidator::identifyLengthHeader(){
	
	if (_req->_headers.find("content-length") != _req->_headers.end()){
		if (_req->_headers.find("transfer-encoding") != _req->_headers.end())
			return _req->setState(P_ERROR, "Malformed Request : Headers : Transfer-encoding and Content-Length", 400);
		else if (!validateContentLength())
			return _req->setState(P_ERROR, "Malformed Request : Headers : Invalid Content-Length", 400);
		else
			return true;
	}
	else if (_req->_headers.find("transfer-encoding") != _req->_headers.end())
		return validateTransferEncoding();
	else if (_req->_method == "POST" || _req->_method == "PUT")
		return _req->setState(P_ERROR, "Malformed Request : Headers : content-length or transfer-encoding required for request type", 400);
	_req->_content_length = 0;
	_req->setBodyType(NO_BODY, NOT_CHUNKED);
	return true;
}

bool	RequestValidator::validateContentLength() {

	int	count = countHeaderValues(_req->_headers, "content-length");
	
	if (count == 1 && !convertContentLength())
			return _req->setState(P_ERROR, V_HEADERS, "Invalid 'Content-Length' value", 400);
	else if (count == 0)
		return _req->setState(P_ERROR, V_HEADERS, "Invalid 'Content-Length' header : no value", 400);
	else if (count > 1)
		return _req->setState(P_ERROR, V_HEADERS, "Multiple 'Content-Length' headers", 400);
		
	if (_req->_content_length == 0)
		_req->setBodyType(NO_BODY, NOT_CHUNKED);
	else
		_req->setBodyType(HAS_BODY, NOT_CHUNKED);
	return true;
}

bool	RequestValidator::convertContentLength( void ){

	const std::string&	val = _req->_headers.find("content-length")->second[0]->val;
	char*				endptr = NULL;

	_req->_content_length = strtoull(val.c_str(), &endptr, 10);
	if (*endptr != '\0')
		return false;
	return true;
}

/*   

	All transfer-coding names are case-insensitive and ought to be registered within the HTTP Transfer Coding registry
	
	The "TE" header field describes capabilities of the client with regard to transfer codings and trailer sections.
	TE is also used within HTTP/1.1 to advise servers about which transfer codings the client is able to accept in a response
	
	TE                 = #t-codings
  	t-codings          = "trailers" / ( transfer-coding [ weight ] )
  	transfer-coding    = token *( OWS ";" OWS transfer-parameter )
  	transfer-parameter = token BWS "=" BWS ( token / quoted-string )

	A sender of TE MUST also send a "TE" connection option within the Connection header field (Section 7.6.1) to inform intermediaries not to forward this field.

 */
bool	RequestValidator::validateTransferEncoding() {

	int count = countHeaderValues(_req->_headers, "transfer-encoding");

	if (count == 0)
		return _req->setState(P_ERROR, V_HEADERS, "Invalid 'Transfer-Encoding' header : no value", 400);
	else if (count > 1)
		return _req->setState(P_ERROR, V_HEADERS, "Multiple 'Transfer-Encoding' values", 501);

	const std::string& val = _req->_headers.find("transfer-encoding")->second[0]->val;
	

	if (to_lower(val).compare("chunked") != 0)
		return _req->setState(P_ERROR, V_HEADERS, "Unsupported Transfer-Encoding: " + val, 501);

	if (_req->_version.compare("HTTP/1.0") == 0)
		_req->setState(P_ERROR, V_HEADERS, "Transfer-Encoding is forbidden in HTTP/1.0 requests", 400);

	_req->setBodyType(true, true);
	return true;
}

bool	RequestValidator::validateContentEncoding() { 
	if (_req->_headers.count("content-encoding") == 0)
		return true;
	return _req->setState(P_ERROR, V_HEADERS, "Unsupported Media Type (Content-Encoding)", 415);
}

int	RequestValidator::countHeaderValues(const fieldMap_t& headers, const std::string& headerName) {
	
	fieldMap_t::const_iterator it = headers.find(headerName);
	
	if (it != headers.end()) 
		return it->second.size();
	return 0;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		GET VECTOR OF VALUE:PARAM FOR THE GIVEN HEADER NAME		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

const std::vector<field_t*>*	RequestValidator::getHeader(const fieldMap_t& headers, const str_t& name){

	fieldMap_t::const_iterator it = headers.find(name);
	
	if (it != headers.end())
		return &it->second;
	return NULL;
	
}

const std::vector<field_t*>*	RequestValidator::getHeader(const fieldMap_t* headers, const str_t& name){
	
	fieldMap_t::const_iterator it = headers->find(name);
	
	if (it != headers->end())
		return &it->second; 
	
	return NULL;
}
/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>				 		BODY VALIDATION 					  		   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

void	RequestValidator::validateBody(){
	if (_req->_r_state != P_TRAILERS)
		_req->setRstate(REQ_DONE);
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>				 		TRAILERS VALIDATION 				  		   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

void	RequestValidator::validateTrailers(){
		_req->setRstate(REQ_DONE);
		_req->setState(P_DONE, "Parsing Complete", 200);
}


// const HeaderFlags*	RequestValidator::findHeaderFlags(const std::string& name){
	
// 	HeaderFlags* headerFlags;	
// 	return headerFlags;
// }


// bool	RequestValidator::validateAllHeaders(const headerMap_t& headers) {

// 	for (fieldMap_t::const_iterator it = headers.begin(); it != headers.end(); it++) {
		
// 		std::string	name = it->first;
// 		const std::vector<field_t*>& values = it->second;

// 		const HeaderFlags* meta = findHeaderFlags(name);
// 		if (!meta)
// 			continue;

// 		if (!validate_header(name, values, meta)) {
// 			if (meta->flags & HDR_CRITICAL_ERROR)
// 				return false;
// 		}
// 	}
// 	return true;
// }

// /* VALIDATE USING HEADER FLAGS */ 
// bool	RequestValidator::validate_header(const std::string& name, const std::vector<field_t*>& vecValues, const HeaderFlags* meta) {

// 	if ((meta->flags & HDR_ONE_INST_ONLY) && vecValues.size() > 1)
// 		return false;

// 	for (size_t i = 0; i < vecValues.size(); i++) {
// 		const field_t* valField = vecValues[i];

// 		if (meta->flags & HDR_ONE_VAL_ONLY){
// 			for(size_t j = i + 1; j < vecValues.size(); j++){
// 				if (vecValues[j]->val == valField->val)
// 					return false;
// 			}
// 		}

// 		if (meta->flags & HDR_VALUE_CHECK) { 
// 			if (name == "content-length") {
// 				for (size_t j = 0; j < valField->val.size(); j++)
// 		  			 if (!isdigit( valField->val[j] ))
// 			   			return false;
// 			}
// 		}

// 		if ((meta->flags & HDR_OPTION_CHECK)) {
// 			for (size_t p = 0; p < valField->params.size(); p++) {
// 				const std::string& pname = valField->params[p].first;
// 				const std::string& pval = valField->params[p].second;
// 				if (pname.empty())
// 					return false;
// 				if (pval.empty())
// 					return false;
// 			}
// 		}
// 		// ADD GENERAL CHECKS = e.g. SYNTAX/ENCODING 
// 	}

// 	return true;
// }
		
// void	RequestValidator::headerDefinitions(){

// 	/* NEED TO EVALUATE THESE PROPERLY - THIS IS A PLACEHOLDER*/
// 	static const HeaderFlags header_flags[] = {
// 			{ "accept",				HDR_CAN_IGNORE },
// 			{ "accept-encoding",	HDR_CAN_IGNORE },
// 			{ "accept-language",	HDR_CAN_IGNORE },
// 			{ "connection",			HDR_ONE_INST_ONLY | HDR_ONE_VAL_ONLY },
// 			{ "content-length",		HDR_ONE_INST_ONLY | HDR_VALUE_CHECK | HDR_CRITICAL_ERROR },
// 			{ "content-type",		HDR_ONE_INST_ONLY | HDR_ONE_VAL_ONLY | HDR_CAN_IGNORE },
// 			{ "date",				HDR_ONE_INST_ONLY | HDR_CRITICAL_ERROR },
// 			{ "expect",				HDR_ONE_INST_ONLY | HDR_CRITICAL_ERROR },
// 			{ "host",				HDR_MANDATORY | HDR_ONE_VAL_ONLY | HDR_CRITICAL_ERROR},
// 			{ "location",			HDR_ONE_INST_ONLY | HDR_VALUE_CHECK | HDR_CRITICAL_ERROR },
// 			{ "transfer-encoding",	HDR_ONE_INST_ONLY | HDR_VALUE_CHECK | HDR_CRITICAL_ERROR },
// 			{ "user-agent",			HDR_CAN_IGNORE },
// 			{ 0, 0 }
// 		};
// 		_hdr_flags = header_flags;
// }