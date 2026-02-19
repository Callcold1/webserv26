/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CParser.cpp                       		            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 15:18:30 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/17 12:47:36 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "CParser.hpp"

CParser::CParser( const std::string& filename ) : Parser() {
	openConfigFile(filename);
}

CParser::~CParser( void ){}

CParser::CParser(const CParser &other): Parser(other._tokens){(void)other;}

CParser& CParser::operator=(const CParser &other) {
	(void) other;
    return *this;
}

void	CParser::openConfigFile(const std::string& filename)
{
	_stream.open(filename.c_str());

	if (!_stream.is_open())
		throw std::runtime_error("Could not open configuration file.");
}

void	CParser::parseConfigFile(Config& config){

	tokeniseStream();
	populateKeywords();
	parseTokens();

	setConfig(config);
}

void	CParser::tokeniseStream(void){

	tokenContext_t context("{};", false, false);
	_tokeniser = new Tokeniser(_stream, context);
	_tokens = _tokeniser->getTokens();
}

void	CParser::parseTokens(void){
	_rawConfig = parse();
}

node_t*	CParser::parse(void) {

	node_t* root = new node_t;
	root->type = "global";

	if (_tokens[_pos].type == T_WORD)
		parseNext(root, token_t(T_EOF, ""));
	else
		std::runtime_error("Invalid config file structure");
	return root;
}

void	CParser::parseNext(node_t* parent, token_t awaited){

	while (!(_tokens[_pos].type == awaited.type && _tokens[_pos].value == awaited.value) && _tokens[_pos].type != T_EOF){

		TokenType_e type = _tokens[_pos].type;
		std::string value = _tokens[_pos].value;

		if (check(T_WORD, "server")){
			consume();
			expect(T_SYMBOL, "{");
			parent->children.push_back(addNode(value));
			parseNext(parent->children.back(), token_t(T_SYMBOL, "}"));
		}
		else if (check(T_WORD, "location")){
			parent->children.push_back(addNode(value));
			consume();
			for (size_t i = 0; i < 3; i++){				// check for modifier (i.e = or ~) and/or path : can have "location = /path" (exact match), or "location /path" or "location ~ \.cgi_type"
				token_t next = peek();					// a path must start with /, cgi with '\.ext' where ext is the file type/extension
				if (next.type == T_WORD){
					parent->children.back()->args.push_back(next.value);
					consume();
				}
				else if (next.type == T_SYMBOL && next.value == "{"){
					consume();
					break;
				}
				else
					throw std::runtime_error("Error: Invalid location data");
			}
			parseNext(parent->children.back(), token_t(T_SYMBOL, "}"));
		}
		else if (type == T_WORD && checkKeywords(parent->type, value)){
			parent->children.push_back(addNode(value));
			consume();
			while (true){
				token_t next = peek();
				if (next.type == T_WORD){
					parent->children.back()->args.push_back(next.value);
					consume();
				}
				else if (next.type == T_SYMBOL && next.value == ";"){
					consume();
					break;
				}
				else
					throw std::runtime_error("Error: Invalid token : expected ';'");
			}
		}
		else
			throw std::runtime_error("Error: Unexpected token '" + value + "' at position " + toString(_pos));
	}
	consume();
}

bool	CParser::checkKeywords(const std::string& context, const std::string& value){

	std::vector<std::string> vec;

	if (context == "server")
		vec = _server_keywords;
	else if (context == "location")
		vec = _location_keywords;
	else if (context == "global")
		vec = _global_keywords;
	if (std::find(vec.begin(), vec.end(), value) != vec.end())
		return true;
	return false;
}

node_t*	CParser::addNode(const std::string& node_type){

	node_t* n = new node_t;
	n->type = node_type;
	return n;
}

void	CParser::populateKeywords(void) {

	// LOCATION
	_location_keywords.push_back("alias");
	_location_keywords.push_back("autoindex");
	_location_keywords.push_back("cgi_path");
	_location_keywords.push_back("charsets");
	_location_keywords.push_back("index");
	_location_keywords.push_back("methods");
	_location_keywords.push_back("redirect_status");
	_location_keywords.push_back("redirect_path");
	_location_keywords.push_back("root");
	_location_keywords.push_back("upload_path");

	// SERVER
	_server_keywords.push_back("charsets");				
	_server_keywords.push_back("error_page");
	_server_keywords.push_back("listen");
	_server_keywords.push_back("max_body_size");
	_server_keywords.push_back("max_connections");
	_server_keywords.push_back("max_header_size");
	_server_keywords.push_back("methods");
	_server_keywords.push_back("root");
	_server_keywords.push_back("server_name");
	
	// GLOBAL
	_global_keywords.push_back("charsets");
	_global_keywords.push_back("error_page");
	_global_keywords.push_back("max_body_size");
	_global_keywords.push_back("max_connections");
	_global_keywords.push_back("max_header_size");
	_global_keywords.push_back("keepalive_timeout");
	_global_keywords.push_back("read_write_timeout");
}

template <typename T>
std::string toString(const T val)
{
	std::stringstream stream;
	stream << val;
	return stream.str();
}

/*	GLOBAL

Directive			Args		Multi Allowed	Type					Duplicate Behavior	Inherited	Notes
=========			====		=============	====					==================	=========	=====

server				block		yes				vector<CServer>			Append				No			No

charset				multi		yes				vector<string>			Append				Replace		Recommend replace if redefined
error_page			multi		yes				vector<errorPage_s>		Append (by code)	Merge		Code uniqueness (map : use error code as key - path can be overwritten)

max_body			1			no				long long				Overwrite			Replace
max_header_size		1			no				long long				Overwrite			Replace

max_connections		1			no				int						Overwrite			Replace		Global only
timeout_ka			1			no				int						Overwrite			Replace
timeout_rw			1			no				int						Overwrite			Replace

 */
void	CParser::setConfig(Config& config){

	std::vector<CServer>		servers;

	std::vector<std::string>	charset;
	std::vector<errorPage_s>	error_pages;

	long long					max_body;
	long long					max_header;

	int							max_conn;
	int							timeout_ka;
	int							timeout_rw;

	for (size_t i = 0; i < _rawConfig->children.size(); i++){

		node_t* node = _rawConfig->children[i];

		if (node->type == "server")
			setServer(node, servers);
		else if (node->type == "charsets")
			setCharsets(node, charset);
		else if (node->type == "error_page")
			setErrorPage(node, error_pages);
		else if (node->type == "location")
			throw std::runtime_error("Error : a location can only exist within a server block");
		else if (node->type == "max_body_size")
			setNumericValue<long long>(node, max_body, 0, SYS_MAX_BODY);
		else if (node->type == "max_connections")
			setNumericValue<int>(node, max_conn, 1, SYS_MAX_CONN);
		else if (node->type == "max_header_size")
			setNumericValue<long long>(node, max_header, 1, SYS_MAX_HEAD);
		else if (node->type == "keepalive_timeout")
			setNumericValue<int>(node, timeout_ka, 1, SYS_TM_KA);
		else if (node->type == "read_write_timeout")
			setNumericValue<int>(node, timeout_rw, 1, SYS_TM_RW);
	}
	config.setServers(servers);
	config.setCharsets(charset);
	config.setErrorPages(error_pages);
	config.setMaxBodySize(max_body);
	config.setMaxHeaderSize(max_header);
	config.setMaxConnections(max_conn);
	config.setTimeoutKA(timeout_ka);
	config.setTimeoutRW(timeout_rw);
}

/* SERVER

Directive			Args		Multi Allowed	Type				Duplicate Behavior	Inherited from Global	Inherited to Location	Notes
=========			====		=============	====				==================	=====================	=====================	=====
locations			block		yes				vector<Clocation>	Append				No						No

charset				multi		yes				vector<string>		Append				Replace					Replace
error_page			multi		yes				vector<errorPage_s>	Append				Merge					Merge
listen				multi		yes				vector<listen_s>	Append				No						No						Required
methods				multi		yes				vector<string>		Append				Replace					Replace
server_name			multi		yes				vector<string>		Append				No						No

max_body			1			no				long long			Overwrite			Replace					Replace
max_header_size		1			no				long long			Overwrite			Replace					Replace

root				1			no				std::string			Overwrite			Replace					Replace

*/

void	CParser::setServer(node_t* server_node, std::vector<CServer>& servers){

	CServer 					server;

	std::vector<CLocation>		locations;

	std::vector<std::string>	charsets;
	std::vector<errorPage_s>	error_pages;
	std::vector<listen_s>		listen;
	std::vector<std::string>	methods;
	std::vector<std::string>	server_names;

	long long					max_body;
	long long					max_header;

	std::string					root;

	for (size_t i = 0; i < server_node->children.size(); i++){

		node_t* node = server_node->children[i];

		if (node->type == "server")
			throw std::runtime_error("Error : server blocks cannot contain server blocks");
		else if (node->type == "location")
			setLocation(node, locations);
		else if (node->type == "error_page")
			setErrorPage(node, error_pages);
		else if (node->type == "listen")
			setPorts(node, listen);
		else if (node->type == "max_body_size")
			setNumericValue<long long>(node, max_body, 0, SYS_MAX_BODY);
		else if (node->type == "max_header_size")
			setNumericValue<long long>(node, max_header, 1, SYS_MAX_HEAD);
		else if (node->type == "methods")
			setMethods(node, methods);
		else if (node->type == "root")
			setPath(node, root);
		else if (node->type == "server_name")
			setServerNames(node, server_names);
	}
	server.setCharsets(charsets);
	server.setErrorPages(error_pages);
	server.setLocations(locations);
	server.setListen(listen);
	server.setMethods(methods);
	server.setNames(server_names);
	server.setMaxBodySize(max_body);
	server.setMaxHeaderSize(max_header);
	server.setRootDir(root);

	servers.push_back(server);
}

/* LOCATION

Directive			Args		Multi Allowed	Type						Duplicate Behavior	Inherited	Notes
=========			====		=============	====						==================	=========	=====

cgi_path			multi		yes				map<string, std::string>	Append (by ext)		Merge		ext uniqueness
charset				multi		yes				vector<string>				Append				Replace
index				multi		yes				std::vector<string>			Append				Replace
methods				multi		yes				std::vector<string>			Append				Replace
try_files			multi		yes				TryFiles_s					Overwrite			No			Only one try_files directive recommended

alias				1			no				string 						Overwrite			No			Overrides root
autoindex			1			no				bool						Overwrite			Replace		bool
modifier			1			no				LocationMatchType_e			Overwrite			No			Should use enum
redirect_status		1			no				redirect_s					Overwrite			No			Should combine with URL
redirect_url		1			no				redirect_s					Overwrite			No			Combine into struct
root				1			no				string						Overwrite			Replace
upload_path			1			no				string						Overwrite			Replace
uri					1			no				string						Overwrite			No			Combine with root (if no alias) and check accessibility

*/
void	CParser::setLocation(node_t* location_node, std::vector<CLocation>& locations){

	CLocation 							location;
	
	LocationMatchType_e					modifier;
	std::string							uri;

	std::map<std::string, std::string>	cgiMap;
	std::vector<std::string>			charsets;
	std::vector<std::string>			index;			//Created using root + uri (+ index args)
	std::vector<std::string>			methods;

	std::string							alias;
	bool								autoindex;
	redirect_s							redirect;
	bool								redirect_status;
	std::string							redirect_url;
	std::string							root;			// Can't have both root and alias
	std::string							upload_path;	

	if (location_node->args.size() == 1)
		uri = location_node->args[0];
	else if (location_node->args.size() == 2){
		setModifier(location_node->args[0], modifier);
		uri = location_node->args[1];
	}

	for (size_t i = 0; i < location_node->children.size(); i++){

		node_t* node = location_node->children[i];

		if (node->type == "location")
			throw std::runtime_error("Error : location blocks cannot location blocks");
		else if (node->type == "alias")
			setPath(node, alias);
		else if (node->type == "autoindex")
			setBoolean(node, autoindex);
		else if (node->type == "cgi_path")
			setCGImap(node, cgiMap);
		else if (node->type == "charset")
			setCharsets(node, charsets);
		else if (node->type == "index")
			setIndex(node, index);
		else if (node->type == "methods")
			setMethods(node, methods);
		else if (node->type == "redirect_status")
			setBoolean(node, redirect_status);
		else if (node->type == "redirect_path")
			setPath(node, redirect_url);
		else if (node->type == "root")
			setPath(node, root);
		else if (node->type == "upload_path")
			setPath(node, upload_path);
	}
	if (checkAliasRoot(alias, root))
		throw std::runtime_error("Error: location contains both alias and root directives");

	redirect.status = redirect_status;
	redirect.url = redirect_url;
	
	location.setAlias(alias);
	location.setCGIMap(cgiMap);
	location.setCharsets(charsets);
	location.setIndex(index);
	location.setMethods(methods);
	location.setAutoIndex(autoindex);
	location.setModifier(modifier);
	location.setRedirect(redirect);
	location.setRootDir(root);
	location.setUploadPath(upload_path);
	location.setURI(uri);

	locations.push_back(location);
}

void	CParser::setBoolean(node_t* node, bool& state){

	if (!isOnlyArg(node->args))
		throw std::runtime_error("Error: multiple arguments when only one permitted : " + node->args[0] + " " + createArgsString(node->args));

	std::string lowercase = to_lower(node->args[0]);
	
	if ( lowercase == "true")
		state = true;
	else if (lowercase == "false")
		state = false;
	else if (lowercase.length() == 1){
		if ( lowercase == "1")
			state = true;
		else if (lowercase == "0")
			state = false;
	}
}

/*
	CGI Handling

	CGI path must point to an executable file (that handles given cgi type(s))
	If not, then should throw error (if strict) or warn (don't add to map)

	If multiple cgi extensions provided with same
*/
void	CParser::setCGImap(node_t* node, std::map<std::string, std::string>& cgiMap){

	size_t size = node->args.size();

	if (size <= 1)
		throw std::runtime_error("Error: invalid cgi_path : must contain path and at least one extension type : " + node->type + " " + createArgsString(node->args));
	std::string path = node->args[0];
	std::string ext;
	for (size_t i = 1; i < size; i++){
		if (node->args[i][0] == '.') // consider anything starting '.' a valid cgi type
			cgiMap[node->args[i]] = path;
		else
			throw std::runtime_error("Error: invalid cgi_path : " + node->type + " " + createArgsString(node->args));
	}
}

void	CParser::setCharsets(node_t* node, std::vector<std::string>& charset){

	size_t size = node->args.size();

	for (size_t i = 0; i < size; i++){
		if (isValidCharset(node->args[i]))
			charset.push_back(node->args[i]);
		else
			throw std::runtime_error("Error: invalid method"); // change to log entry for config
	}
}

void	CParser::setErrorPage(node_t* node, std::vector<errorPage_s>& error_pages){

	size_t size = node->args.size();
	int error_num;
	std::string path;

	if (convertToType(path, node->args[size - 1])){

		// possibly create an array/vector of valid/supported error numbers to use for validation
		for (size_t i = 0; i < size - 1; i++){
			if (convertToType(error_num, node->args[i]) && error_num >= 400 && error_num < 600){
				errorPage_s error(error_num, path);
				error_pages.push_back(error);
			}
			else
				throw std::runtime_error("Error: invalid error_page number"); // change to log entry for config
		}
	}
	else
		throw std::runtime_error("Error: invalid path");
}

void	CParser::setIndex(node_t* node, std::vector<std::string>& index){

	size_t size = node->args.size();

	for (size_t i = 0; i < size - 1; i++)
			index.push_back(node->args[i]);
}

void	CParser::setModifier(std::string& str, LocationMatchType_e& modifier){
	if (str.length() != 1)
		throw std::runtime_error("Error: invalid modifier : " + str);

	if (str[0] == '=')
		modifier = EXACT;		// http uri must match location.uri exactly 
	else if (str[0] == '^')
		modifier = PREFIX; 		// if uri matches, return straight away without checking other locations (https://chatgpt.com/share/6995da46-ff54-800f-b363-7aa166db7e2e)
	else				
		throw std::runtime_error("Error: invalid modifier : " + str);
}

void	CParser::setMethods(node_t* node, std::vector<std::string>& methods){

	size_t size = node->args.size();

	for (size_t i = 0; i < size - 1; i++){
		if (isValidMethod(node->args[i]))
			methods.push_back(node->args[i]);
		else
			throw std::runtime_error("Error: invalid method"); // change to log entry for config
	}
}

void	CParser::setServerNames(node_t* node, std::vector<std::string>& names){

	size_t size = node->args.size();

	for (size_t i = 0; i < size - 1; i++)
			names.push_back(node->args[i]);
}

void	CParser::setPorts(node_t* node, std::vector<listen_s>& ports){

	size_t size = node->args.size();
	bool def = false;

	if (node->args[size - 1] == "default_server")
		def = true;

	for (size_t i = 0; i < size; i++){

		if (def == true && i == (size - 1))
			break;

		uint32_t ip;
		uint16_t port_num;
		splitPortInfo(node->args[i], ip, port_num);

		listen_s port(ip, def, port_num);
		ports.push_back(port);
	}
}

void	CParser::setPath(node_t* node, std::string& path){

	if (!isOnlyArg(node->args))
		throw std::runtime_error("Error: multiple arguments when only one permitted : " + node->args[0] + " " + createArgsString(node->args));
	path = node->args[0];
}

template <typename T>
void	CParser::setNumericValue(node_t* node, T& target, T min, T max){

	T val;

	if (isOnlyArg(node->args) && convertToType(val, node->args[0])){
		if (val <= max && val >= min)
			target = val;
		else
			throw std::runtime_error("Error: value out of range : ");
	}
	else
		throw std::runtime_error("Error: value not of expected type : ");
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
>>>>>>		TYPE VALIDATION		<<<<<<
>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<
*/

bool	CParser::isValidCharset(const std::string& str){
	if (str.compare("utf-8") == 0 ||
		str.compare("iso-8859-1") == 0)
		return true;
	else
		return false;
}

bool	CParser::isValidMethod(const std::string& str){
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

bool	CParser::checkAliasRoot(const std::string& alias, const std::string& root){
	return (alias.length() > 0 && root.length() > 0);
}
