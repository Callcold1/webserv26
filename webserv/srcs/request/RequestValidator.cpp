/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestValidator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:26:19 by kmooney           #+#    #+#             */
/*   Updated: 2026/04/22 19:22:45 by kmooney          ###   ########.fr       */
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
	return (str.compare("GET") == 0 ||
		str.compare("POST") == 0 ||
		str.compare("PUT") == 0 ||
		str.compare("DELETE") == 0 ||
		str.compare("HEAD") == 0 ||
		str.compare("OPTIONS") == 0
	);
}

bool	RequestValidator::validateMethod(){

	str_t method = _req->getMethod();

	if (_req->_r_state != V_REQLINE) {

		if (!_req->_matched_location->isMethodSupported(method)){
			
			if (_req->_matched_location->isMethodSupported(to_upper(method)))
				return _req->setState(P_ERROR, V_REQLINE, ERR_RL_METHOD_CASE, 400);
			if(!_req->_matched_server->isMethodSupported(method))
				return _req->setState(P_ERROR, _req->_r_state, ERR_RL_METHOD_NOT_PERMITTED, 501);
			else
				return _req->setState(P_ERROR, _req->_r_state, ERR_RL_METHOD_NOT_PERMITTED, 405);
		}
		return true;
	}
	else if (!isValidMethod(method))
				return _req->setState(P_ERROR, V_REQLINE, ERR_RL_METHOD_NOT_SUPPORTED, 505);
	
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
	if (_req->_uri.uri_type == ABSOLUTE_FORM && (!(_req->_uri.path.empty() || _req->_uri.path[0] == '/')))
		return _req->setState(P_ERROR, ERR_RL_PATH_SYNTAX, 400);
	_req->_uri.path = normalisePath(_req->_uri.path);
	_req->_re.uri_path = _req->_uri.path;
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
	if (!_req->matchServerToHostHeader()){
		_req->setState(P_ERROR, V_HEADERS, ERR_NO_MATCHING_SERVER, 404);
		return;
	}
	if (!_req->matchLocationToURIPath()){
		_req->setState(P_ERROR, V_HEADERS, ERR_NO_MATCHING_LOCATION, 404);
		return;
	}
	if (!validateMethod())
		return;
	if (!validateRepresentation())
		return;
	if (!validateConnectionHeader())
		return;

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

	bool default_val = false;

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

	if (count == 0)
		return true;
	else if ( count > 1)
		return _req->setState(P_ERROR, V_HEADERS, "Multiple 'Content-Type' received", 400);

	fieldMap_t::iterator										header 			= _req->_headers.find("content-type");
	const std::string& 											val 			= header->second[0]->val;
	const std::vector< std::pair<std::string, std::string> >&	options 		= header->second[0]->params;
	//int 														options_size 	= options.size();

	if (!_req->_matched_server)
		return _req->setState(P_ERROR, V_HEADERS, "Trying to validate content type but no matching server", 404);

	std::map<std::string, std::vector<std::string> > server_content_types = _req->_matched_server->getMime(); // Do we even use this?

	if (!isValidMediaType(val)) //KM - remove or just check syntax? Not really an error or not? Changed to isValidMediaType from isValidContentType 
		return _req->setState(P_ERROR, V_HEADERS, "Invalid content-Type", 415);
	if (val.find("multipart/") != std::string::npos)
		return validateMultiPartHeader(val, options);
	if (val.find("charset") != std::string::npos)
		return validateCharset(options);
	return true;
}

// bool	isValidContentType(const std::string& name){ // THESE ARE TEST VALUES TO BE REPLACED WITH SERVER CHECK

// 	static std::vector<std::string> validTypes;

// 	if (validTypes.empty()){
// 		validTypes.push_back("application/x-www-form-urlencoded");
// 		validTypes.push_back("application/javascript");
// 		validTypes.push_back("application/json");
// 		validTypes.push_back("application/octet-stream");
// 		validTypes.push_back("application/xml");
// 		validTypes.push_back("multipart/form-data");
// 		validTypes.push_back("image/gif");
// 		validTypes.push_back("image/jpeg");
// 		validTypes.push_back("image/png");
// 		validTypes.push_back("image/svg+xml");
// 		validTypes.push_back("image/webp");
// 		validTypes.push_back("image/x-icon");
// 		validTypes.push_back("text/css");
// 		validTypes.push_back("text/html");
// 		validTypes.push_back("text/plain");
// 		validTypes.push_back("font/woff");
// 		validTypes.push_back("font/woff2");
// 	}
// 	return (find(validTypes.begin(), validTypes.end(), name) != validTypes.end());
// }

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
				if (!isValidMediaType(charset))
					return _req->setState(P_ERROR, V_HEADERS, ERR_MP_BOUNDARY_LENGTH, 415);
				if (!isSupportedMediaType(charset))
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

bool	RequestValidator::isSupportedMediaType(const std::string& type){
	//KM - finish
	(void)type;
	return true;
}

/*
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		VALIDATE MULTIPART PART		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<
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
	MULTIPART CONTENT DISPOSITION :
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
	else if ((part.name.find("file") != std::string::npos) || (part.name.find("File") != std::string::npos)){
		if (part.filename.length() == 0)
			return _req->setState(P_ERROR, P_BODY, ERR_MP_CONTENT_DISPOSITION_FILENAME_MISSING, 415); // CODE CHECK REQUIRED
		else {
			appendPathToRoot(&part.upload_dest, part.filename, _req->_re.paths.upload_base);
			getFilePermissions(part.upload_dest);
		}
		part.isFile = true;
		_req->_mp_m.filecount++;
	}
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
		if (!isValidMediaType(header[i]->val))
			continue;
		part.supported_content.push_back(header[i]);
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
	else
		_req->setRstate(P_MULTI);
	return true;
}

bool	RequestValidator::validateCharset(const std::vector< std::pair<std::string, std::string> >& options){

	const std::string& option_key = options[0].first;
	const std::string& option_val = options[0].second;

	if (option_key.compare("charset") == 0 && !option_val.empty()){
		if (!isValidMediaType(option_val))	// KM - add check to see if supported on server
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
	else if ((_req->_method == "POST" || _req->_method == "PUT") && _req->_version == "HTTP/1.1") // FOR 1.0, EOF can be used to indicate end of body if no TE or content-length
		return _req->setState(P_ERROR, "Malformed Request : Headers : content-length or transfer-encoding required for request type", 400);
	_req->_sizes.body_bytes_expected = 0;
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

	if (_req->_sizes.body_bytes_expected == 0)
		_req->setBodyType(NO_BODY, NOT_CHUNKED);
	else
		_req->setBodyType(HAS_BODY, NOT_CHUNKED);
	return true;
}

bool	RequestValidator::convertContentLength( void ){

	const std::string&	val = _req->_headers.find("content-length")->second[0]->val;
	char*				endptr = NULL;

	_req->_content_length = val.c_str();
	_req->_sizes.body_bytes_expected = strtoull(val.c_str(), &endptr, 10);
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
	replaceChar(name, ' ','_');
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

void	RequestValidator::validateBody() {
	if (!_req->_isChunked && _req->_sizes.body_bytes_expected < _req->_sizes.body_bytes_read) { 
		_req->setState(P_ERROR, V_BODY, "Incomplete Body after connection closed", 400);
		return;
	}
	if (_req->_isChunked && !_req->_body_complete) {
		_req->setState(P_ERROR, V_BODY, "Incomplete Body after connection closed (Chunked)", 400);
		return;
	}
	if (_req->_body_complete)
		_req->setRstate(REQ_DONE);
	if (_req->_r_state != P_TRAILERS)
		_req->setRstate(REQ_DONE);
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>				 		TRAILERS VALIDATION 				  		   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

void	RequestValidator::validateTrailers(){
	
	// TO_DO : Need to repeat header validation (but for trailers)
	_req->setRstate(REQ_DONE);
	_req->setState(P_DONE, "Parsing Complete", 200);
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

/* >>>>>>		VALIDATE MEDIA TYPE (Content-Type and Accept)		<<<<<< */
bool	RequestValidator::isValidMediaType(const std::string& value){

	size_t pos = value.find('/');

	if (pos != std::string::npos) {

		std::string type = value.substr(0, pos);
		std::string sub_type = value.substr(pos + 1);
		
		if (!(type == "*" || (type.length() > 0 && isValidToken(type))))
			return _req->setState(P_ERROR, V_HEADERS, "Header Validation Error : invalid media type", 400);
		if (!(sub_type == "*" || (sub_type.length() > 0 && isValidToken(sub_type))))
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