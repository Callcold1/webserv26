/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 22:01:11 by kmooney           #+#    #+#             */
/*   Updated: 2025/06/23 16:14:40 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core_def.hpp"
#include "utility_functions.hpp"
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

	void			parseReqLine(token_t& token);
	bool			getRequestLine(token_t& rl, token_t& it);
	
	bool			parseURI( token_t& rl );
	eURIstates&		parseScheme( eURIstates& state, token_t& uri );
	eURIstates&		parseAuth( eURIstates& state, token_t& uri );
	eURIstates&		parseUser( eURIstates& state, token_t& uri );
	eURIstates&		parseHost( eURIstates& state, token_t& uri );
	eURIstates&		parsePort( eURIstates& state, token_t& auth );
	eURIstates&		parsePath( eURIstates& state, token_t& uri );
	eURIstates&		parseQuery( eURIstates& state, token_t& uri );
	eURIstates&		parseFrag( eURIstates& state, token_t& uri ); 
	bool			URInormalisation( void );

	void			parseField(token_t& it);
	bool			parseFieldLine(token_t& line);
	bool			parseFieldVal(token_t& value_tkn, field_t* field);

	void			parseBody(token_t& it);
	void			parsePlain(token_t& it);
	bool			parseChunkData(token_t& it, std::string& data);
	bool			parseChunkSize(token_t& it);
	void			finalChunkChecks(token_t& it);
	void			parseChunked(token_t& it);
	void			parseMultiPart(token_t& it);

	void			tokenToString(const token_t& token, std::string& dest);
	std::string		tokenToString(const token_t& token);

	//bool			parseSingleToken(token_t& it, bool last_val, std::string& dest, const std::string& delim, bool delim_req);
	bool			parseSingleToken( token_t& it, eParseContext context, std::string& dest);
	bool			parseNextLine(token_t& it, token_t& line);

	size_t			splitToToken(token_t& it, bool last_val, std::vector<token_t>& tkns, const std::string& delim);
	bool			splitToToken(token_t& it, bool last_val, token_t& tkn, const std::string& delim);
	bool			splitToTokenMultiDelims(token_t& it, bool last_val, token_t& tkn, const std::string& delims, char& found_delim);

	std::string		getParsedString();
	
	template <class T>
	void			setIterators(token_t& t, T& begin, T& end);
	void			setIterators();

	bool			tokenCharValidation(const token_t& chars, const std::string& error, int errNum, bool (*f)(unsigned char));
	bool			rlCharsCheck(const token_t& chars, const std::string& error, int errNum);
	void			createContextMap();
	
private:
	
	Request*				 _req;
	std::map< eParseContext, parseContext > _contextMap;
	std::vector<token_t>	_tknLst;
	token_t					_p_it;
	
	size_t					_unparsed_len;
	size_t					_expected_len;
	size_t					_parsed_body_len;
	uint64_t				_chunk_size;
	bool					_chunk_parser_status;
};

bool	bytesAvailable(token_t& it, size_t required); 
