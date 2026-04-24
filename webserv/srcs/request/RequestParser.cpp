/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:37:52 by kmooney           #+#    #+#             */
/*   Updated: 2026/04/24 17:51:11 by kmooney          ###   ########.fr       */
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
	_chunk_size = 0;
	_chunk_parser_status = PARSE_CHUNK_SIZE;
	_mp_p.state = MP_NONE;
}

RequestParser::~RequestParser(){
}

void	RequestParser::clear() {
	_tknLst.clear();

	_p_it = token_s();
	_chunk_size = 0;
	_chunk_parser_status = PARSE_CHUNK_SIZE;
	_mp_p.state = MP_NONE;
}

/**
 * @brief Main entry point to parse raw HTTP request data.
 *
 * This function feeds raw HTTP data into the parser, initializes internal iterators,
 * and triggers the parsing process. It processes as much of the request as possible
 * given the available data. Any remaining bytes_unread data length is recorded in `_req->_unparsed_len`.
 *
 * @param data A pointer to the raw HTTP request data buffer.
 * @param length The size of the data buffer in bytes.
 *
 * @see addData()
 * @see setIterators()
 * @see parseRequest()
 */
void	RequestParser::parse(const char* data, size_t length){
	if (length == 0 && _req->_sizes.bytes_unread == 0){ // && _req->_sizes.bytes_unread == 0 added 20/4/26 - check for issues
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
void	RequestParser::addData(const char* data, size_t length){
	for (std::size_t i = 0; i < length; i++)
		_req->_buffer.push_back(data[i]);
}

void	RequestParser::setIterators(){
	if (_req->_r_state == P_REQLINE)
		_p_it.begin = 0;
	_p_it.end = _req->_buffer.size();
}


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

bool	RequestParser::parseNextLine(token_s& it, token_s& line) {
	return splitToToken(it, false, line, "\r\n");
}

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

size_t	RequestParser::splitToToken( token_s& it, bool last_val, std::vector<token_s>& tkns, const std::string& delim)
{
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

void	RequestParser::parseRequest(){

	size_t start = _p_it.begin;

	_req->_p_state = P_PARSE;
	switch (_req->_r_state){
		case P_REQLINE	: { parseReqLine(_p_it); break; }
		case P_HEADERS	: { parseField(_p_it); break; }
		case P_BODY		: { parseBody (_p_it); break; }
		case P_MULTI	: { parseBody (_p_it); break; }
		case P_TRAILERS	: { parseField(_p_it); break; }
		default			: { break; }
	}
	_req->_sizes.bytes_read += _p_it.begin - start;
	_req->_unparsed.begin = _p_it.begin;
	_req->_unparsed.end = _p_it.end;
	if (_p_it.end >= _p_it.begin)
		_req->_sizes.bytes_unread = _p_it.end - _p_it.begin;
	else
		_req->setState(P_ERROR, "Error Parsing : Index invalidated", 500);
	return ;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING REQUEST LINE	  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
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

void	RequestParser::parseBody(token_s& it) {

	size_t start = it.begin;

	while (it.begin != it.end){
		if (_req->_isChunked){
			parseChunked(it);
			_req->_sizes.body_bytes_read += it.begin - start;
			return;
		}
		else if (_req->_isMultiPart){
			_req->setRstate(P_MULTI);
			parseMultiPart(it);
			_req->_sizes.body_bytes_read += it.begin - start;
			return;
		}
		else {
			parsePlain(it);
			_req->_sizes.body_bytes_read += it.begin - start;
			return;
		}
		if (!checkBodySize())
			return;
	}
	_req->setPstate(P_AWAIT);
}

/*
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		CHECK BODY SIZE		<<<<<<
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/

bool	RequestParser::checkBodySize(void){ // Need to move buffer if parsing is complete and tmpfile

	_req->_sizes.body_local_size = _req->_body.size();
	if ((long long)_req->_sizes.body_bytes_read > _req->_matched_location->getMaxBodySize())
		_req->setState(P_ERROR, P_REQLINE, "Bad Request : Malformed Body : maximum size exceeded", 400);

	if (_req->_sizes.body_local_size > MAX_LOCAL_BODY || (_req->_bodyInFile && _req->_sizes.body_bytes_read == _req->_sizes.body_bytes_expected && _req->_sizes.body_local_size > 0)){

		if (!_req->_bodyInFile){
			if (!_req->_isMultiPart && _req->_isChunked){
				if (!assignTempfile(_req->_body_filename, &(_req->_upload.path), "Chunked_"))
					return _req->setState(P_ERROR, P_REQLINE, "Server Error : Problem creating tempfile", 500);
			}
			else if (!_req->_bodyInFile && !_req->_isMultiPart && !_req->_isChunked){
				if (!assignTempfile(_req->_body_filename, &(_req->_upload.path), "Plain_"))
					return _req->setState(P_ERROR, P_REQLINE, "Server Error : Problem creating tempfile", 500);
			}
			_req->_bodyInFile = true;
			appendPathToRoot(_req->_upload.path, _req->_re.paths.upload_base);
			getFilePermissions(_req->_upload);
			_req->_re.paths.upload_paths.push_back(std::pair< fileInfo_s, std::string>(_req->_upload, _req->_body_filename));
		}
		if(!appendToTempFile(_req->_body_filename, _req->_body))
			_req->setState(P_ERROR, P_REQLINE, "Server Error : Problem creating tempfile", 500);
		_req->_sizes.body_local_size = 0;
	}
	return true;
}

bool	RequestParser::appendToTempFile(std::string& filename, std::string& data){

	std::ofstream ofs(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);

	if (!ofs)
		return false;

	ofs.write(data.c_str(), data.size());
	ofs.close();
	data.clear();
    return true;
}

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		PARSING PLAIN BODY		<<<<<<
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
*/
void	RequestParser::parsePlain(token_s& it) {

	size_t remaining = _req->_sizes.body_bytes_expected - _req->_sizes.body_bytes_read;

	if (it.end < it.begin)
		_req->setState(P_ERROR, "Parsing Error : invalid index", 500);

	size_t available = it.end - it.begin;
	size_t to_copy   = std::min(remaining, available);

	if (to_copy > 0) {
		_req->_body.append(_req->_buffer.begin() + it.begin, _req->_buffer.begin() + it.begin + to_copy);
		_req->_sizes.body_size += to_copy;
		it.begin += to_copy;

		if (_req->_sizes.body_size < _req->_sizes.body_bytes_expected) {
			_req->setPstate(P_AWAIT);
			return;
		}
	}
	if (_req->_sizes.body_size != _req->_sizes.body_bytes_expected)
		_req->setState(P_ERROR, P_REQLINE, "Bad Request : Malformed Body : body size does not match value in content-length header", 400);
	else {
		_req->setState(P_DONE, V_BODY, "Parsing Complete", 200);
		_req->_body_complete = true;
	}
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

	_req->_body_complete = true;

	if (line.begin != line.end)
		_req->setRstate(P_TRAILERS);
	else{
		_req->setState(P_DONE, V_BODY, "Parsing Complete", 200);
		_req->_body_complete = true;
	}
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
				_req->_body_complete = true;
				finalChunkChecks(it);
				return;
			}
			if (!parseChunkData(it, data))
					return;
			_req->_body +=data;
			_req->_sizes.body_size +=_chunk_size;
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

	if (token_size < size + 2){
		if (!parseNextLine(it, line))
			_req->setPstate(P_AWAIT);
		else {
			_req->setState(P_ERROR, P_BODY, "Malformed Body : invalid multipart boundary", 400);
			_mp_p.state = MP_ERROR;
		}
	}
	else if (matchBoundaryMP(it.begin, it.end, final)){
		if (final)
			parseFinalMP(it, token_size);
		else if (!parseNextLine(it, line) || (line.end - line.begin) > size){
				_req->setState(P_ERROR, P_BODY, "Malformed Body : invalid multipart boundary", 400);
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


void	RequestParser::parseFinalMP(token_s& it, ptrdiff_t size) {

	token_s		line;
	ptrdiff_t	bm_size = (ptrdiff_t)_mp_p.boundary_marker.size();

	if (!parseNextLine(it, line) && _mp_p.state != MP_FINAL){
		_mp_p.state = MP_FINAL;
		if (size == bm_size){
			_req->setState(P_AWAIT, V_MPBODY, "Parsing Complete", 200);
			it.begin = it.end;
		}
		else if (size == bm_size + 1 && (_req->_buffer[it.begin + bm_size] == '\r' && (it.begin + bm_size + 1 == it.end))){
			_req->setState(P_AWAIT, V_MPBODY, "Parsing Complete", 200);
			it.begin = it.end - 1;
		}
		else {
			_req->setState(P_ERROR, P_BODY, "Invalid characters following final boundary", 400);
			_mp_p.state = MP_ERROR;
		}
		return;
	}
	if (it.begin == it.end){
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

	if (part.tempFilePath.length() == 0){
		if (!assignTempfile(part.tempFilePath, NULL, "Multipart_")){
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
	_req->_sizes.body_size += size;
	part.body_size += size;
}

void	RequestParser::appendToBodyMP(mpPart_t& part, size_t begin, size_t size) {

	part.body.insert(part.body.end(), _req->_buffer.begin() + begin, _req->_buffer.begin() + begin + size);
	_req->_sizes.body_size += size;
	part.body_size += size;
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

bool	RequestParser::parseDelimiterPairs(token_s& it, std::vector<token_s>& out, const std::vector<std::pair <char, char> >& delim_pairs, char primary_delim) {

	bool					in_delim = false;
	std::pair<char, char>	active_pair;
	size_t					pos = it.begin;
	size_t					end = it.end;
	size_t					num_pairs = delim_pairs.size();
	token_s					result;

	if (_req->_buffer[pos] == primary_delim)
		pos++;

	while (pos != end) {

		result.begin = pos;
		result.end = pos;

		while (pos != end) {

			char c = _req->_buffer[pos];

			if (c == primary_delim) {

				pos++;
				c = _req->_buffer[pos];

				for (size_t i = 0; i < num_pairs; i++){

					if (c == delim_pairs[i].first) {
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
				while (in_delim && pos < end){
					if (_req->_buffer[pos] != active_pair.second){
						pos++;
					}
					else
						in_delim = false;
				}
				if (in_delim)
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
