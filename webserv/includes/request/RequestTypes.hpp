/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTypes.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:24:23 by kmooney           #+#    #+#             */
/*   Updated: 2026/03/03 10:50:53 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core_def.hpp"
#include <string>
#include <vector>
#include <map>

#define DELIM_REQUIRED true
#define DELIM_OPTIONAL false
#define HAS_BODY true
#define NO_BODY false
#define IS_CHUNKED true
#define NOT_CHUNKED false

enum	eParse 			{ P_PARSE, P_AWAIT, P_DONE, P_ERROR };
enum	eReqState		{ P_REQLINE, P_REQLINE_PARAMS, V_REQLINE, P_HEADERS, V_HEADERS, P_BODY, V_BODY, P_TRAILERS, V_TRAILERS, P_MULTI, V_MULTI, REQ_DONE };
enum	eURIstates		{ SCHEME, AUTH, USER, HOST, PORT, PATH, QUERY, FRAG, URI_ERROR, URI_END };
enum	uri_types		{ ABSOLUTE_FORM, ASTERISK, AUTH_FORM, ORIGIN_FORM };
enum	eBodyStates		{ BODY_CHUNKED, BODY_MULTIPART };
enum	mpState			{ MP_NONE, MP_BOUNDARY, MP_HEADERS, MP_BODY, MP_CHUNKED, MP_COMPLETE, MP_ERROR };
enum	eParseContext	{ ABSOLUTE_URI, ORIGIN_URI, USER_NAME, USER_PASS, QUERY_KEY, QUERY_VAL, FIELD_NAME, FIELD_VAL, PARAM_KEY, PARAM_VAL, CHUNKSIZE, BOUNDARY };
enum	eResponseType	{ RESPOND_NOT_SET, RESPOND_CGI, RESPOND_ERROR, RESPOND_FILE, RESPOND_INDEX, RESPOND_REDIR };


struct respond_t{
	eResponseType	type;
	std::string		request_path;		// URI FROM REQUEST request->uri.path - TMP
	std::string		normalised_path;	// NORMALISED URI FROM REQUEST request->uri.path - TMP
	std::string		leftover_path;		// PATH REMAINING AFTER LOCATION MATCHING - TMP
	std::string		location_path;		// THE MATCHING LOCATION URI - TMP
	std::string		location_root;		// ROOT FOR MATCHING LOCATION - TMP
	std::string		appended_root;		// LEFTOVER APPENDED TO ROOT - KEEP
	fileInfo_s		target;				// FILEINFO ON APPENDED ROOT

	respond_t():type(RESPOND_NOT_SET){}
};


/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>			 				STRUCTS		 					  		   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/* TOKEN : CONTAINS THE */
struct token_s {

	std::deque<char>::const_iterator	begin;
	std::deque<char>::const_iterator	end;

	token_s() {}
	token_s(std::deque<char>::const_iterator b, std::deque<char>::const_iterator e)
		: begin(b), end(e) {}

};

/* 

	Used as part of fieldMap_t (below) 

	fieldMap_t	< string (header_name), vector< field_t *> >
	field_t 	< string (header_value), vector< pair<string (key), string (value)> > parameters

	Each header is parsed into the headers object which is a fieldMap_t and accessed using 
	the header_name. 

	The name is mapped to a vector of field_t objects.

	When a new instance of a header name is encountered, or if there are multiple
	values in the parsed line, the value and any parameters are parsed 
	into a field_t struct and pushed to the back of the vector mapped to the given header name.
*/
typedef struct {
	std::string val;
	std::vector<std::pair<std::string, std::string> >params;
} field_t;

typedef std::map<std::string, std::vector<field_t* > > fieldMap_t;
typedef std::vector< std::pair<std::string, std::string> > query_t;

typedef struct uri_s {
	enum uri_types			uri_type;
	size_t					len;
	int						port_int;
	str_t 					auth, frag, host, pass, path, port, query, scheme, str, target, user;
	query_t					queries;

	uri_s()
		: uri_type(ORIGIN_FORM), len(0), port_int(80), auth(""), frag(""), host(""),
			pass(""), path(""), port(""), query(""), scheme(""), str(""), user(""){}
} uri_t;

typedef bool (*charValFUnc)(unsigned char);

typedef struct parseContext{
	bool					last_val;
	bool					delim_req;
	std::string				delim;
	std::string				error;
	int						errNum;
	charValFUnc				validator;

	parseContext( bool lv, bool dr, const char* d, const char* e, int n, charValFUnc v )
		: last_val(lv), delim_req(dr), delim(d), error(e), errNum(n), validator(v){}

	parseContext()
		: last_val(false), delim_req(false), delim(""), error(""), errNum(0), validator(0) {}
} parseContext;

struct mpMessage_t;

struct mpPart_t {
	fieldMap_t*											headers; // headers are parsed into here before being validated and moved to the relevant field
	std::string											name;
	std::string											filename;
	std::string											disposition_type;
	std::vector<std::pair <std::string, std::string> >	disposition_parameters;
	std::vector<field_t*>								supported_content;
	std::vector<char>									body;

	bool												isFile;
	std::string											tempFilePath;
	int													fd;
	mpState												parse_status;
};

struct mpMessage_t {
	std::vector<mpPart_t>	parts;
	std::string 			boundary;
	std::string				default_charset;
};

struct mpParser_t {
	mpState				state;

	std::string			boundary;
	std::string			boundary_marker;

	size_t				current_index;

	token_s				header_buffer;
	std::vector<char>	body_buffer;

	size_t				boundary_match_index;
};
/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					ITERATORS FUNCTORS								   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

// struct HeaderValueContains {

// 	std::string str;

//     HeaderValueContains(const std::string& target) : str(target) {}

//     bool operator()(const std::string& key, std::string target) {
//         return field && field->val.compare(str);
//     }
// };


// /*
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// >>>>>>					ITERATORS TEMPLATE 								   <<<<<<
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// */

// template <typename Func>
// bool iterateHeaderKeys(headerMap_t& headers, Func func) {
// 	for (headerMap_t::iterator it = headers.begin(); it != headers.end(); it++) {
// 		if (!func)
// 			return true;
// 		else if func(it->first)
// 			return true;
// 	}
// 	return false;
// }

// template <typename Func>
// bool iterateHeaderFields(headerMap_t& headers, Func func) {

// 	for (headerMap_t::iterator it = headers.begin(); it != headers.end(); it++) {
// 		std::vector<field_t*>& vec = it->second;
// 		for (std::vector<field_t*>::iterator vec_it = vec.begin(); vec_it != vec.end(); vec_it++) {
// 			if (func(it->first, *vec_it)) // func(key, field_t*)
// 				return true;
// 		}
// 	}
// 	return false;
// }

// template <typename Func>
// bool iterateHeaderParams(headerMap_t& headers, Func func) {
// 	for (headerMap_t::iterator it = headers.begin(); it != headers.end(); it++) {
// 		std::vector<field_t*>& vec = it->second;
// 		for (std::vector<field_t*>::iterator vec_it = vec.begin(); vec_it != vec.end(); vec_it++) {
// 			if (!*vec_it) continue;
// 			std::map<std::string, std::string>& params = (*vec_it)->params;
// 			for (std::map<std::string, std::string>::iterator p_it = params.begin(); p_it != params.end(); p_it++) {
// 				if (func(it->first, (*vec_it), p_it->first, p_it->second)) // (key, field, paramKey, paramVal)
// 					return true;
// 			}
// 		}
// 	}
// 	return false;
// }

// /*	 headerMap_t iterator ( return type void )	*/
// template <typename Func>
// void	forEachHeader(headerMap_t& headers, Func func) {

// 	for (headerMap_t::iterator it = headers.begin(); it != headers.end(); it++) {

// 		std::vector<field_t*>& vec = it->second;

// 		for (std::vector<field_t*>::iterator vec_it = vec.begin(); vec_it != vec.end(); vec_it++) {
// 			func(it->first, *vec_it);
// 		}
// 	}
// }

