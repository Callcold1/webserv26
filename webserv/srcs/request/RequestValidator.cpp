/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestValidator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:26:19 by kmooney           #+#    #+#             */
/*   Updated: 2026/03/03 10:12:10 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "RequestValidator.hpp"
#include "RequestTypes.hpp"

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>				 CONSTRUCTION/DESTRUCTION/COPY						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

RequestValidator::RequestValidator(Request* request) : _req() { 
	_req = request;
	_config = request->_config;
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


bool	RequestValidator::locationSupportsMethod(const str_t& method) {

	std::vector<std::string> location_method = _location.getMethod();
	size_t size = _location.getMethod().size();
	
	for (size_t i = 0; i < size; i++){
		if (method.compare(location_method[i]))
			return true;
	}
	return false;
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

void	RequestValidator::setLocation(CLocation& location){
	//_location = _server->getLocationForTarget(_req->_uri.path);
	_location = location; // temporary - really need to use CServer object to find location - this should happen in the client class
}

bool	RequestValidator::validateMethod(){

	if (_req->_r_state != V_REQLINE) {
		if (!locationSupportsMethod(_req->_method))
			return _req->setState(P_ERROR, _req->_r_state, "Method Not Permitted", 501);
		else if (locationSupportsMethod(to_upper(_req->_method)))
			return _req->setState(P_ERROR, V_REQLINE, "Method must be uppercase", 400);
	}
	else if (!isValidMethod(_req->_method))
		return _req->setState(P_ERROR, V_REQLINE, "Method Not Supported", 501);
	return true;
}

/* bool	RequestValidator::validateVersion(){
 	A server can send a 505 (HTTP Version Not Supported) response if it wishes, for any reason, 
	to refuse service of the client's major protocol version.
	if (!(_req->_version.compare("HTTP/1.1") == 0 || _req->_version.compare("HTTP/1.0") == 0))
		return _req->setState(P_ERROR, V_REQLINE, "Unsupported Version", 505);
	return true;
} */

bool	RequestValidator::validateVersion()
{
	if (_req->_version.compare("HTTP/1.1") == 0 || _req->_version.compare(str_t("HTTP/1.0")) == 0)
		return true ;
	else {
		if (_req->_version.compare(str_t("HTTP/0.9")) == 0 ||
			_req->_version.compare(str_t("HTTP/2.0")) == 0 ||
			_req->_version.compare(str_t("HTTP/3.0")) == 0) {
			return _req->setState(P_ERROR, V_REQLINE, "Unsupported Version", 505);
		}
		else
			return _req->setState(P_ERROR, V_REQLINE, "Unsupported Version", 400);
	}
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
	if (_req->_uri.scheme.empty())
		return _req->setState(P_ERROR, V_REQLINE, "Malformed Request Line : Scheme element missing from Absolute Form URI", 400);
	to_lower_ref(_req->_uri.scheme);
	if (_req->_uri.scheme.compare("https") == 0 || _req->_uri.scheme.compare("http") == 0)
		return true;
	return _req->setState(P_ERROR, V_REQLINE, "Scheme Unsupported", 400);
}

/*  URI USER VALIDATION  */

bool	RequestValidator::validateUser(){
	/* INCOMPLETE */
		/* userinfo = *( unreserved / pct-encoded / sub-delims / ":" ) */
	/* THIS SHOULD PROBABLY RETURN ERROR 400 or 404
	 	USERNAME / PASS SHOULD NOT APPEAR IN THE
	*/
	
	if (!percentDecode( _req->_uri.user ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid Percent Encoding (User) ", 400);
	if (!isValidUTF8( _req->_uri.user ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid UTF-8 encoding (User)", 400);
	return true;
}

/*  URI PASS VALIDATION  */
bool	RequestValidator::validatePass(){
	if (!percentDecode( _req->_uri.pass ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid Percent Encoding (Pass) ", 400);
	if (!isValidUTF8( _req->_uri.user ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid UTF-8 encoding (Pass)", 400);
	return true;
}

/*  URI HOST VALIDATION  */
bool	RequestValidator::validateHost() {
	if ( _req->_uri.host.empty())
		return _req->setState(P_ERROR, "Malformed Request Line : Missing Host Element ", 400);
	if (!percentDecode( _req->_uri.host ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid Percent Encoding (Host) ", 400);
	if (!isValidUTF8( _req->_uri.host ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid Percent Encoding (Host) ", 400);
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
		return _req->setState(P_ERROR, "Malformed Request Line : Path ", 400);
	//else if (_req->_.path.empty())
	// 	_req->_.path[0] = '//';
	// else if (_req->_.uri_type == ABSOLUTE)
	// 	remove_dot_segments(_req->_.path);
	// 	else if (_req->_.path[0] == '//') {
	// //	merge_path(base_path); // need to get base path from Server Config */
	// 	remove_dot_segments(_req->_.path);
	// }
	if (!percentDecode( _req->_uri.path ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid Percent Encoding (Path) ", 400);
	if (!isValidUTF8( _req->_uri.path ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid Percent Encoding (Path) ", 400);
	return true;
}

/*  URI QUERY VALIDATION  */

bool	RequestValidator::validateQuery(){
	if (!percentDecode( _req->_uri.query ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid Percent Encoding (Query) ", 400);
	if (!isValidUTF8( _req->_uri.query ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid Percent Encoding (Query) ", 400);
	return true;
}

/*  URI FRAGMENT VALIDATION  */

bool	RequestValidator::validateFrag(){
	if (!percentDecode( _req->_uri.frag ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid Percent Encoding (Frag) ", 400);
	if (!isValidUTF8( _req->_uri.frag ))
		return _req->setState(P_ERROR, "Malformed Request Line : Invalid Percent Encoding (Frag) ", 400);
	return true;
}

/*
>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		URI VALIDATION UTILS  	   <<<<<<
>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/
 bool	RequestValidator::percentDecode( str_t& encoded)
 {
/* URI 7.3
	Note, however, that the "%00" percent-encoding
   (NUL) may require special handling and should be rejected if the
   application is not expecting to receive raw data within a component. */
	 str_t	decoded;
	 size_t	len = encoded.length();
	 size_t	i = 0;
	 
	 decoded.reserve(len);
	 while ( i < len )
	 {
		 if (encoded[i] == '%')
		 {
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
	if (_req->_hasBody)
		_req->setRstate(P_BODY);
	else {
		_req->setRstate(REQ_DONE);
		_req->setPstate(P_DONE);
	}
}

bool	RequestValidator::validateHostHeader() { // URI COMPARISON REQUIRED

	int count = countHeaderValues(_req->_headers, "host");
	
	if ( count == 0)
		return _req->setState(P_ERROR, V_HEADERS, "Missing 'Host' header", 400);
	if ( count > 1)
		return _req->setState(P_ERROR, V_HEADERS, "Multiple 'Host' header values", 400);
/* 	When an origin server receives a request with an absolute-form of request-target,
	the origin server MUST ignore the received Host header field (if any) and instead 
	use the host information of the request-target. Note that if the request-target does
	 not have an authority component, an empty Host header field will be sent in this case.
	  A server MUST accept the absolute-form in requests even though most HTTP/1.1 clients 
	  will only send the absolute-form to a proxy. */
	return true;
}

/*
>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		REPRESENTATION VALIDATION  <<<<<<
>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
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
	// Need to iterate over vector
	const std::string& 											val 			= header->second[0]->val;
	const std::vector< std::pair<std::string, std::string> >&	options 		= header->second[0]->params;
	//int 														options_size 	= options.size();

	//const std::vector<std::string>  validTypes = config.getValidTypes(route); // PSEUDO SERVER CHECK CODE - TO REPLACE TEMP
	if (!temporaryIsValidContentType(val))
		return _req->setState(P_ERROR, V_HEADERS, "Content-Type not supported", 415);

	if (val.find("multipart/") != std::string::npos)
		return validateMultiPart(val, options);
	if (val.find("charset") != std::string::npos)
		return validateCharset(options);
	//return validateCharset(val, options);
	return true;
}

bool	temporaryIsValidContentType(const std::string& name){ // THESE ARE TEST VALUES TO BE REPLACED WITH SERVER CHECK
	static std::vector<std::string> validTypes;
	validTypes.push_back("text/html");
	validTypes.push_back("text/plain");
	validTypes.push_back("application/json");
	validTypes.push_back("multipart/form-data");
	validTypes.push_back("multipart/related");

	if (std::find(validTypes.begin(), validTypes.end(), name) == validTypes.end())
		return false;
	return true;
}

bool	temporaryIsValidCharset(const std::string& name){ // THESE ARE TEST VALUES TO BE REPLACED WITH SERVER CHECK
	static std::vector<std::string> validTypes;
	validTypes.push_back("utf-8"); // STANDARD CHARSET - PRETTY UBIQUITOUS
	validTypes.push_back("ISO-8859-1"); // SINGLE CHAR LATIN

	if (std::find(validTypes.begin(), validTypes.end(), name) == validTypes.end())
		return false;
	return true;
}

bool	RequestValidator::validateMultiPart(const std::string& val, const std::vector< std::pair<std::string, std::string> >& options){
	size_t		size = options.size();
	std::string	boundary;
	std::string	type;

	bool is_related;
	bool has_boundary;
	bool has_type;

	if (val == "multipart/related")
		is_related = true;

	if (size != 0) {

		for (size_t i = 0; i < size; i++){
			const std::string& option_key = options[i].first;
			const std::string& option_val = options[i].second;
			
			if (option_key.compare("boundary") == 0){
				boundary = option_val;
				has_boundary = true;
			}
			else if (option_key.compare("type") == 0){
				type = option_val;
				has_type = true;
			}
		}
	}
	if (!has_boundary)
		return _req->setState(P_ERROR, V_HEADERS, "Missing multiPart boundary", 400);
	if (has_type && !is_related)
		return _req->setState(P_ERROR, V_HEADERS, "Invalid option in Content-Type header (type) ", 400);
	if (!is_related && has_type)
		return _req->setState(P_ERROR, V_HEADERS, "Missing default type", 400);
	if (!temporaryIsValidContentType(type))
		return _req->setState(P_ERROR, V_HEADERS, "Content-Type not supported", 415);

	_req->setBoundaryString("--" + boundary);
	if (has_type)
		_req->setDefaultMultiPartType(type);
	_req->_isMultiPart = true;
	return true;
}

//bool	RequestValidator::validateCharset(const std::string& val, const std::vector< std::pair<std::string, std::string> >& options){
bool	RequestValidator::validateCharset(const std::vector< std::pair<std::string, std::string> >& options){ // i.e CONTENT TYPE
	
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

int	RequestValidator::countHeaderValues(const fieldMap_t& headers, const std::string& headerName) {
	fieldMap_t::const_iterator it = headers.find(headerName);
	if (it != headers.end()) {
		return it->second.size();
	}
	return 0;
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
