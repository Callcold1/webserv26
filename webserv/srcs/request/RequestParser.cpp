/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junruh <junruh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:37:52 by kmooney           #+#    #+#             */
/*   Updated: 2026/03/24 13:59:25 by junruh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "Request.hpp"

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					CONSTRUCTION/DESTRUCTION/COPY					   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
																				*/
RequestParser::RequestParser(Request* request): _req(){
	createContextMap();
	_req = request;
	_req->setRstate(P_REQLINE);
	_req->setPstate(P_PARSE);
	_os_body_len = 0;
	_expected_len = 0;
	_parsed_body_len = 0;
	_chunk_size = 0;
	_chunk_parser_status = PARSE_CHUNK_SIZE;
	_mp_p.state = MP_NONE;
}

RequestParser::~RequestParser(){
}

void	RequestParser::clear() {
	_tknLst.clear();

	_p_it = token_s();
	_os_body_len = 0;
	_expected_len = 0;
	_parsed_body_len = 0;
	_chunk_size = 0;
	_chunk_parser_status = PARSE_CHUNK_SIZE;
	_mp_p.state = MP_NONE;
}

/**
 * @brief Main entry point to parse raw HTTP request data.
 *
 * This function feeds raw HTTP data into the parser, initializes internal iterators,
 * and triggers the parsing process. It processes as much of the request as possible
 * given the available data. Any remaining unparsed data length is recorded in `_req->_unparsed_len`.
 *
 * @param data A pointer to the raw HTTP request data buffer.
 * @param length The size of the data buffer in bytes.
 *
 * @see addData()
 * @see setIterators()
 * @see parseRequest()
 */
void	RequestParser::parse(const char* data, size_t length){
	if (length == 0){
		_req->setState(P_ERROR, P_REQLINE, "Empty Request", 400);
		return;
	}
	addData(data, length);
	setIterators();
	parseRequest();
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>				 ADDING DATA AND SETTING ITERATORS					   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
																				*/
/**
 * @brief Appends raw data bytes to the internal parsing buffer.
 *
 * This function takes a pointer to a buffer and appends each byte to the internal `_buffer`
 * used by the parser. This buffer accumulates data across multiple calls until a full request
 * can be parsed.
 *
 * @param data Pointer to the raw input data buffer.
 * @param length Number of bytes to append from the input buffer.
 *
 * @note This function does not perform any parsing or validation — it only stores data.
 */
void	RequestParser::addData(const char* data, size_t length){
	for (std::size_t i = 0; i < length; i++)
		_req->_buffer.push_back(data[i]);
}

/**
 * @brief Initializes internal parsing iterators for processing the request buffer.
 *
 * This function sets the beginning and end positions of the `_p_it` token range,
 * which is used to track the portion of the buffer currently being parsed.
 *
 * If the request is in the `P_REQLINE` state, it resets the beginning of the parse range
 * to the start of the buffer. The end iterator is always set to the current end of the buffer.
 *
 * @note This function must be called after new data is added to the buffer via `addData()`.
 *
 * @see _p_it
 * @see _req
 */
void	RequestParser::setIterators(){
	if (_req->_r_state == P_REQLINE)
		_p_it.begin = 0;
	_p_it.end = _req->_buffer.size();
}

/**
 * @brief Sets the begin and end iterators of a token.
 *
 * This templated utility function assigns the provided `begin` and `end` iterators
 * to the corresponding fields of the given `token_s` object.
 *
 * @tparam T The iterator type (e.g., std::string::iterator or std::vector<char>::iterator).
 * @param t The token object whose iterator boundaries will be set.
 * @param begin The iterator pointing to the start of the token's range.
 * @param end The iterator pointing to the end of the token's range.
 *
 * @note This function is commonly used to isolate and define specific ranges within
 * the request buffer for parsing.
 */
template <class T>
void	RequestParser::setIterators(token_s& t, T& begin, T& end){
	t.begin = begin;
	t.end = end;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 SPLITING FUNCTIONS								   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
																				*/
/**
 * @brief Extracts the next line from the input token stream.
 *
 * This function attempts to split the given token `it` at the next line delimiter (typically CRLF or LF),
 * storing the result in `line`. It is used during HTTP request parsing to extract individual lines
 * (such as the request line or header lines) from the raw input buffer.
 *
 * Internally, it delegates to `splitToToken()` to perform the actual line splitting.
 *
 * @param it The input token stream to split.
 * @param line A token that will receive the next line extracted from the input.
 * @return true if a line was successfully extracted; false if no full line is currently available.
 *
 * @see splitToToken()
 */
bool	RequestParser::parseNextLine(token_s& it, token_s& line) {
	return splitToToken(it, false, line, "\r\n");
}

/**
 * @brief Parses a single token from the input stream, splitting at a specified delimiter.
 *
 * This function extracts a single token from the input token stream `it` and stores it in
 * the `dest` string. The function splits the input based on the specified delimiter `delim` (e.g., a space, colon, etc.).
 * If the delimiter is not found or if the token cannot be parsed, it returns false, and if `delim_req` is true,
 * it sets the parser state to an error.
 *
 * @param it The input token stream to parse.
 * @param last_val Indicates whether this is the last token in the stream.
 * @param dest A reference to a string where the parsed token will be stored.
 * @param delim The delimiter to split the token at.
 * @param delim_req If true, an error state will be set if the delimiter is not found.
 * @return true if the token was successfully parsed and stored in `dest`, false if parsing failed.
 *
 * @see splitToToken()
 * @see tokenToString()
 * @see RequestParser::setPstate()
 */
// bool	RequestParser::parseSingleToken( token_s& it, bool last_val, std::string& dest, const std::string& delim, bool delim_req ){
// 	token_s tkn;
// 		//DOES THIS CAUSE A PROBLEM WITH CHARACTER CHECKING
// 	if (splitToToken(it, last_val, tkn, delim)){
// 		tokenToString(tkn, dest);
// 		return true;
// 	}
// 	if (delim_req == true)
// 		_req->setPstate(P_ERROR);
// 	return false;
// }

bool	RequestParser::parseSingleToken( token_s& it, eParseContext contextNum, std::string& dest){

	token_s target;

	std::map< eParseContext, parseContext >::iterator map_it = _contextMap.find(contextNum);
	parseContext* context = &map_it->second;

	if (splitToToken(it, context->last_val, target, context->delim)){
		if (!tokenCharValidation(target, context->error, context->errNum, context->validator))
			return false;
		tokenToString(target, dest);
		return true;
	}
	if (context->delim_req == true)
		_req->setPstate(P_ERROR);
	return false;
}

/**
 * @brief Splits a token into a vector of tokens based on a string delimiter.
 * 			The delimiter is discarded.
 *
 * This function splits a token at the occurrence of a sequence of characters defined by the `delim` string.
 * The resulting tokens are stored in the `tkns` vector.
 *
 * If `last_val` is true, the remainder of the token is included as the last token.
 *
 * @param it The token to be split, represented by the `token_s` object.
 * @param last_val If true, the final portion of the token is included even if it is not followed by a delimiter.
 * @param tkns A vector to store the resulting tokens.
 * @param delim The string used as the delimiter for splitting.
 * @return The number of tokens generated.

 */

size_t	RequestParser::splitToToken( token_s& it, bool last_val, std::vector<token_s>& tkns, const std::string& delim)
{
	// NEED TO ADD DQUOTE CHECK AND ESCAPE CHAR
	size_t	parse_pos = it.begin;
	size_t	delim_end = delim.length();
	
	while (parse_pos < it.end)
	{
		size_t	compare = parse_pos;
		size_t	delim_pos = 0;

		while (compare < it.end && delim_pos < delim_end && _req->_buffer[compare] == delim[delim_pos]) {
			compare++;
			delim_pos++;
		}
		if (delim_pos == delim_end && (!(parse_pos == it.begin && compare - it.begin == delim.length()))){ //&& parse_pos > 1 added to prevent single space appearing in results
			tkns.push_back(token_s(it.begin, parse_pos));
			parse_pos = compare;
			it.begin = parse_pos;
		}
		else
			parse_pos++;
	}
	if (last_val == true && it.begin != it.end){
		tkns.push_back(token_s(it.begin, it.end));
		it.begin = it.end;
	}
	return tkns.size();
}

/**
 * @brief Finds and extracts a token before a string delimiter (delimiter is discarded)
 *
 * This function looks for the occurrence of a string delimiter in the token and extracts the portion
 * of the token that precedes it. The result is stored in `tkn`.
 *
 * If the delimiter is found, the iterator `it` is advanced to the character following the delimiter.
 *
 * @param it The token to search within, represented by the `token_s` object.
 * @param last_val If true, the last portion of the token is included even if it is not followed by a delimiter.
 * @param tkn A reference to the `token_s` object where the extracted token will be stored.
 * @param delim The string delimiter to search for.
 * @return true if a token was successfully extracted; false if no delimiter was found.
 */
bool	RequestParser::splitToToken( token_s& it, bool last_val, token_s& tkn, const std::string& delim ) {

	size_t	parse_pos = it.begin;
	size_t	delim_end = delim.length();

	while (parse_pos < it.end) {

		size_t	compare = parse_pos;
		size_t	delim_pos = 0;

		while (compare < it.end && delim_pos < delim_end && (_req->_buffer[compare] == delim[delim_pos])) {
			compare++;
			delim_pos++;
		}
		if (delim_pos == delim_end){
			setIterators(tkn, it.begin, parse_pos);
			it.begin = compare;
			return true;
		}
		else
			parse_pos++;
	}
	if (last_val == true && it.begin != it.end){
		setIterators(tkn, it.begin, it.end);
		it.begin = it.end;
		return true;
	}
	return false;
}

bool	RequestParser::splitToTokenMultiDelims(token_s& it, bool last_val, token_s& tkn, const std::string& delims, char& found_delim)
{
	size_t parse_pos = it.begin;

	while (parse_pos < it.end)
	{
		if (delims.find(_req->_buffer[parse_pos]) != std::string::npos) {
			setIterators(tkn, it.begin, parse_pos);
			found_delim = _req->_buffer[parse_pos];
			it.begin = parse_pos + 1;
			return true;
		}
		parse_pos++;
	}

	if (last_val && it.begin != it.end) {
		setIterators(tkn, it.begin, it.end);
		found_delim = '\0';
		it.begin = it.end;
		return true;
	}
	found_delim = '\0';
	return false;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 HELPER FUNCTIONS		  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/**
 * @brief Converts a token into a string.
 *
 * This function converts a `token_s` object, which contains a range defined by iterators (`begin` and `end`),
 * into a `std::string` by constructing the string from the characters between the `begin` and `end` iterators.
 *
 * @param token The token object to be converted to a string.
 * @return A `std::string` representing the characters between the `begin` and `end` iterators of the token.
 */
void	RequestParser::tokenToString(const token_s& token, std::string& dest) {
	dest = std::string(_req->_buffer.begin() + token.begin, _req->_buffer.begin() + token.end);
}

std::string	RequestParser::tokenToString(const token_s& token) {
	std::string str(_req->_buffer.begin() + token.begin, _req->_buffer.begin() + token.end);
	return str;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING CONTROL		  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/**
 * @brief Parses an HTTP request by processing its various components: request line, headers, and body.
 *
 * This function checks the current state of the request (`_r_state`) and invokes the appropriate parsing
 * function based on the state. The eURIstates& include:
 * - `P_REQLINE`: Parses the request line (method, URI, and protocol).
 * - `P_HEADERS`: Parses the HTTP headers.
 * - `P_BODY	: Parses the body of the HTTP request.
 * - `P_TRAILERS: Parses the HTTP trailers (if transfer encoding is chunked).
 *
 *
 * The `_p_state` is set to `P_PARSE` to indicate that parsing is in progress.
 *
 * @note The function works by calling the specific parsing functions like `parseReqLine`, `parseHeaders`,
 * and `parseBody` depending on the state of the request.
 */
void	RequestParser::parseRequest(){

	_req->_p_state = P_PARSE;
	switch (_req->_r_state){
		case P_REQLINE	: { parseReqLine(_p_it); break; }
		case P_HEADERS	: { parseField(_p_it); break; }
		case P_BODY		: { parseBody (_p_it); break; }
		case P_MULTI	: { parseBody (_p_it); break; }
		case P_TRAILERS	: { parseField(_p_it); break; }
		default			: { break; }
	}
	_req->_unparsed.begin = _p_it.begin;
	_req->_unparsed.end = _p_it.end;
	if (_p_it.end >= _p_it.begin)
		_req->_unparsed_length = _p_it.end - _p_it.begin;
	else
		_req->setState(P_ERROR, "Error Parsing : Index invalidated", 500);
	return ;
}


/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING REQUEST LINE	  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/**
 * @brief Parses the HTTP request line.
 *
 * This function processes the request line (which includes the HTTP method, URI, and version) of
 * the incoming HTTP request. It uses the `getRequestLine` function to retrieve the request line
 * and then calls splitToToken which then splits the request line using space to delimit.
 * If there are not exactly 2 spaces ( 3 rl arguments ), an error is returned indicating malformed request line.
 * - `RL_COMPLETE`: Indicates that the request line has been fully parsed.
 *
 * The function checks for any parsing errors, and if an error is detected, it sets the appropriate
 * error state and message.
 *
 * @param it The token iterator pointing to the current position in the input data.
 *
 * @return This function does not return any value but modifies the state of the request and
 *         the `_rl_state` during parsing.
 *
 * @note If the request line format is incorrect, the state is set to `P_ERROR` and an error message is logged.
 */
void	RequestParser::parseReqLine(token_s& it){

	token_s	rl;

	if (!getRequestLine(rl, it))
		return;
	tokenToString(rl, _req->_req_line);
	size_t token_count = splitToToken(rl, true, _tknLst, " ");
	if (token_count < 3){
		_req->setState(P_ERROR, "Malformed Request Line : too few parameters in request line", 400);
		return;
	}
	else if (token_count > 3){
		_req->setState(P_ERROR, "Malformed Request Line : too many parameters in request line", 400);
		return;
	}
	tokenToString(_tknLst[0], _req->_method);
	if (!parseURI(_tknLst[1]))
	 	return;

	tokenToString(_tknLst[2], _req->_version);
	_req->setRstate(V_REQLINE);
	return;
}

/**
 * @brief Retrieves the request line from the HTTP request.
 *
 * This function attempts to parse the next line from the input iterator and assigns it to the
 * `rl` token. If the line is successfully parsed, it sets the request line state to `RL_METHOD`.
 * If the line cannot be parsed, the function sets the request parsing state to `P_AWAIT`.
 *
 * @param rl The token where the parsed request line will be stored.
 * @param it The iterator pointing to the current position in the input data.
 *
 * @return `true` if the request line was successfully parsed, `false` otherwise.
 */
bool	RequestParser::getRequestLine(token_s& rl, token_s &it){

	if (parseNextLine(it, rl))
		return rlCharsCheck(rl, "Request Line Format Error : invalid character", 400);
	else if (rlCharsCheck(it, "Request Line Format Error : invalid character", 400))
		_req->setPstate(P_AWAIT);
	return false;
}

/*
>>>>>>>>>>>>>>>>>>>>>><<<<<<
>>>>>>	 URI PARSING  <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>><<<<
*/

bool	RequestParser::parseURI( token_s& uri )
{
	eURIstates	state = SCHEME;

	tokenToString(uri, _req->_uri_str);
	while (uri.begin != uri.end)
	{
		switch (state) {
			case SCHEME:	state = parseScheme(state, uri); break;
			case AUTH:		state = parseAuth(state, uri); break;
			case PATH: 		state = parsePath(state, uri); break;
			case QUERY: 	state = parseQuery(state, uri); break;
			case FRAG:		state = parseFrag(state, uri); break;
			case URI_ERROR:	return false;
			case URI_END: return true;
			default: break;
		}
	}
	return true;
}

eURIstates&	RequestParser::parseScheme( eURIstates& state, token_s& uri ){

	if (parseSingleToken(uri, ABSOLUTE_URI, _req->_uri.scheme)){
		_req->_uri.uri_type = ABSOLUTE_FORM;
		return state = AUTH;
	}
	if (_req->getParseStatus() == P_ERROR)
		return state = URI_ERROR;

	if (parseSingleToken(uri, ORIGIN_URI, _req->_uri.scheme)){
		_req->_uri.uri_type = ORIGIN_FORM;
		_req->_uri.path.append("/");
		return state = PATH;
	}
	if (_req->getParseStatus() == P_ERROR)
		return state = URI_ERROR;

	_req->setState(P_ERROR, "Malformed Request Line : No authority or path in URI ", 400);
	return state = URI_ERROR;
}

eURIstates&	RequestParser::parseAuth( eURIstates& state, token_s& uri ){
	token_s		auth;
	char		found_delim;

	if (splitToTokenMultiDelims(uri, true, auth, "/?#", found_delim )){

		tokenToString(auth, _req->_uri.auth);
		if (!_req->_uri.scheme.empty() && _req->_uri.auth.empty())
			_req->setState(P_ERROR, "Malformed Request Line : No authority or server component in URI", 400);

	while (state == AUTH || state == USER || state == HOST || state == PORT)
		switch (state){
			case AUTH : 	{ state = USER; continue; }
			case USER:		state = parseUser(state, auth); continue;
			case HOST: 		state = parseHost(state, auth); continue;
			case PORT: 		state = parsePort(state, auth); continue;
			case URI_ERROR: return state = URI_ERROR;
			default	 : { break;}
		}
	}
	switch (found_delim){
		case '/' : { _req->_uri.path.append("/"); return state = PATH; }
		case '?' : { return state = QUERY; }
		case '#' : { return state = FRAG;}
		default	 : { return state = URI_END;}
	}
}

eURIstates&	RequestParser::parseUser( eURIstates& state, token_s& auth ){

	token_s userinfo;

	if (splitToToken(auth, false, userinfo, "@" )){
		if (!tokenCharValidation(userinfo, "Malformed Request Line : Invalid characters in User element", 400, isUserinfoChar))
			return state = URI_ERROR;
		if (parseSingleToken(userinfo, USER_NAME, _req->_uri.user))
			parseSingleToken(userinfo, USER_PASS, _req->_uri.pass);
	}
	return state = HOST;
}

eURIstates&	RequestParser::parseHost( eURIstates& state, token_s& uri ){

	token_s		host;
	char		found_delim;

	if (splitToTokenMultiDelims(uri, true, host, ":/?#", found_delim )){
		if (!tokenCharValidation(host, "Malformed Request Line : Invalid characters in Host element ", 400, isHostChar))
			return state = URI_ERROR;
		tokenToString(host, _req->_uri.host);
		switch (found_delim){
			case ':' : { return state = PORT;}
			default	 : { return state = PATH;}
		}
	}
	return state = PATH;
}

eURIstates&		RequestParser::parsePort( eURIstates& state, token_s& auth ){

	if (!tokenCharValidation(auth, "Malformed Request Line : Invalid characters in Port element", 400, isDigit))
		return state = URI_ERROR;
	tokenToString(auth, _req->_uri.port);
	return state = PATH;
}

eURIstates&		RequestParser::parsePath( eURIstates& state, token_s& uri ){
	token_s		path;
	std::string path_str;
	char		found_delim;

	if (splitToTokenMultiDelims(uri, true, path, "?#", found_delim )){
		if (!tokenCharValidation(path, "Malformed Request Line : Invalid characters in Path element", 400, isPathChar))
			return state = URI_ERROR ;

		tokenToString(path, path_str);
		_req->_uri.path += path_str;
		switch (found_delim){
			case '?' : { return state = QUERY;}
			case '#' : { return state = FRAG; }
			default	 : { break;}
		}
	}
	return state = URI_END;
}

eURIstates&	RequestParser::parseQuery( eURIstates& state, token_s& uri ){

	token_s		query;

	if (splitToToken(uri, true, query, "#")){
		if (!tokenCharValidation(query, "Malformed Request Line : Invalid characters in Query element", 400, isQueryChar))
			return state = URI_ERROR;
		tokenToString(query, _req->_uri.query);
		state = FRAG;
	}
	else
		state = URI_END;

	std::string	key;
	std::string	val;
	std::vector<token_s> querylist;

	if (state == URI_END)
		splitToToken(uri, true, querylist, "&");
	else
		splitToToken(query, true, querylist, "&");
	std::vector<token_s>::iterator it = querylist.begin();

	for (; it != querylist.end(); it++) {
		if (parseSingleToken(*it, QUERY_KEY, key)) {
			if (!tokenCharValidation(*it, "Malformed Request Line : Query value ", 400, isQueryChar))
				return state = URI_ERROR;
			tokenToString(*it, val);
			to_lower_ref(key);
			_req->_uri.queries.push_back(std::make_pair(key, val));
		}
	}
	return state;
}

eURIstates&	RequestParser::parseFrag( eURIstates& state, token_s& uri ){
	if (!tokenCharValidation(uri, "Malformed Request Line : Invalid characters in Fragment element", 400, isFragmentChar))
		return state = URI_ERROR;
	tokenToString(uri, _req->_uri.frag);
	return state = URI_END;
}

bool	RequestParser::URInormalisation( void ){
	return true;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING HEADERS		  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

bool	RequestParser::parseFieldValue(token_s& value_tkn, field_t* field){

	size_t	parse_pos = value_tkn.begin;

	while (_req->_buffer[parse_pos] == ' ' || _req->_buffer[parse_pos] == '\t')
		parse_pos++;
	if (_req->_buffer[parse_pos] == '"'){
		value_tkn.begin = parse_pos;
		if (parseQuotedString(value_tkn, field->val))
			return true;
		else
			return _req->setState(P_ERROR, "Malformed Field : Invalid Field Value", 400);
	}
	else {
		if (!tokenCharValidation(value_tkn, "Malformed Field : Invalid Character in Field Value", 400, isTextChar))
			return false;
		tokenToString(value_tkn, field->val);
		trimWS(field->val);
		return true;
	}
}

bool	RequestParser::parseFieldParamKey(token_s& token_key, std::string& key){
	tokenToString(token_key, key);
	trimLWS(key);
	if (!tokenCharValidationStr(key, "Malformed Field : Invalid parameter key", 400, isTokenChar))
		return false;
	to_lower_ref(key);
	return true;
}


bool	RequestParser::parseFieldParamValue(token_s& token_val, std::string& value){

	size_t	parse_pos = token_val.begin;

	if (_req->_buffer[parse_pos] == '"'){
		if (parseQuotedString(token_val, value))
			return true;
		else
			return _req->setState(P_ERROR, "Malformed Field : Invalid parameter value", 400);
	}
	else {
		tokenToString(token_val, value);
		trimTWS(value);
		return tokenCharValidationStr(value, "Malformed Field : Invalid parameter value", 400, isTokenChar);
	}
}

bool	RequestParser::parseFieldParams(std::vector<token_s>& params, field_t* field){

	std::string param_val;
	std::string param_key;
	std::vector<token_s> vec;

	for (size_t i = 0; i < params.size(); i++) {
		vec.clear();
		size_t count = splitToToken(params[i], true, vec, "=");
		if (count != 2)
			return _req->setState(P_ERROR, _req->_r_state, "Malformed Field : Parameter(s)", 400);
		if (!parseFieldParamKey(vec[0], param_key))
			return false;
		if (!parseFieldParamValue(vec[1], param_val))
			return false;
		field->params.push_back(std::make_pair(param_key, param_val));
	}
	return true;
}

/**
 * @brief Parses the value of an HTTP header field, extracting the main value and its parameters.
 *
 * This function takes a header field value token (e.g., `"text/html; charset=UTF-8"`),
 * splits it by semicolons, and stores the first token as the main value.
 * Remaining tokens are treated as optional key-value parameters and stored in the `params` list
 * of the `field_t` object.
 *
 * parseFieldParams() is called if the size of the param_vector is greater than 1 where
 * each optional parameter is parsed as a key-value pair (e.g., `"charset=UTF-8"`).
 * @param line_tkn The token representing the raw value of the header field (possibly with parameters).
 * @param new_field A pointer to a `field_t` struct that will be populated with the parsed data.
 * @return true Always returns true (parsing is assumed successful).
 *
 * @see splitToToken()
 * @see parseFieldValue()
 * @see parseFieldParams()
 */
bool	RequestParser::splitFieldLine(token_s& line_tkn, field_t* field) {

	std::vector<token_s>	param_vector;

	splitToToken(line_tkn, true, param_vector, ";");
	if (param_vector.size() == 0 || !parseFieldValue(param_vector[0], field))
		return false;
	if (param_vector.size() == 1)
		return true;
	std::vector<token_s> params(param_vector.begin() + 1, param_vector.end());
	return parseFieldParams(params, field);
}

/**
 * @brief Parses a single HTTP field line and stores the result in the request object.
 *
 * This method processes a token representing an HTTP field line (e.g., "Content-Type: text/html").
 * It extracts the field name and associated values, normalizes the name to lowercase,
 * splits comma-separated values if present, and stores them as a vector of `field_t*`
 * in the `_req->_headers or _req->_trailers map.
 *
 * If the line is malformed (e.g., missing a colon delimiter), the method sets the request state
 * to an error with HTTP status code 400.
 *
 * @param line The tokenized line representing one HTTP field line.
 * @return true if the field line was successfully parsed and stored; false if the line is malformed.
 *
 * @see parseSingleToken()
 * @see parsefield_values()
 * @see RequestParser::setState()
 */

bool	RequestParser::parseFieldLine(token_s& line) {
	std::string					field_name;
	std::vector< field_t * >	field_vector;
	fieldMap_t*					fieldMap;

	if (_req->_r_state == P_HEADERS)
		fieldMap = &_req->_headers;
	else if (_req->_r_state == P_TRAILERS)
		fieldMap = &_req->_trailers;
	else if(_req->_r_state == P_MULTI)
		fieldMap = currentPart().headers; // returns NULL for 2nd mp

	if (parseSingleToken(line, FIELD_NAME, field_name)) {
		to_lower_ref(field_name);

		std::vector<token_s> values;
		// if (field_name == "user-agent"){
			// std::vector< std::pair <char, char > > delim_pairs;
			// delim_pairs.push_back(std::pair<char,char> ('(', ')'));
			// parseDelimiterPairs(line, values, delim_pairs, ' ');
		// }
		// else
		if (field_name == "user-agent")
			splitToToken(line, true, values, "\r\n");
		else
			splitToToken(line, true, values, ",");
		for (size_t i = 0; i < values.size(); i++) {
			token_s& currentToken = values[i];
			field_t* new_field = new field_t();
			if (field_name == "user-agent")
				parseFieldValue(currentToken, new_field);
			else
				splitFieldLine(currentToken, new_field);
			field_vector.push_back(new_field);
		}
		(*fieldMap)[field_name] = field_vector;
		return true;
	}
	else
		return false;
}

/**
 * @brief Parses all HTTP field lines from the input token stream.
 *
 * This function iteratively extracts and parses field lines from the provided token stream `it`.
 * For each line:
 * - If the line is empty, it signals the end of the field (headers or trailers) section and transitions the request
 *   to the next state (V_HEADERS OR V_TRAILERS) as appropriate.
 * - If the line cannot be read (e.g., due to incomplete input), it sets the parser state to `P_AWAIT`
 *   to await more data.
 * - If a field line is malformed, parsing is aborted (the error is set by `parseFieldLine()`).
 *
 * On successful parsing of all headers, the parser state is set to `P_AWAIT`.
 *
 * @param it The token stream representing the raw HTTP field section to be parsed.
 *
 * @see parseNextLine()
 * @see parseFieldLine()
 * @see RequestParser::setPstate()
 * @see RequestParser::setRstate()
 */

void	RequestParser::parseField(token_s& it) {
	token_s line;
	eReqState next_state;

	switch (_req->getReqStatus()){
		case (P_HEADERS) : {next_state = V_HEADERS; break;}
		case (P_TRAILERS) : {next_state = V_TRAILERS; break;}
		case (P_MULTI) : {next_state = V_MPHEADERS; break;}
		default : break;
	}

	while (it.begin != it.end) {
		if (!parseNextLine(it, line)) {
			_req->setPstate(P_AWAIT);
			return;
		}
		if (line.begin == line.end) {
			_req->setRstate(next_state);
			if (next_state == V_MPHEADERS)
				_mp_p.state = MP_BODY;
			return;
		}
		if (!parseFieldLine(line)) {
			return;
		}
	}
	_req->setPstate(P_AWAIT);
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>						 PARSING BODY		  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/* FUNCTION TO PARSE THE BODY */
void	RequestParser::parseBody(token_s& it) {

	if (!checkBodySize())// Need to move buffer if parsing is complete and tmpfile
		return;
	while (it.begin != it.end){
		if (_req->_isChunked){
			parseChunked(it);
			return;
		}
		else if (_req->_isMultiPart){
			_req->setRstate(P_MULTI);
			parseMultiPart(it);
			return;
		}
		else {
			parsePlain(it);
			return;
		}
	}
	_req->setPstate(P_AWAIT);
}

/*
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		CHECK BODY SIZE		<<<<<<
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/

bool	RequestParser::checkBodySize(void){ // Need to move buffer if parsing is complete and tmpfile

	if ((long long)_req->_body_length > _req->_matched_location->getMaxBodySize())
		_req->setState(P_ERROR, P_REQLINE, "Bad Request : Malformed Body : maximum size exceeded", 400);
	
	if (_req->_body_buffer > MAX_LOCAL_BODY){
		
		int fd = -1;

		if (!_req->_bodyInFile){
			if (!_req->_isMultiPart && _req->_isChunked){
				if (!assignTempfile(_req->_body_filename, fd, "Chunked_"))
					_req->setState(P_ERROR, P_REQLINE, "Server Error : Problem creating tempfile", 500);
			}
			else if (!_req->_bodyInFile && !_req->_isMultiPart && !_req->_isChunked){
				if (!assignTempfile(_req->_body_filename, fd, "Plain_"))
					_req->setState(P_ERROR, P_REQLINE, "Server Error : Problem creating tempfile", 500);
			}
			//_req->_re.paths.upload_paths.push_back(std::pair< fileInfo_s, std::string>(_req->, _req->_body_filename)); //Need to check where file destination is (fileInfo_s) - when upload folder, that's ok but what about cgi?
		}	// Is this handled in updateRe?
		if(!appendToTempFile(_req->_body_filename, _req->_body))
			_req->setState(P_ERROR, P_REQLINE, "Server Error : Problem creating tempfile", 500);
		_req->_body_buffer = 0;
	}
	return true;
}

bool	RequestParser::appendToTempFile(std::string& filename, std::string& data){

	std::ofstream ofs(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);

	if (!ofs)
		return false;

	ofs.write(data.c_str(), data.size());
	ofs.close();
    return true;
}

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		PARSING PLAIN BODY		<<<<<<
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
*/
void	RequestParser::parsePlain(token_s& it) {

	size_t remaining = _req->_content_length - _req->_body_length; //need to replace with _req->body_length (i.e. value not function);
	if (it.end < it.begin)
		_req->setState(P_ERROR, "Parsing Error : invalid index", 500);
	size_t available = it.end - it.begin;
	size_t to_copy   = std::min(remaining, available);

	if (to_copy > 0) {
		_req->_body.append(it.begin, it.begin + to_copy);
		_req->_body_length += to_copy;
		it.begin += to_copy;
	}
	if (_req->_body_length < _req->_content_length) {
		_req->setPstate(P_AWAIT); // RELYING ON SERVER TO TIME OUT IF FULL BODY IS NOT RECEIVED
		return;
	}
	if (_req->_body_length != _req->_content_length)
		_req->setState(P_ERROR, P_REQLINE, "Bad Request : Malformed Body : body size does not match value in content-length header", 400);
	else
		_req->setState(P_DONE, V_BODY, "Parsing Complete", 200);
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING CHUNKED BODY							   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

bool	bytesAvailable(token_s& it, size_t required) {
	size_t dist = 0;
	if ( it.end > it.begin)
		dist = it.end - it.begin;
	return dist >= required;
}

bool	RequestParser::parseChunkData(token_s& it, std::string& data) {

	size_t	total_needed = _chunk_size + 2;

	if (!bytesAvailable(it, total_needed)) {
		_req->setPstate(P_AWAIT);
		return false;
	}

	token_s chunk;
	if (_req->_buffer[it.begin + _chunk_size] == '\r' && _req->_buffer[it.begin + _chunk_size + 1] == '\n'){
		chunk.begin = it.begin;
		chunk.end = it.begin + _chunk_size;
		tokenToString(chunk, data);
		it.begin += total_needed;
	}
	else
		return _req->setState(P_ERROR, P_BODY, "Malformed Body : Invalid chunk termination", 400);
	_chunk_parser_status = PARSE_CHUNK_SIZE;
	return true;
}

bool	RequestParser::parseChunkSize( token_s& it){

	token_s	line;

	if (!parseNextLine(it, line)) {
		_req->setPstate(P_AWAIT);
		return false;
	}
	std::string	size_str;

	_chunk_parser_status = PARSE_CHUNK_BODY;
	parseSingleToken(line, CHUNKSIZE, size_str);
	if (substrToHex(_chunk_size, size_str, 0, size_str.length()))
		return true;
	return _req->setState(P_ERROR, P_BODY, "Malformed Body : Invalid chunk size", 400);
}

void	RequestParser::finalChunkChecks(token_s& it){
	token_s line;

	if (!parseNextLine(it, line)) {
		_req->setPstate(P_AWAIT);
		return;
	}
	if (line.begin != line.end)
		_req->setRstate(P_TRAILERS);
	else
		_req->setState(P_DONE, V_BODY, "Parsing Complete", 200);
}

void	RequestParser::parseChunked( token_s& it ){

	std::string data;

	while (it.begin != it.end) {
		if (_chunk_parser_status == PARSE_CHUNK_SIZE) {
			if (!parseChunkSize(it))
				 return;
		}
		if (_chunk_parser_status == PARSE_CHUNK_BODY){
			if (_chunk_size == 0){
				finalChunkChecks(it);
				return;
			}
			if (!parseChunkData(it, data))
					return;
			_req->_body +=data;
			_req->_content_length +=_chunk_size;
			_chunk_size = 0;
		}
	}
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING MULTIPART								   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

void	RequestParser::parseMultiPart( token_s& it ){
	if (_mp_p.state == MP_NONE){
		_mp_p.boundary =_req->_boundary;
		_mp_p.boundary_marker =_req->_boundary + "--";
		_mp_p.state = MP_BOUNDARY;
	}
	while (_mp_p.state != MP_COMPLETE && _req->_p_state == P_PARSE){
		switch (_mp_p.state) {
			case MP_NONE:		return;
			case MP_BOUNDARY:	parseBoundaryMP(it); _req->_mp_state = &_mp_p.state; break;
			case MP_HEADERS:	parseField(it); _req->_mp_state = &_mp_p.state; return;
			case MP_BODY: 		parseBodyMP(it); _req->_mp_state = &_mp_p.state; return;
			case MP_FINAL:		parseFinalMP(it, it.end - it.begin); _req->_mp_state = &_mp_p.state; return;
			case MP_ERROR:		_req->_mp_state = &_mp_p.state; return;
			default: break;
		}
	}
	return;
}

void	RequestParser::parseBoundaryMP(token_s& it){

	token_s	line;
	bool	final = false;
	size_t	size = _mp_p.boundary.size();
	size_t	token_size = it.end - it.begin;

	// Full line received and boundary or last boundary not present 		== error
	// Part line received and last boundary matches 						== message complete (Could receive a final optional \r\n in next read)
	// Part line received and boundary matches but final does not			== await \r\n

	if (token_size < size + 2){ 																		// +2 allows for "boundary + \r\n" or "boundary + --"
		if (!parseNextLine(it, line))
			_req->setPstate(P_AWAIT);
		else {																							// if \r\n received, then the expected boundary line is invalid
			_req->setState(P_ERROR, P_BODY, "Malformed Body : invalid multipart boundary", 400); 		// CODE CHECK REQUIRED
			_mp_p.state = MP_ERROR;	
		}
	}
	else if (matchBoundaryMP(it.begin, it.end, final)){													// boundary matches : if final, then final boundary matches
		if (final)
			parseFinalMP(it, token_size);
		else if (!parseNextLine(it, line) || (line.end - line.begin) > size){ 							// either no "boundary\r\n" received or "boundary + extra chars" - both errors 
				_req->setState(P_ERROR, P_BODY, "Malformed Body : invalid multipart boundary", 400); 	// CODE CHECK REQUIRED
				_mp_p.state = MP_ERROR;
		}
		else {
			mpPart_t	part;

			_req->_mp_m.parts.push_back(part);
			_mp_p.current_index = _req->_mp_m.parts.size() - 1;
			_mp_p.state = MP_HEADERS;
		}
	}
	return;
}

bool	RequestParser::matchBoundaryMP(size_t begin, size_t end, bool& final){

	size_t size = _mp_p.boundary.size();
	size_t final_size = size + 2;
	bool match = true;

	if (end - begin < size)
		return false;

	else if (end - begin >= final_size){

		for (size_t i = 0; i < final_size; i++) {
			if (_req->_buffer[begin + i] != _mp_p.boundary_marker[i]){
				match = false;
				break;
			}
		}
		if (match == true){
			final = true;
			return true;
		}
	}
	for (size_t i = 0; i < size; i++) {
		if (_req->_buffer[begin + i] != _mp_p.boundary[i])
			return false;
	}
	return true;
}

/* 
	parseFinalMP called in 2 places: 
		1)  parseBoundary MP : Final Boundary has been encountered but the line hasn't been parsed yet
		
		2)  parseMultiPart   : This function was previously called and a possible \r\n is awaited (either --boundary-- or --boundary--\r received)
							    Either way, only the \r\n is awaited. If this hasn't been received, then in the case of --boundary--\r we treat as error
		
	 	If parseNextLine works --boundary--\r\n has been encountered and parsing is set to complete, unless data follows (i.e. it.begin != it.end)

		If we want to allow pipelining, we need to change this function.
*/
void	RequestParser::parseFinalMP(token_s& it, ptrdiff_t size) {

	token_s		line;
	ptrdiff_t	bm_size = (ptrdiff_t)_mp_p.boundary_marker.size();
																													// final boundary received 
	if (!parseNextLine(it, line) && _mp_p.state != MP_FINAL){														// no \r\n received	
		_mp_p.state = MP_FINAL;
		if (size == bm_size){
			_req->setState(P_AWAIT, V_MPBODY, "Parsing Complete", 200);																					
			it.begin = it.end;
		}																											// token contains exactly final boundary
		else if (size == bm_size + 1 && (_req->_buffer[it.begin + bm_size] == '\r' && (it.begin + bm_size + 1 == it.end))){		// \r received and awaiting \n
			_req->setState(P_AWAIT, V_MPBODY, "Parsing Complete", 200);
			it.begin = it.end - 1;
		}
		else {
			_req->setState(P_ERROR, P_BODY, "Invalid characters following final boundary", 400); 					// CODE?
			_mp_p.state = MP_ERROR;
		}
		return;
	}
	if (it.begin == it.end){ 																						// Parsing is complete as final \r\n received 
		_req->setState(P_DONE, V_MPBODY, "Parsing Complete", 200);
		_mp_p.state = MP_COMPLETE;
	}
	else {
		_req->setState(P_ERROR, P_BODY, "Invalid characters following final boundary", 400);
		_mp_p.state = MP_ERROR;
	}
}

void	RequestParser::parseBodyMP(token_s& it) {

	mpPart_t&	part = currentPart();
	size_t		pos = it.begin;
	size_t		boundary_size = _mp_p.boundary.size() + 4;
	size_t		dist = 0;
	bool		final = false;

	while (pos != it.end){

		if (it.end - pos >= boundary_size) {

			if (_req->_buffer[pos] == '\r' && _req->_buffer[pos + 1] == '\n') {

				size_t	boundary_pos = pos + 2;

				if (matchBoundaryMP(boundary_pos, it.end, final)){
					dist = pos - it.begin;
					if (part.isFile)
						appendToTempfileMP(part, it.begin, dist);
					else
						appendToBodyMP(part, it.begin, dist);
					it.begin = boundary_pos;
					parseBoundaryMP(it);
					_req->setRstate(V_MPBODY);
					return;
				}
				else {
					pos += 2;
					continue;
				}
			}
		}
		pos++;
	}
	size_t available = it.end - it.begin;

	if (available > 0 && available > boundary_size){
		if (part.isFile)
			appendToTempfileMP(part, it.begin, available - boundary_size);
		else
			appendToBodyMP(part, it.begin, available - boundary_size);
	}
	it.begin += available - boundary_size;
	_req->setPstate(P_AWAIT);
}

void	RequestParser::appendToTempfileMP(mpPart_t& part, size_t begin, size_t size) {
	
	if (part.tempFilePath.length() == 0 || part.fd == -1){
		if (!assignTempfile(part.tempFilePath, part.fd, "Multipart_")){
			_mp_p.state = MP_ERROR;
			return;
		}
		_req->_re.paths.upload_paths.push_back(std::pair< fileInfo_s, std::string >(part.upload_dest, part.tempFilePath));
	}

	std::ofstream ofs(part.tempFilePath.c_str(), std::ios::out | std::ios::binary | std::ios::app);

	if (!ofs) {
		_mp_p.state = MP_ERROR;
		return;
	}
	size_t written;
	for (written = 0; written < size; begin++, written++){
		ofs.put(_req->_buffer[begin]);
	}
	ofs.close();
}

void	RequestParser::appendToBodyMP(mpPart_t& part, size_t begin, size_t size) {
	part.body.insert(part.body.end(), _req->_buffer.begin() + begin, _req->_buffer.begin() + begin + size);
	//part.body.insert(part.body.end(), _req->_buffer[begin], _req->_buffer[begin + size]);
}

mpPart_t& RequestParser::currentPart() {
	return _req->_mp_m.parts[_mp_p.current_index];
}

std::string	RequestParser::getParsedString(){
	std::string str = tokenToString(_p_it);
	return str;
}

/*
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		CHARACTER VALIDATION		<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
*/

bool	RequestParser::rlCharsCheck(const token_s& chars, const std::string& error, int errNum){

	for (size_t it = chars.begin; it != chars.end; it++){
		if (_req->_buffer[it] < 32 || _req->_buffer[it] > 126){
			if ((it + 1) == chars.end)
				return true;
			return _req->setState(P_ERROR, error, errNum);
		}
	}
	return true;
}

bool	RequestParser::tokenCharValidation(const token_s& chars, const std::string& error, int errNum, bool (*f)(unsigned char)){

	for (size_t it = chars.begin; it != chars.end; it++)
	{
		if (f && !f(_req->_buffer[it]))
			return _req->setState(P_ERROR, error, errNum);
	}
	return true;
}

bool	RequestParser::tokenCharValidationStr(const std::string& str, const std::string& error, int errNum, bool (*f)(unsigned char)) {
	for (size_t i = 0; i < str.size(); i++) {
		if (!f || !f(str[i]))
			return _req->setState(P_ERROR, error, errNum);
	}
	return true;
}

/*
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
>>>>>>		PARSING CONTEXT MAP		<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

void	RequestParser::createContextMap(){

	static std::map< eParseContext, parseContext> _map;
	if (_map.empty())
	{
		_map[ABSOLUTE_URI] = parseContext	(false, DELIM_OPTIONAL, "://", "Request Line Malformed : Scheme", 400, isSchemeChar);
		_map[ORIGIN_URI] = parseContext		(false, DELIM_OPTIONAL, "/", "Request Line Malformed : Scheme", 400, isSchemeChar);
		_map[USER_NAME] = parseContext		(true , DELIM_OPTIONAL, ":", "Request Line Malformed : User", 400, isSchemeChar);
		_map[USER_PASS] = parseContext		(true , DELIM_OPTIONAL, "", "Request Line Malformed : Pass", 400, isSchemeChar);
		_map[QUERY_KEY] = parseContext		(true , DELIM_REQUIRED, "=", "Request Line Malformed : Query key", 400, isQueryChar);
		_map[QUERY_VAL] = parseContext		(false, DELIM_OPTIONAL, "", "Request Line Malformed : Query val", 400, isQueryChar);
		_map[FIELD_NAME] = parseContext		(false, DELIM_REQUIRED, ":", "Malformed header : Field Name", 400, isTokenChar);
		_map[FIELD_VAL] = parseContext		(false, DELIM_OPTIONAL, ".", "Malformed header : Field Val", 400, isFieldValueChar);
		_map[PARAM_KEY] = parseContext		(true , DELIM_REQUIRED, "=", "Malformed header : Field Parameter Key", 400, isTokenChar);
		_map[PARAM_VAL] = parseContext		(false, DELIM_OPTIONAL, ";", "Malformed header : Field Parameter Value", 400, isQuotedParamKeyChar);
		_map[CHUNKSIZE] = parseContext		(true , DELIM_OPTIONAL, ";", "Malformed Body : Invalid Chunk Size", 400, 0);
	}
	_contextMap = _map;
}

bool	RequestParser::parseQuotedString(token_s& it, std::string& out) {

	size_t	pos = it.begin;
	size_t	end = it.end;

	if (pos == end || _req->_buffer[pos] != '"')
		return false;

	pos++;

	std::string result;

	while (pos != end) {

		char c = _req->_buffer[pos];

		if (c == '"') {
			pos++;
			it.begin = pos;
			out = result;
			return true;
		}

		if (c == '\\') {
			pos++;
			if (pos == end)
				return false;
			result.push_back(_req->_buffer[pos]);
			pos++;
			continue;
		}

		if ((unsigned char)c < 32 && c != '\t')
			return false;
		result.push_back(c);
		pos++;
	}
	return false;
}

/* 
	If a delimiter is found, instead of splitting as normal, if the first of a subsequent delimiter pair is found,
	then the first delimiter will be ignored.

	To deal with 'User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:148.0) Gecko/20100101 Firefox/148.0'
	Should parse to
						Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:148.0)
						Gecko/20100101
						Firefox/148.0
*/
bool	RequestParser::parseDelimiterPairs(token_s& it, std::vector<token_s>& out, const std::vector<std::pair <char, char> >& delim_pairs, char primary_delim) {

	bool					in_delim = false;
	std::pair<char, char>	active_pair;
	size_t					pos = it.begin;
	size_t					end = it.end;
	size_t					num_pairs = delim_pairs.size();
	token_s					result;

	if (_req->_buffer[pos] == primary_delim) // allow first char to be primary delim, and if so skip (don't)
		pos++;

	while (pos != end) {
				
		result.begin = pos;
		result.end = pos;

		while (pos != end) {

			char c = _req->_buffer[pos];
			
			if ((c == primary_delim)) {
				
				pos++;										// move to char following primary_delim
				c = _req->_buffer[pos];
				
				for (size_t i = 0; i < num_pairs; i++){

					if (c == delim_pairs[i].first) {		// check if current char is one of the delim pairs.
						active_pair = delim_pairs[i];	
						in_delim = true;
						pos++;
						break;
					}
				}
				if (!in_delim){
					result.end = pos - 1;
					if (result.begin < result.end)
						out.push_back(result);
					break;
				}
				while (in_delim && pos < end){				// char is one a delim_pair - skip chars until partner is found  
					if (_req->_buffer[pos] != active_pair.second){
						pos++;
					}
					else
						in_delim = false; // active pair.second matches
				}
				if (in_delim)			  // pos == end and active_pair.second not found
					return false;
				pos++;
			}
			else if (pos == end - 1){
				result.end = end;
				out.push_back(result);
				it.begin = ++pos;
			}
			else if ((c == primary_delim) && (result.begin != result.end)){
				result.end = pos;
				out.push_back(result);
				break;
			}
			else
				pos++;
		}
	}
	if (it.begin < it.end){
		out.push_back(it);
		it.begin = pos;
	}
	return true;
}