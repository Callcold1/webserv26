/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTypes.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:24:23 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/23 12:42:37 by kmooney          ###   ########.fr       */
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
enum	eReqState		{ P_REQLINE, P_REQLINE_PARAMS, V_REQLINE, P_HEADERS, V_HEADERS, P_BODY, V_BODY, P_TRAILERS, V_TRAILERS, REQ_DONE };
enum	eURIstates		{ SCHEME, AUTH, USER, HOST, PORT, PATH, QUERY, FRAG, URI_ERROR, URI_END };
enum	uri_types		{ ABSOLUTE_FORM, ASTERISK, AUTH_FORM, ORIGIN_FORM };
enum	eParseContext	{ ABSOLUTE_URI, ORIGIN_URI, USER_NAME, USER_PASS, QUERY_KEY, QUERY_VAL, FIELD_NAME, FIELD_VAL, PARAM_KEY, PARAM_VAL, CHUNKSIZE};

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
	const char*				delim;
	const char*				error;
	int						errNum;
	charValFUnc				validator;

	parseContext( bool lv, bool dr, const char* d, const char* e, int n, charValFUnc v )
		: last_val(lv), delim_req(dr), delim(d), error(e), errNum(n), validator(v){}

	parseContext()
		: last_val(false), delim_req(false), delim(""), error(""), errNum(0), validator(0) {}
} parseContext;

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

