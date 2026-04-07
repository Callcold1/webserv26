/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 22:01:11 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/23 12:42:37 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core_def.hpp"
#include "utilityFunctions.hpp"
#include "RequestValidator.hpp"
#include "RequestTypes.hpp"

#define PARSE_CHUNK_SIZE false
#define PARSE_CHUNK_BODY true

class Request;

class RequestParser {
	public:
					RequestParser(Request* request);
					~RequestParser();

	void			clear();
	void			parse(const char* data, size_t length);
	void			addData(const char* data, size_t length);

	void			parseRequest();

	void			parseReqLine(token_s& token);
	bool			getRequestLine(token_s& rl, token_s& it);

	bool			parseURI( token_s& rl );
	eURIstates&		parseScheme( eURIstates& state, token_s& uri );
	eURIstates&		parseAuth( eURIstates& state, token_s& uri );
	eURIstates&		parseUser( eURIstates& state, token_s& uri );
	eURIstates&		parseHost( eURIstates& state, token_s& uri );
	eURIstates&		parsePort( eURIstates& state, token_s& auth );
	eURIstates&		parsePath( eURIstates& state, token_s& uri );
	eURIstates&		parseQuery( eURIstates& state, token_s& uri );
	eURIstates&		parseFrag( eURIstates& state, token_s& uri );
	bool			URInormalisation( void );

	void			parseField(token_s& it);
	bool			parseFieldLine(token_s& line);
	bool			splitFieldLine(token_s& value_tkn, field_t* field);
	bool			parseFieldValue(token_s& value_tkn, field_t* field);
	bool			parseFieldParams(std::vector<token_s>& params, field_t* field);
	bool			parseFieldParamKey(token_s& token_key, std::string& key);
	bool			parseFieldParamValue(token_s& token_val, std::string& value);

	void			parseBody(token_s& it);
	void			parsePlain(token_s& it);
	bool			parseChunkData(token_s& it, std::string& data);
	bool			parseChunkSize(token_s& it);
	void			finalChunkChecks(token_s& it);
	void			parseChunked(token_s& it);

	void			parseMultiPart(token_s& it);
	void			parseBoundaryMP(token_s& it);
	void			parseHeadersMP(token_s& it);
	void			parseBodyMP(token_s& it);
	void			parseFinalMP(token_s& it, ptrdiff_t size);
	bool			matchBoundaryMP(size_t begin, size_t end, bool& final);
	void			appendToTempfileMP(mpPart_t& part, size_t begin, size_t size);
	void			appendToBodyMP(mpPart_t& part, size_t begin, size_t size);
	void			tokenToString(const token_s& token, std::string& dest);
	std::string		tokenToString(const token_s& token);
	
	bool			parseDelimiterPairs(token_s& it, std::vector<token_s>& out, const std::vector<std::pair <char, char> >& delim_pairs, char primary_delim);
	bool			parseSingleToken( token_s& it, eParseContext context, std::string& dest);
	bool			parseNextLine(token_s& it, token_s& line);
	bool			parseQuotedString(token_s& it, std::string& out);
	bool			splitToToken(token_s& it, bool last_val, token_s& tkn, const std::string& delim);
	size_t			splitToToken(token_s& it, bool last_val, std::vector<token_s>& tkns, const std::string& delim);
	bool			splitToTokenMultiDelims(token_s& it, bool last_val, token_s& tkn, const std::string& delims, char& found_delim);

	std::string		getParsedString();

	template <class T>
	void			setIterators(token_s& t, T& begin, T& end);
	void			setIterators();

	bool			tokenCharValidation(const token_s& chars, const std::string& error, int errNum, bool (*f)(unsigned char));
	bool			tokenCharValidationStr(const std::string& str, const std::string& error, int errNum, bool (*f)(unsigned char));
	bool			rlCharsCheck(const token_s& chars, const std::string& error, int errNum);
	void			createContextMap();

	mpPart_t&		currentPart();

private:

	Request*				 _req;
	std::map< eParseContext, parseContext > _contextMap;
	std::vector<token_s>	_tknLst;
	token_s					_p_it;

	size_t					_os_body_len;
	size_t					_expected_len;
	size_t					_parsed_body_len;
	uint64_t				_chunk_size;
	bool					_chunk_parser_status;
	mpParser_t				_mp_p;
};

bool	bytesAvailable(token_s& it, size_t required);
