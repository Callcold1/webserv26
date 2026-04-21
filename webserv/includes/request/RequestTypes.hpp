/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTypes.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:24:23 by kmooney           #+#    #+#             */
/*   Updated: 2026/04/20 14:51:15 by kmooney          ###   ########.fr       */
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

class	CLocation;
class	CServer;

enum	eParse 			{ P_PARSE, P_AWAIT, P_DONE, P_ERROR };
enum	eReqState		{ P_REQLINE, P_REQLINE_PARAMS, V_REQLINE, P_HEADERS, V_HEADERS, P_BODY, V_BODY, P_TRAILERS, V_TRAILERS, P_MULTI, V_MPHEADERS, V_MPBODY, REQ_DONE };
enum	eURIstates		{ SCHEME, AUTH, USER, HOST, PORT, PATH, QUERY, FRAG, URI_ERROR, URI_END };
enum	uri_types		{ ABSOLUTE_FORM, ASTERISK, AUTH_FORM, ORIGIN_FORM };
enum	mpState			{ MP_NONE, MP_BOUNDARY, MP_HEADERS, MP_BODY, MP_CHUNKED, MP_FINAL, MP_COMPLETE, MP_ERROR };
enum	eParseContext	{ ABSOLUTE_URI, ORIGIN_URI, USER_NAME, USER_PASS, QUERY_KEY, QUERY_VAL, FIELD_NAME, FIELD_VAL, PARAM_KEY, PARAM_VAL, CHUNKSIZE, BOUNDARY };
enum	eResponseType	{ RESPOND_NOT_SET, RESPOND_CGI, RESPOND_ERROR, RESPOND_FILE, RESPOND_INDEX, RESPOND_MP_UP, RESPOND_REDIR, RESPOND_STATIC, RESPOND_UPLOAD };
enum	eBodyType 		{ BODY_NONE, BODY_CHUNKED, BODY_EOF, BODY_CONTENT_LEN };

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>			 				STRUCTS		 					  		   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/* TOKEN : CONTAINS THE */
struct token_s {

	size_t	begin;
	size_t	end;

	token_s():begin(0), end(0) {}
	token_s(size_t b, size_t e)
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

	void reset(void) {
		// uri_type;	Need a default state : URI_TYPE_NOT_SET?
		len = 0; port_int = 80;
		auth.clear(); frag.clear();	host.clear(); 
		pass.clear(); path.clear(); port.clear();
		query.clear(); scheme.clear(); str.clear();
		target.clear();	user.clear(); queries.clear();
	}
} uri_t;

typedef bool (*charValFUnc)(unsigned char);

typedef struct parseContext {
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
	fieldMap_t*											headers; 				// headers are parsed into here before being validated and moved to the relevant field
	std::string											name;	 				// must be present
	std::string											filename; 				// if name=file, filename or filename* must be present
	std::string											disposition_type;		// header must be present (this stores value)
	std::vector<std::pair <std::string, std::string> >	disposition_parameters; // parameters from header
	std::vector<field_t*>								supported_content;
	std::vector<char>									body;					// used if no file upload

	size_t												body_size;				// bytes that have been added to body file or body 
	bool												isFile;					// set to true if filename is present
	std::string											tempFilePath;			// created if filename is present and body is appended here each read cycle
	fileInfo_s											upload_dest;			// populated when validating content-disposition header
	int													fd; 					// might not be required as fstream is used to append data
	mpPart_t(): headers(new fieldMap_t), body_size(0), isFile(0), fd(-1){}
};

struct mpMessage_t {
	std::vector<mpPart_t>	parts;
	std::string				default_charset;
	size_t					filecount;
	mpMessage_t(): filecount(0){}
};

struct mpParser_t {
	mpState				state;

	std::string			boundary;
	std::string			boundary_marker;
	size_t				current_index;
	size_t				boundary_match_index;

	mpParser_t(): state(MP_NONE), current_index(0), boundary_match_index(0) {}
};

struct	parsedSizes_s{
	size_t	bytes_received;			// Number of bytes read from server (should equal buffer size)
	size_t	bytes_read;				// Number of bytes read from buffer (i.e. total raw message size that has been processed) - every char counted
	size_t	bytes_unread;			// Number of unread bytes in buffer
	size_t	body_bytes_read;		// Total length of body parsed 			- every char counted
	size_t	body_bytes_expected;	// Set to Content-Length value once header has been validated 
	size_t	body_size;				// Size of body							- size of body after parsing (doesn't include removed chars e.g. \r\n) - updated for all types (chunked, mp, other) and body saved to file
	size_t	body_local_size;		// Size of body	string 					- used to determine if file required due to size
	size_t	chunked_bytes;			// Size of chunked body					- only characters that appear in body // Maybe not needed (body size could cover this)

	parsedSizes_s():
		bytes_received(0),
		bytes_read(0),
		bytes_unread(0),
		body_bytes_read(0),
		body_bytes_expected(0),
		body_size(0),
		body_local_size(0),
		chunked_bytes(0){}

	void reset(size_t remainder) {
		bytes_received = 0;
		bytes_unread = remainder;
		bytes_read = 0;
		body_bytes_read = 0;
		body_bytes_expected = 0;
		body_size = 0;
		body_local_size = 0;
		chunked_bytes = 0;
	}
};

struct reqPaths_s {
	fileInfo_s											 error_path;
	std::string											 redirect_url;
    fileInfo_s											 resource_path;
	std::string											 upload_base;
    std::vector< std::pair < fileInfo_s, std::string > > upload_paths; //tmpfile path, upload path - can move from tmp to upload path
};

struct respond_t {
	eResponseType				type;			// Response Type
	int							status;			// Response Code
	std::string					message;		// Response Message
	const CLocation*			location;		// Matched Location
	CServer*					server;			// Matched Server
	reqPaths_s					paths;			// Response Paths
	std::string					uri_path;		// URI path (parsed from request URI and normalised)
	std::string					leftover_path;	// URI path leftover after location matching
	bool						keep_alive;		// Connection Preference
	std::vector<std::string> 	accept_type; 	// Accept types (content-type) in order of preference (Matched with config mime_types)
	query_t						query_pairs;	// Query string KV pairs
	std::string					cgi_type;		// If CGI, then the file ending ".type" appears here
	bool						body_in_file;	// Set to true if body is saved in a file (will appear in _re.paths.upload_paths)
	respond_t():type(RESPOND_NOT_SET), status(0), keep_alive(0), body_in_file(0){}

	void reset(void){
		type = RESPOND_NOT_SET;
		status = 0;
		message.clear();
		location = NULL;
		server = NULL;
		paths.error_path.reset();
		accept_type.clear();
		leftover_path.clear();
		paths.error_path.reset();
		paths.redirect_url.clear();
		paths.resource_path.reset();
		paths.upload_base.clear();
		paths.upload_paths.clear();
		query_pairs.clear();
		uri_path.clear();
		leftover_path.clear();
		keep_alive = false;
		accept_type.clear();
		query_pairs.clear();
		body_in_file = 0;
	}
};
