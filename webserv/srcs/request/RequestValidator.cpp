/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestValidator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:26:19 by kmooney           #+#    #+#             */
/*   Updated: 2026/04/14 08:58:33 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "RequestValidator.hpp"
#include "RequestTypes.hpp"
#include "RequestParser.hpp"

/*
>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		CONSTRUCTION/DESTRUCTION/COPY		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

RequestValidator::RequestValidator(Request* request) : _req() {
	_req = request;
}

RequestValidator::~RequestValidator(){}

/*
>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		REQUEST VALIDATION CONTROL		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<
*/

void	RequestValidator::validate() {
	eReqState state = _req->getReqStatus();

	switch (state) {
		case V_REQLINE	: { validateReqLine() ; break;}
		case V_HEADERS	: { validateHeaders() ; break;}
		case V_BODY		: { validateBody() ; break;}
		case V_MPHEADERS: { validateMPheaders() ; break;}
		case V_MPBODY	: { validateMultiPartBody() ; break;}
		case V_TRAILERS	: { validateTrailers() ; break;}
		default 		: { break; }
	}
}

/*
>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		REQUEST LINE VALIDATION		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
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
	_req->_re.uri_path = _req->_uri.path; // temporary (testing respond_t)
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
	if (!validateConnectionHeader())
		return;
	//headerDefinitions();
	if (!validateAllHeaders(_req->_headers)) // USE IF STATEMENT TO RETURN IF NOT TRUE
		return;
	if (!_req->matchServerToHostHeader()){
		_req->setState(P_ERROR, V_HEADERS, ERR_NO_MATCHING_SERVER, 404);
		return;
	}
	if (!_req->matchLocationToURIPath()){
		_req->setState(P_ERROR, V_HEADERS, ERR_NO_MATCHING_LOCATION, 404);
		return;
	}
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
		else{
			_req->setHostname(it->second[0]->val); // need to check syntax (valid port if present, doesn't start with port etc)
			_req->_host_header = it->second[0]->val;
		}
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

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		VALIDATE CONNECTION HEADER		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
*/

bool	RequestValidator::validateConnectionHeader() {

	bool default_val = false; // close (changed to true for HTTP 1.1)

	if (_req->_version == "HTTP/1.1")
		default_val = true;

	fieldMap_t::const_iterator it = _req->_headers.find("connection");

	if (it == _req->_headers.end())
		_req->_re.keep_alive = default_val;
	else {
		// Need to consider parameters or encodings? Is multiple values automatic rejection?
		int count = it->second.size();

		if (count == 1){
			if (it->second[0]->val == "keep-alive")
				_req->_re.keep_alive = true;
			else
				_req->_re.keep_alive = false;
			return true;
		}
		else if ( count > 1)
			return _req->setState(P_ERROR, V_HEADERS, "Multiple 'Connection' header values", 400);
		else
			return false;
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

	std::map<std::string, std::vector<std::string> > server_content_types = _req->_matched_server->getMime();
	if (!isValidContentType(val))
		return _req->setState(P_ERROR, V_HEADERS, "Content-Type not supported", 415);
	if (val.find("multipart/") != std::string::npos)
		return validateMultiPartHeader(val, options);
	if (val.find("charset") != std::string::npos)
		return validateCharset(options);
	return true;
}

bool	isValidContentType(const std::string& name){ // THESE ARE TEST VALUES TO BE REPLACED WITH SERVER CHECK

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
	return (find(validTypes.begin(), validTypes.end(), name) != validTypes.end());
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
				if (!isValidContentType(charset))
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
	Still to consider : Field Format	: 	Strings: length, charset
											Files: type, extension, size

						Field name 		:	Matches expected schema
										:	No injection patterns
										:	Length limits

	form-data
	=========
	Headers : 	content-disposition :	mandatory

				value 	: 	Acceptable values : "form-data" (just now this is all we accept)

				params	:	name 			: mandatory
							filename		: mandatory if "name=file"

							name=file 		: then filename parameter is mandatory
							name=__charset__: then the body of that part should contain the default charset for the form-data

							filename		: Not required when the file name isn't available/meaningless/private (e.g. file drag and dropped))
											: Can be percent-encoded
											: needs to be sanitised : normalise and dot removal - no absolute paths?

	Headers : 	content-type 	: optional (must match expectation (e.g. if file upload ))

				value		 	: MIME type
				params		 	: ?

				default value   : text/plain (if not a file)
								: application/octet-stream
*/
bool	RequestValidator::validateMPheaders(){

	mpPart_t&	part = _req->_parser->currentPart();

	const std::vector<field_t*>* cont_disp = getHeader(part.headers, "content-disposition");
	if (cont_disp && !validateMPContentDisposition(part, *cont_disp))
		return false;

	const std::vector<field_t*>* cont_type = getHeader(part.headers, "content-type");
	if (cont_type && (!validateMPContentType(part, *cont_type)))
		return false;
	_req->setRstate(P_BODY);
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

	if (!updateMPparams(part, header[0]->params)) // populates name/filename/content-disposition params etc and sanitises filename
		return false;

	if (part.name.length() == 0)
		return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_NO_NAME, 400); // CODE CHECK REQUIRED
	else if (part.name == "file" || part.name == "File"){
		if (part.filename.length() == 0)
			return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_FILENAME_MISSING, 415); // CODE CHECK REQUIRED
		else {
			appendPathToRoot(&part.upload_dest, part.filename, _req->_re.paths.upload_base);
			getFilePermissions(part.upload_dest);
			if (part.upload_dest.not_found == false){
				*_req->_mp_state = MP_ERROR;
				return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_FILENAME_DUP, 400); // CODE CHECK REQUIRED
			}
		}
		part.isFile = true;
		_req->_mp_m.filecount++;
	}
	// filename suffix (type) check? - check magic number is consistent with filename
	return true;
}

bool	RequestValidator::updateMPparams(mpPart_t& part, const std::vector<std::pair< str_t, str_t> >& params){

	int n = 0;
	int fn = 0;
	int fns = 0;

	for (size_t i = 0; i < params.size(); i++) {
		if (params[i].first == "name"){
			n++;
			part.name = params[i].second;
		}
		if (params[i].first == "filename"){
			std::string fname = params[i].second;
			fn++;
			if (sanitiseFilename(fname))
				part.filename = fname;
			else
				return false;
		}
		if (params[i].first == "filename*" ){
			std::string fname = params[i].second;
			fns++;
			if (sanitiseFilenameStar(fname))
				part.filename = fname;
			else
				return false;
		}
		if (n > 1)
			return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_NAME_MULTI, 400); // CODE CHECK REQUIRED
		if (fn > 1)
			return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_FILENAME_MULTI, 400); // CODE CHECK REQUIRED
		if (fns > 1)
			return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_FILENAME_STAR_MULTI, 400); // CODE CHECK REQUIRED
		part.disposition_parameters.push_back(params[i]);
	}
	return true;
}

bool	RequestValidator::validateMPContentType(mpPart_t& part, const std::vector<field_t*>& header){

	for (size_t i = 0; i < header.size(); i++){
		if (!isValidContentType(header[i]->val))
			continue;
		if (validateParamsForContentType(header[i]->val, header[i]->params)){
			part.supported_content.push_back(header[i]);
		}
	}
	if (header.size() == 0){
		field_t*  field = new field_t;
		if (part.isFile)
			field->val = "application/octet-stream"; //default - might need to change type?
		else
			field->val = "text/plain";
		part.supported_content.push_back(field);
	}
	if (part.supported_content.size() == 0)
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

bool	RequestValidator::validateMultiPartBody(){

	// file signatures (magic bytes) - body check
	// size limits - parser check?
	// allowed types - map file type to content type?

	mpPart_t&	part = _req->_parser->currentPart();
	(void)part;
	/* void

			Multipart body rules :

			Body 	:	length
						:	Can be empty (but must be delimited properly)
						:	Check max body size (whilst parsing)?
						:	Check content-length

			Body 	:	filetype
						:	inspect file signature (magic bytes)

			Size limit 		:	Total request size
							:	Per-part size
							:	Number of parts

			Encoding   		: 	Proper handling of binary data
							:  	No unintended UTF-8 decoding
							:  	No truncation

			Server rules	:	only one file allowed
							:	size (e.g. max 5MB)
							:	allowed MIME types

	*/
	mpState state = *_req->_mp_state;
	if (state == MP_COMPLETE){
		_req->setRstate(REQ_DONE);
		_req->setPstate(P_DONE);
	}
	else if (state == MP_FINAL){
		_req->setRstate(P_BODY);
		_req->setPstate(P_AWAIT);
	}
	return true;
}

bool	RequestValidator::validateCharset(const std::vector< std::pair<std::string, std::string> >& options){

	const std::string& option_key = options[0].first;
	const std::string& option_val = options[0].second;

	if (option_key.compare("charset") == 0 && !option_val.empty()){
		if (!isValidContentType(option_val))
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

bool	RequestValidator::sanitiseFilename(std::string& name){

	if (name.empty())
		return false;
	// strip ../ etc)
	// normalise encoding
	// reject control characters
	// replace ' ' with _
	// limit length
	return true;
}

bool	RequestValidator::sanitiseFilenameStar(std::string& name){

	std::vector<std::string> split_name; // [0] = charset, [1] = 'Language : ignored!' [2] = filename

	if (!splitFilename(name, split_name))
		return false;
	if (!percentDecode(split_name[2]))
		return _req->setState(P_ERROR, V_MPHEADERS, "Failed percent-decoding", 415); //CODE?
	if (split_name[0] == "UTF-8"){
		isValidUTF8(split_name[2]);
		name = split_name[2];
	}
	else if (split_name[0] == "ISO-8859-1")
		name = ISO8859_1ToUTF8(split_name[2]);
	else
		return _req->setState(P_ERROR, V_HEADERS, "Unsupported Media Type (MultiPart filename encoding)", 415);
	return true;
}

bool	RequestValidator::splitFilename(std::string& name, std::vector<std::string>& vec){

	std::string::size_type pos = name.find("'");
	std::string::size_type pos2 = name.find("'", pos); //pos + 1?

	if (pos == std::string::npos || pos2 == std::string::npos)
		return false;
	vec.push_back(name.substr(0, pos));
	vec.push_back(name.substr(pos, pos2));
	vec.push_back(name.substr(pos2));
	return true;
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

bool	RequestValidator::validateAllHeaders(const fieldMap_t& headers) {

	for (fieldMap_t::const_iterator it = headers.begin(); it != headers.end(); it++) {

		std::string	name = it->first;
		const std::vector<field_t*>& values = it->second;

		headerContext_s context = getHeaderContext(name);
		if (!validateHeader(values, context))
			return false;
	}
	return true;
}

/*
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		FILE FORMAT VALIDATION		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
*/

bool RequestValidator::validateFileFormat(std::string& path) {

	std::string mime_type = detectMimeFromFilename(path);

	std::map<std::string, std::vector<std::string> > mime_map = _req->_matched_server->getMime(); // Change mimemap to str vec<str>

	size_t pos = path.find_last_of('.');
	if (pos == std::string::npos || mime_type.empty())
		return false;

	std::string file_ext = path.substr(pos);

	std::map<std::string, std::vector<std::string> >::iterator it = mime_map.find(mime_type);

	if (it != mime_map.end()) {

		const std::vector<std::string>& exts = it->second;

		for (size_t i = 0; i < exts.size(); ++i) {
			if (exts[i] == file_ext)
				return true;
		}
	}

	return false;
}
/*
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		HEADERS VALIDATION		<<<<<<
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
*/

bool	RequestValidator::validateHeader(const std::vector<field_t*>& values, const headerContext_s& context){

	size_t instances = values.size();

	if (instances > 1 && !context.is_repeatable)
		return _req->setState(P_ERROR, V_HEADERS, "Header Validation Error : " + context.name + ": multiple instances of singleton header", 400);
	for (size_t i = 0; i < instances; i++){

		std::string& header_value = values[i]->val;
		std::vector<std::pair <std::string, std::string> >& params = values[i]->params;

		switch (context.hdr_name){												// CHECK HEADER VALUE IS APPROPRIATE FORMAT AND VALUE IS VALID
			case (HDR_ACCEPT):{ isValidMediaRange(header_value); break;}
			case (HDR_ACCEPT_CHARSET):{ isValidCharset(header_value); break;}
			case (HDR_ACCEPT_ENCODING):{ isValidCodings(header_value); break;}
			//case (HDR_ACCEPT_LANGUAGE):{ isValidLanguage(header_value); break;}
			case (HDR_AUTHORIZATION):{ break;}
			case (HDR_CONNECTION):{ break;}
			case (HDR_CACHE_CONTROL):{ break;}
			case (HDR_CONTENT_ENCODING):{ break;}
			case (HDR_CONTENT_LANGUAGE):{ break;}
			case (HDR_CONTENT_LENGTH):{ break;}
			case (HDR_CONTENT_TYPE):{ isValidMediaType(header_value, false); break;} // Doesn't check type or subtype in IANA registry
			case (HDR_COOKIE):{ break;}
			case (HDR_DATE):{ break;}
			case (HDR_EXPECT):{ break;}
			case (HDR_HOST):{ break;}
			case (HDR_IF_MATCH):{ break;}
			case (HDR_IF_MODIFIED_SINCE):{ break;}
			case (HDR_IF_NONE_MATCH):{ break;}
			case (HDR_IF_RANGE):{ break;}
			case (HDR_IF_UNMODIFIED_SINCE):{ break;}
			case (HDR_MAX_FORWARDS):{ break;}
			case (HDR_ORIGIN):{ break;}
			case (HDR_PRAGMA):{ break;}
			case (HDR_RANGE):{ break;}
			case (HDR_REFERER):{ break;}
			case (HDR_SET_COOKIE):{ break;}
			case (HDR_TRAILER):{ break;}
			case (HDR_TRANSFER_ENCODING):{ break;}
			case (HDR_TE):{ break;}
			case (HDR_UPGRADE):{ break;}
			case (HDR_UPGRADE_INSECURE):{ break;}
			case (HDR_USER_AGENT):{ break;}
			default :{ break;}
		}

		size_t params_size = params.size();

		if (!context.has_parameters && params.size() > 0)
			return _req->setState(P_ERROR, V_HEADERS, "Header Validation Error : " + context.name + ": unexpected parameters in header field", 400);

		if (params_size > 0){
			if (!context.has_parameters)
				return _req->setState(P_ERROR, V_HEADERS, "Header Validation Error : " + context.name + ": unexpected parameters (q-val) in header field", 400);
			for (size_t i = 0; i < params_size; i++){
				if (params[i].first == "q" && !context.supports_q_val)
					return _req->setState(P_ERROR, V_HEADERS, "Header Validation Error : " + context.name + ": unexpected parameters (q-val) in header field", 400);
			}

		}	// check parameters are of appropriate format/type for header value
	}

/*
	if (context.is_list);
	bool			is_mergeable;
	valueType		type;
	bool			supports_q_val;
	bool			is_commentable;
*/
	return true;
}

/* >>>>>>		CREATE/GET HEADER CONTEXT		<<<<<< */
headerContext_s	RequestValidator::getHeaderContext(const std::string& header_name){

	static std::map<std::string, headerContext_s> header_context;

	if (header_context.empty()){
		// name, is_list, is_repeatable, is_mergeable, val_type, has_parameters, q_val, is_commentable
		header_context.insert(std::pair<std::string, headerContext_s>("accept", headerContext_s("accept", HDR_ACCEPT, true, true, true, VT_MEDIA_RANGE, true, true, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("accept-charset", headerContext_s("accept-charset", HDR_ACCEPT_CHARSET, true, true, true, VT_TOKEN, true, true, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("accept-encoding", headerContext_s("accept-encoding", HDR_ACCEPT_ENCODING, true, true, true, VT_TOKEN, true, true, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("accept-language", headerContext_s("accept-language", HDR_ACCEPT_LANGUAGE, true, true, true, VT_LANGUAGE_TAG, true, true, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("authorization", headerContext_s("authorization", HDR_AUTHORIZATION, false, false, false, VT_CREDENTIALS, true, false, false)));  	// ?"qval":false
		header_context.insert(std::pair<std::string, headerContext_s>("connection", headerContext_s("connection", HDR_CONNECTION, true, true, true, VT_TOKEN,false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("cache-control", headerContext_s("cache-control", HDR_CACHE_CONTROL, true, true, true, VT_DIRECTIVE,true, false, false)));			//"qval":false
		header_context.insert(std::pair<std::string, headerContext_s>("content-encoding", headerContext_s("content-encoding", HDR_CONTENT_ENCODING, true, true, true, VT_TOKEN,false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("content-language", headerContext_s("content-language", HDR_CONTENT_LANGUAGE, true, true, true, VT_LANGUAGE_TAG,false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("content-length", headerContext_s("content-length", HDR_CONTENT_LENGTH, false, false, false, VT_NUMBER,false, false, false)));			//"notes":"Must be single"
		header_context.insert(std::pair<std::string, headerContext_s>("content-type", headerContext_s("content-type", HDR_CONTENT_TYPE, false, false, false, VT_MEDIA_TYPE,true, false, false)));			//"qval":false
		header_context.insert(std::pair<std::string, headerContext_s>("cookie", headerContext_s("cookie", HDR_COOKIE, false, false, false, VT_COOKIE_PAIR,true, false, false)));						//"notes":"Semicolon-separated, NOT comma list"
		header_context.insert(std::pair<std::string, headerContext_s>("date", headerContext_s("date", HDR_DATE, false, false, false, VT_DATE,false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("expect", headerContext_s("expect", HDR_EXPECT, true, true, true, VT_TOKEN,true, false, false)));								// ?"qval":false
		header_context.insert(std::pair<std::string, headerContext_s>("host", headerContext_s("host", HDR_HOST, false, false, false,VT_AUTHORITY, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("if-match", headerContext_s("if-match", HDR_IF_MATCH, true, true, true,VT_ETAG, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("if-modified-since", headerContext_s("if-modified-since", HDR_IF_MODIFIED_SINCE, false, false, false,VT_DATE, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("if-none-match", headerContext_s("if-none-match", HDR_IF_NONE_MATCH, true, true, true,VT_ETAG, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("if-range", headerContext_s("if-range", HDR_IF_RANGE, false, false, false,VT_ETAG_OR_DATE, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("if-unmodified-since", headerContext_s("if-unmodified-since", HDR_IF_UNMODIFIED_SINCE, false, false, false,VT_DATE, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("origin", headerContext_s("origin", HDR_ORIGIN, false, false, false,VT_URI, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("max-forwards", headerContext_s("max-forwards", HDR_MAX_FORWARDS, false, false, false,VT_NUMBER, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("pragma", headerContext_s("pragma", HDR_PRAGMA, true, true, true, VT_DIRECTIVE, true, false, false)));//?"qval":false and "notes":"Legacy"
		header_context.insert(std::pair<std::string, headerContext_s>("range", headerContext_s("range", HDR_RANGE, true, true, true, VT_RANGE, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("referer", headerContext_s("referer", HDR_REFERER, false, false, false, VT_URI, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("set-cookie", headerContext_s("set-cookie", HDR_SET_COOKIE, false, true, false, VT_COOKIE, true, false, false))); //"notes":"MUST NOT merge" and ?"qval":false
		header_context.insert(std::pair<std::string, headerContext_s>("te", headerContext_s("te", HDR_TE, true, true, true, VT_TOKEN, true, false, false))); // ?"qval":false
		header_context.insert(std::pair<std::string, headerContext_s>("trailer", headerContext_s("trailer", HDR_TRAILER, true, true, true, VT_FIELD_NAME, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("transfer-encoding", headerContext_s("transfer-encoding", HDR_TRANSFER_ENCODING, true, true, true, VT_TOKEN, false, false, false)));
		header_context.insert(std::pair<std::string, headerContext_s>("upgrade", headerContext_s("upgrade", HDR_UPGRADE, true, true, true, VT_PROTOCOL, false, false, false))); //,"notes":"Hop-by-hop"
		header_context.insert(std::pair<std::string, headerContext_s>("upgrade-insecure-requests", headerContext_s("upgrade-insecure-requests", HDR_UPGRADE_INSECURE, false, false, false, VT_TOKEN, false, false, false))); //,"notes":"Hop-by-hop" and ?"qval":"false"
		header_context.insert(std::pair<std::string, headerContext_s>("user-agent", headerContext_s("user-agent", HDR_ACCEPT_CHARSET, false, false, false, VT_PRODUCT, true, false, false))); // ?"qval": false
	}
	std::map<std::string, headerContext_s>::const_iterator it = header_context.find(header_name);

	if (it != header_context.end())
		return it->second;
	return headerContext_s("Empty", HDR_EMPTY, true, true, true, VT_TOKEN, false, false, false);
}

/* >>>>>>		VALIDATE CODINGS (accept-encoding)		<<<<<< */
bool	RequestValidator::isValidCodings(std::string& value){
	return (value == "*" || value == "identity" || isValidContentCodingType(value));
}

/* >>>>>>		VALIDATE CONTENT-ENCODING (content-coding and accept-encoding)		<<<<<< */
bool	RequestValidator::isValidContentCodingType(std::string& value){

	static std::vector<std::string> content_codings;

	if (content_codings.empty()){
		content_codings.push_back ("aes128gcm");
		content_codings.push_back ("br");
		content_codings.push_back ("compress");
		content_codings.push_back ("dcb");
		content_codings.push_back ("dcz");
		content_codings.push_back ("deflate");
		content_codings.push_back ("exi");
		content_codings.push_back ("gzip");
		content_codings.push_back ("identity");
		content_codings.push_back ("pack200-gzip");
		content_codings.push_back ("x-compress");
		content_codings.push_back ("x-gzip");
		content_codings.push_back ("zstd");
	}
	to_lower_ref(value);
	return (find(content_codings.begin(), content_codings.end(), value) != content_codings.end());
}

/* >>>>>>		VALIDATE MEDIA RANGE (Accept)		<<<<<< */
bool	RequestValidator::isValidMediaRange(std::string& value){
	return isValidMediaType(value, true);
}

/* >>>>>>		VALIDATE MEDIA TYPE (Content-Type and Accept)		<<<<<< */
bool	RequestValidator::isValidMediaType(std::string& value, bool is_range){

	size_t pos = value.find('/');

	if (pos != std::string::npos) {

		std::string type = value.substr(0, pos);
		std::string sub_type = value.substr(pos + 1);
		
		if (!is_range && (type == "*" || sub_type == "*"))
			return _req->setState(P_ERROR, V_HEADERS, "Header Validation Error : invalid media type (wildcard)", 400);
		if (!(type.length() > 0 && isValidToken(type)))
			return _req->setState(P_ERROR, V_HEADERS, "Header Validation Error : invalid media type", 400);
		if (!(sub_type.length() > 0 && isValidToken(sub_type)))
			return _req->setState(P_ERROR, V_HEADERS, "Header Validation Error : invalid media sub-type", 400);
		return true;
	}
	return _req->setState(P_ERROR, V_HEADERS, "Header Validation Error : invalid media type ", 400);
}

bool	RequestValidator::isValidCharset(std::string& value){
	size_t size = value.length();

	for (size_t i = 0; i < size; i++){
		if (!isTokenChar(value[i]))
			return false;
	}
	return true;
}

bool	RequestValidator::isSupportedCharset(const std::string& value){

	std::vector<str_t> supported_charset = _req->_matched_location->getCharset();
	size_t size = supported_charset.size();

	for (size_t i = 0; i < size; i++){
		if (value == supported_charset[i])
			return true;
	}
	return false;
}

bool	RequestValidator::isValidToken(std::string& value){
	size_t size = value.length();

	for (size_t i = 0; i < size; i++){
		if (!isTokenChar(value[i]))
			return false;
	}
	return true;
}