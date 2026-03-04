/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:37:52 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/23 12:44:20 by kmooney          ###   ########.fr       */
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
	_unparsed_len = 0;
	_expected_len = 0;
	_chunk_size = 0;
	_chunk_parser_status = PARSE_CHUNK_SIZE;
}

RequestParser::~RequestParser(){
}

void	RequestParser::clear() {
	_tknLst.clear();

	_p_it = token_s();
	_unparsed_len = 0;
	_expected_len = 0;
	_parsed_body_len = 0;
	_chunk_size = 0;
}


/**
 * @brief Main entry point to parse raw HTTP request data.
 *
 * This function feeds raw HTTP data into the parser, initializes internal iterators,
 * and triggers the parsing process. It processes as much of the request as possible
 * given the available data. Any remaining unparsed data length is recorded in `_unparsed_len`.
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
		_p_it.begin = _req->_buffer.begin();
	_p_it.end = _req->_buffer.end();
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

size_t	RequestParser::splitToToken( token_s& it, bool last_val, std::vector<token_s>& tkns, const std::string& delim )
{

	// NEED TO ADD DQUOTE CHECK AND ESCAPE CHAR
	std::deque<char>::const_iterator	parse_pos = it.begin;
	while (parse_pos != it.end)
	{
		std::deque<char>::const_iterator	compare = parse_pos;
		std::string::const_iterator			delim_pos = delim.begin();
		while (compare != it.end && delim_pos != delim.end() && *compare == *delim_pos) {
			compare++;
			delim_pos++;
		}
		if (delim_pos == delim.end()){
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
bool	RequestParser::splitToToken( token_s& it, bool last_val, token_s& tkn, const std::string& delim )
{
	std::deque<char>::const_iterator	parse_pos	= it.begin;
	
	while (parse_pos != it.end)	{
		
		std::deque<char>::const_iterator	compare = parse_pos;
		std::string::const_iterator			delim_pos = delim.begin();

		while (compare != it.end && delim_pos != delim.end() && *compare == *delim_pos) {
			compare++;
			delim_pos++;
		}
		if (delim_pos == delim.end()){
			setIterators(tkn, it.begin, parse_pos);
			it.begin = compare;
			return true;
		}
		else
			parse_pos++;
	}
	if (last_val == true && it.begin != it.end){
		setIterators(tkn, it.begin, it.end);
		it.begin = it.end; // added today 902/02
		return true;
	}
	return false;
}

bool	RequestParser::splitToTokenMultiDelims(token_s& it, bool last_val, token_s& tkn, const std::string& delims, char& found_delim)
{
	std::deque<char>::const_iterator parse_pos = it.begin;

	while (parse_pos != it.end)
	{
		if (delims.find(*parse_pos) != std::string::npos) {
			setIterators(tkn, it.begin, parse_pos);
			found_delim = *parse_pos;
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
	dest.assign(token.begin, token.end);
}

std::string	RequestParser::tokenToString(const token_s& token) {
	return std::string(token.begin, token.end);
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
		case P_TRAILERS	: { parseField(_p_it); break; }
		default			: { break; }
	}
	_req->_unparsed.begin = _p_it.begin;
	_req->_unparsed.end = _p_it.end;
	_req->_unparsed_length = std::distance(_p_it.begin, _p_it.end);
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

/**
 * @brief Parses the value of an HTTP header field, extracting the main value and its parameters.
 *
 * This function takes a header field value token (e.g., `"text/html; charset=UTF-8"`),
 * splits it by semicolons, and stores the first token as the main value.
 * Remaining tokens are treated as optional key-value parameters and stored in the `params` list
 * of the `field_t` object.
 *
 * Each optional parameter is parsed as a key-value pair (e.g., `"charset=UTF-8"`).
 * If a space precedes, or follows "=", _req->setState is called to change the state to P_ERROR and return false.
 * Keys are converted to lowercase for normalization.
 *
 * @param value_tkn The token representing the raw value of the header field (possibly with parameters).
 * @param new_field A pointer to a `field_t` struct that will be populated with the parsed data.
 * @return true Always returns true (parsing is assumed successful).
 *
 * @see splitToToken()
 * @see parseSingleToken()
 * @see tokenToString()
 * @see setState()
 * @see to_lower_ref()
 */
bool	RequestParser::parseFieldVal(token_s& value_tkn, field_t* field) {
	
	std::vector<token_s>			param_vector;
	std::vector<token_s>::iterator	it;
	
	token_s token;
	std::string	param_key;
	std::string	param_val;

	splitToToken(value_tkn, true, param_vector, ";");
	{	
		it = param_vector.begin();
		if (!tokenCharValidation(*it, "Malformed Field : Invalid Character in Field Value", 400, isTextChar))
			return false;
		tokenToString(*it++, field->val);
		trimWS(field->val);

		for (; it != param_vector.end(); it++) {
			if (parseSingleToken(*it, PARAM_KEY, param_key)) {
				tokenToString(*it, param_val);
				if ((!param_key.empty() && param_key[param_val.size() - 1] == ' ') ||
			 		(!param_val.empty() && param_val[0] == ' '))
					return _req->setState(P_ERROR, _req->_r_state, "Malformed Field : Parameters (Space before or after \"=\")", 400);
				trimWS(param_val);
				trimWS(param_key);
				to_lower_ref(param_key);
				field->params.push_back(std::make_pair(param_key, param_val));
			}
		}
	}
	return true;
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
	fieldMap_t*	fieldMap;
	
	if (_req->_r_state == P_HEADERS)
		fieldMap = &_req->_headers;
	else
		fieldMap = &_req->_trailers;

	if (parseSingleToken(line, FIELD_NAME, field_name)) {
		to_lower_ref(field_name);

		std::vector<token_s> values;
		splitToToken(line, true, values, ",");
	
		for (size_t i = 0; i < values.size(); i++) {
			token_s& currentToken = values[i];
	
			field_t* new_field = new field_t();
			parseFieldVal(currentToken, new_field);
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

	if (_req->getReqStatus() == P_HEADERS)
		next_state = V_HEADERS;
	else{
		next_state = V_TRAILERS;
	}

	while (it.begin != it.end) {
		if (!parseNextLine(it, line)) {
			_req->setPstate(P_AWAIT);
			return;
		}
		if (line.begin == line.end) {
			_req->setRstate(next_state);
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

	while (it.begin != it.end){
		if (_req->_isChunked){
			parseChunked(it);
			return;
		}
		else if (_req->_isMultiPart){
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
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>						 PARSING PLAIN BODY	  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/
void	RequestParser::parsePlain(token_s& it) {

	size_t remaining = _req->_content_length - _req->_body_length; //need to replace with _req->body_length (i.e. value not function);
	size_t available = std::distance(it.begin, it.end);
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
	ptrdiff_t dist = std::distance(it.begin, it.end);
	return dist >= 0 && static_cast<size_t>(dist) >= required;
}

bool	RequestParser::parseChunkData(token_s& it, std::string& data) {

	size_t	total_needed = _chunk_size + 2;

	if (!bytesAvailable(it, total_needed)) {
		_req->setPstate(P_AWAIT);
		return false;
	}
	
	token_s chunk;
	if (*(it.begin + _chunk_size) == '\r' && *(it.begin + _chunk_size + 1) == '\n'){
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

	// 1) multipart header will be parsed first 
	// 2) use this to populate boundary in MultipartMessage struct,			 
	// 2) then use this boundary to parse the body into parts.
	// 3) Each part will be stored as a MultipartPart struct, which contains the content type, disposition, filename (if applicable), and the content itself as a vector of chars.

/* 
 MULTIPART STRUCTS

typedef struct  {
	std::string			contentType;
	std::string			disposition;
	std::string			filename;
	bool				isFile;
	std::string			tempFilePath;
	std::vector<char>	content;
} MultipartPart;

typedef struct {
	std::vector<MultipartPart>	parts;
	std::string					boundary;
} MultipartMessage;
 */
void	RequestParser::parseMultiPart( token_s& it ){

	while (it.begin == it.end){
		// parse boundary
		// parse content type header
		// parse boundary delimited parts
			// for each part, parse headers and content
				// store content in temp file if isFile is true
		// store each part in _req->_multipart_message.parts
		// set _req->_multipart_message.boundary
		// if Content-Disposition header is present, parse filename and disposition type
			// if filename is present, set isFile to true and store content in temp file
			// if filename is not present, set isFile to false and store content in memory
		// if Content-Type header is present, store content type in MultipartPart struct
			// if not present, set content type to application/octet-stream
		// if Content-Type is multipart/mixed, need to recursively parse the content of that part as another multipart message using the same parsing logic, but with a different boundary specified in the Content-Type header of that part.
			// this will require a recursive call to parseMultiPart with the new boundary and content of that part as the input token stream.
			// need to ensure that the recursive parsing correctly handles the boundaries and does not interfere with the parsing of the outer multipart message.
		// once final boundary is reached, set state to P_DONE
		// if at any point parsing fails, set state to P_ERROR with appropriate message and code
		// NEED TO CONSIDER CASES WHERE BOUNDARY MAY BE SPLIT ACROSS CHUNKS, OR WHERE HEADERS MAY BE SPLIT ACROSS CHUNKS, ETC. MAY NEED TO IMPLEMENT A BUFFERING MECHANISM TO HANDLE INCOMPLETE DATA.
		// 		
		return;
	}
// 	std::string		data;

// 	while (it.begin != it.end) {
// 		if (_chunk_size == 0 && !parseChunkSize(it))
// 			return;
// 		if (_chunk_size == 0){
// 			_req->setRstate(V_BODY);
// 			_req->setPstate(P_DONE);
// 			return;
// 		}
// 		if (!parseChunkData(it, data))
// 			return;
// 		_req->_body += data;
// 		_chunk_size = 0;
// 	}
}

std::string	RequestParser::getParsedString(){
	std::string str = tokenToString(_p_it);
	return str;
}

bool	RequestParser::rlCharsCheck(const token_s& chars, const std::string& error, int errNum){
	
	std::deque<char>::const_iterator it = chars.begin;

	for (; it != chars.end; it++)
	{	
		if (*it < 32 || *it > 126){
			if ((it + 1) == chars.end)
				return true;
			return _req->setState(P_ERROR, error, errNum);
		}
	}
	return true;
}

bool	RequestParser::tokenCharValidation(const token_s& chars, const std::string& error, int errNum, bool (*f)(unsigned char)){
	
	std::deque<char>::const_iterator it = chars.begin;

	for (; it != chars.end; it++)
	{	
		if (f && !f(*it))
			return _req->setState(P_ERROR, error, errNum);
	}
	return true;
}

void	RequestParser::createContextMap(){

	static std::map< eParseContext, parseContext> _map;
	if (_map.empty())
	{
		_map[ABSOLUTE_URI] = parseContext	(false, DELIM_OPTIONAL, "://", "Request Line Malformed : Scheme", 400, isSchemeChar);
		_map[ORIGIN_URI] = parseContext		(false, DELIM_OPTIONAL, "/", "Request Line Malformed : Scheme", 400, isSchemeChar);
		_map[USER_NAME] = parseContext		(true , DELIM_OPTIONAL, ":", "Request Line Malformed : User", 400, isSchemeChar);
		_map[USER_PASS] = parseContext		(true , DELIM_OPTIONAL, "", "Request Line Malformed : Pass", 400, isSchemeChar);
		_map[QUERY_KEY] = parseContext		(true , DELIM_REQUIRED, ":", "Request Line Malformed : Query key", 400, isQueryChar);
		_map[QUERY_VAL] = parseContext		(false, DELIM_OPTIONAL, "", "Request Line Malformed : Query val", 400, isQueryChar);
		_map[FIELD_NAME] = parseContext		(false, DELIM_REQUIRED, ":", "Malformed header : Field Name", 400, isTokenChar);
		_map[FIELD_VAL] = parseContext		(false, DELIM_OPTIONAL, ".", "Malformed header : Field Val", 400, isFieldValueChar);
		_map[PARAM_KEY] = parseContext		(true , DELIM_REQUIRED, "=", "Malformed header : Field Parameter Key", 400, isTokenChar);
		_map[PARAM_VAL] = parseContext		(false, DELIM_OPTIONAL, ";", "Malformed header : Field Parameter Value", 400, isQuotedParamKeyChar);
		_map[CHUNKSIZE] = parseContext		(true , DELIM_OPTIONAL, ";", "Malformed Body : Invalid Chunk Size", 400, 0);
	}
	_contextMap = _map;
}