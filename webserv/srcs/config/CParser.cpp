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

CParser::~CParser( void ){
	delete _tokeniser;
}

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
	initKeywords();
	parseTokens();

	setConfig(config);
	delete _rawConfig;
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
		parseNext(root, tokeniser_s(T_EOF, ""));
	else
		std::runtime_error("Invalid config file structure");
	return root;
}

void	CParser::parseNext(node_t* parent, tokeniser_s awaited){

	while (!(_tokens[_pos].type == awaited.type && _tokens[_pos].value == awaited.value) && _tokens[_pos].type != T_EOF){

		TokenType_e type = _tokens[_pos].type;
		std::string value = _tokens[_pos].value;

		if (check(T_WORD, "server")){
			consume();
			expect(T_SYMBOL, "{");
			parent->children.push_back(addNode(value));
			parseNext(parent->children.back(), tokeniser_s(T_SYMBOL, "}"));
		}
		else if (check(T_WORD, "location")){
			parent->children.push_back(addNode(value));
			consume();
			for (size_t i = 0; i < 3; i++){				// check for modifier (i.e = or ~) and/or path : can have "location = /path" (exact match), or "location /path" or "location ~ \.cgi_type"
				tokeniser_s next = peek();					// a path must start with /, cgi with '\.ext' where ext is the file type/extension
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
			parseNext(parent->children.back(), tokeniser_s(T_SYMBOL, "}"));
		}
		else if (type == T_WORD && checkKeywords(parent->type, value)){
			parent->children.push_back(addNode(value));
			consume();
			while (true){
				tokeniser_s next = peek();
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

void	CParser::initKeywords(void) {

	// LOCATION
	_location_keywords.push_back("alias");
	_location_keywords.push_back("autoindex");
	_location_keywords.push_back("cgi_path");
	_location_keywords.push_back("charset");
	_location_keywords.push_back("index");
	_location_keywords.push_back("max_body_size");
	_location_keywords.push_back("method");
	_location_keywords.push_back("redirect_status");
	_location_keywords.push_back("redirect_path");
	_location_keywords.push_back("root");
	_location_keywords.push_back("try_files");
	_location_keywords.push_back("upload_path");

	// SERVER
	_server_keywords.push_back("autoindex");
	_server_keywords.push_back("charset");
	_server_keywords.push_back("error_page");
	_server_keywords.push_back("index");
	_server_keywords.push_back("listen");
	_server_keywords.push_back("max_body_size");
	_server_keywords.push_back("max_header_size");
	_server_keywords.push_back("method");
	_server_keywords.push_back("root");
	_server_keywords.push_back("server_name");
	_server_keywords.push_back("keepalive_timeout");
	_server_keywords.push_back("read_write_timeout");

	// GLOBAL
	_global_keywords.push_back("autoindex");
	_global_keywords.push_back("charset");
	_global_keywords.push_back("error_page");
	_global_keywords.push_back("index");
	_global_keywords.push_back("max_body_size");
	_global_keywords.push_back("max_connections");
	_global_keywords.push_back("max_header_size");
	_global_keywords.push_back("method");
	_global_keywords.push_back("root");
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

Directive			Args		Multi Allowed	Type					Duplicate Behavior	Inherited				Context(s)
=========			====		=============	====					==================	=========				==========
autoindex			1			no				bool					Overwrite			Replace if redefined	Global/CServer/CLocation
charset				multi		yes				vector<string>			Append				Replace if redefined	Global/CServer/CLocation
error_page			multi		yes				vector<errorPage_s>		Append 				Merge					Global/CServer/CLocation
index				multi		yes				std::vector<string>		Append				Replace if redefined	Global/CServer/Location
max_body			1			no				long long				Overwrite			Replace if redefined	Global/CServer/CLocation
max_connections		1			no				int						Overwrite			n/a						Global only
max_header_size		1			no				long long				Overwrite			Replace if redefined	Global/CServer
method				multi		yes				vector<string>			Append				Replace if redefined	Global/CServer/CLocation
root				1			no				std::string				Overwrite			Replace if redefined	Global/CServer/CLocation
server				block		yes				vector<CServer>			Append				n/a						Global only
timeout_ka			1			no				int						Overwrite			Replace if redefined	Global/CServer
timeout_rw			1			no				int						Overwrite			Replace if redefined	Global/CServer

*/
void	CParser::setConfig(Config& config){

	//std::vector<CServer>	servers;
	tempConfig_s			tmp;

	for (size_t i = 0; i < _rawConfig->children.size(); i++){

		node_t* node = _rawConfig->children[i];

		if (node->type == "server"){
			tmp.servers_set = setServer(node, tmp.servers);
			tmp.servers_set = true;
		}
		else
			setVariable(tmp, node, GLOBAL_HANDLER);
	}
	if (!tmp.servers_set)
		throw std::runtime_error("Error : configuration file must contain at least one 'server' block ");
	pushToConfig(tmp, config);
}

/* SERVER

Directive			Args		Multi Allowed	Type				Duplicate Behavior	Inherited from Global	Inherited to Location	Context(s)
=========			====		=============	====				==================	=====================	=====================	===========
autoindex			1			no				bool				Overwrite			Replace if redefined	Replace if redefined	Global/CServer/CLocation
charset				multi		yes				vector<string>		Append				Replace if redefined	Replace if redefined	Global/CServer/CLocation
error_page			multi		yes				vector<errorPage_s>	Append				Merge					Merge					Global/CServer/Location
index				multi		yes				std::vector<string>	Append				Replace if redefined	Replace if redefined	Global/CServer/Location
listen				multi		yes				vector<listen_s>	Append				No						No						CServer only
locations			block		yes				vector<Clocation>	Append				No						No						CServer only
max_body			1			no				long long			Overwrite			Replace if redefined	Replace if redefined	Global/CServer/CLocation
max_header_size		1			no				long long			Overwrite			Replace if redefined	Replace if redefined	Global/CServer
method				multi		yes				vector<string>		Append				Replace if redefined	Replace if redefined	Global/CServer/CLocation
root				1			no				std::string			Overwrite			Replace if redefined	Replace if redefined	Global/CServer/CLocation
server_names		multi		yes				vector<string>		Append				No						No						CServer only
timeout_ka			1			no				int					Overwrite			Replace if redefined	No						Global/CServer
timeout_rw			1			no				int					Overwrite			Replace if redefined	No						Global/CServer
*/

bool	CParser::setServer(node_t* server_node, std::vector<CServer>& servers){

	tempConfig_s tmp;

	for (size_t i = 0; i < server_node->children.size(); i++){

		node_t* node = server_node->children[i];

		if (node->type == "server")
			throw std::runtime_error("Error : server blocks cannot contain server blocks");
		else if (node->type == "location"){
			tmp.servers_set = setLocation(node, tmp.locations);
			tmp.location_set = true;
		}
		else
			setVariable(tmp, node, SERVER_HANDLER);
	}
	if (!tmp.listen_set)
		throw std::runtime_error("Error : server must contain at least one 'Listen' directive ");
	if (!tmp.location_set)
		throw std::runtime_error("Error : server must contain at least one 'location' block ");
	CServer server;

	pushToServer(tmp, server);
	servers.push_back(server);

	return true;
}

/* LOCATION

Directive			Args		Multi Allowed	Type						Duplicate Behavior	Inherited				Context(s)						Notes
=========			====		=============	====						==================	=========				===========						=====

alias				1			no				string 						Overwrite			n/a						Location only					Overrides root
autoindex			1			no				bool						Overwrite			Replace if redefined	Global/CServer/CLocation
cgi_path			multi		yes				map<string, string>			Append				n/a						Location only
charset				multi		yes				vector<string>				Append				Replace if redefined	Global/CServer/CLocation
error_page			multi		yes				vector<errorPage_s>	Append	Merge				Merge					Global/CServer/Location
index				multi		yes				vector<string>				Append				Replace if redefined	Global/CServer/CLocation
max_body			1			no				long long					Overwrite			Replace if redefined	Global/CServer/CLocation
method				multi		yes				vector<string>				Append				Replace if redefined	Global/CServer/CLocation
modifier			1			no				LocationMatchType_e			Overwrite			n/a						Location only
redirect_status		1			no				redirect_s					Overwrite			n/a						Location only					Combined into redirect_s
redirect_url		1			no				redirect_s					Overwrite			n/a						Location only					Combined into redirect_s
root				1			no				string						Overwrite			Replace if redefined	Global/CServer/CLocation
try_files			multi		yes				TryFiles_s					Overwrite			n/a						Location only
upload_path			1			no				string						Overwrite			n/a						Location only
uri					1			no				string						Overwrite			n/a						Location only


*/

bool	CParser::setLocation(node_t* location_node, std::vector<CLocation>& locations){

	tempConfig_s	temp;

	if (location_node->args.size() == 1){
		temp.uri = location_node->args[0];
		temp.uri_set = true;
	}
	else if (location_node->args.size() == 2){
		setModifier(location_node->args[0], temp.modifier);
		temp.uri = location_node->args[1];
		temp.uri_set = true;
	}
	else
		throw std::runtime_error("Invalid number of arguments provided for location uri");

	for (size_t i = 0; i < location_node->children.size(); i++){

		node_t* node = location_node->children[i];

		setVariable(temp, node, LOCATION_HANDLER);
	}
	if (checkAliasRoot(temp.alias, temp.root))
		throw std::runtime_error("Error: location contains both alias and root directives");

	temp.redirect.status = temp.redirect_status;
	temp.redirect.url = temp.redirect_url;

	CLocation	location;

	pushToLocation(temp, location);
	locations.push_back(location);
	if (locations.size()){
		std::cout << "Max Body : " << locations[0].getMaxBodySize() << std::endl;
		std::cout << "Root dir : " << locations[0].getRootDir() << std::endl;
	}
	return true;
}

void	CParser::setVariable(tempConfig_s& tmp, node_t* node, HandlerContext_e context){

		if (node->type == "alias"){
			if (context == LOCATION_HANDLER)
				tmp.alias_set = setPath(node, tmp.alias);
			else
				throw std::runtime_error("Error : the 'alias' directive can only be used inside a Location block");
		}

		else if (node->type == "autoindex")
			tmp.autoindex_set = setBoolean(node, tmp.autoindex, "autoindex");
		
			else if (node->type == "cgi_path"){
			if (context == LOCATION_HANDLER)
				tmp.cgi_map_set = setCGImap(node, tmp.cgi_map);
			else
				throw std::runtime_error("Error : the 'cgi_path' directive can only be used inside a Location block");
		}
		
		else if (node->type == "charset")
			tmp.charset_set = setcharset(node, tmp.charset);
		
		else if (node->type == "error_page")
			tmp.error_pages_set = setErrorPage(node, tmp.error_pages);
		
		else if (node->type == "index")
			tmp.index_set = setIndex(node, tmp.index);

		else if (node->type == "listen"){
			if (context == SERVER_HANDLER)
				tmp.listen_set = setPorts(node, tmp.listen);
			else
				throw std::runtime_error("Error : a 'listen' directive can only exist within a server block");
		}
		
		else if (node->type == "location"){
			// if (context == SERVER_HANDLER)
			// 	tmp.location_set = setLocation(node, tmp.locations);
			// else
				throw std::runtime_error("Error : a 'location' directive can only exist within a server block");
		}
		
		else if (node->type == "max_body_size")
			tmp.max_body_set = setNumericValue<long long>(node, tmp.max_body, 0, SYS_MAX_BODY);

		
		else if (node->type == "max_connections"){
			if (context == GLOBAL_HANDLER)
				tmp.max_connections_set = setNumericValue<int>(node, tmp.max_connections, 1, SYS_MAX_CONN);
			else
				throw std::runtime_error("Error : the 'max_connections' directive cannot be used inside server or location blocks");
		}
		
		else if (node->type == "max_header_size"){
			if (context == GLOBAL_HANDLER || context == SERVER_HANDLER)
				tmp.max_header_set = setNumericValue<long long>(node, tmp.max_header_size, 1, SYS_MAX_HEAD);
			else
				throw std::runtime_error("Error : the 'max_header_size' directive cannot be used inside a location block");
		}		
		
		else if (node->type == "method")
			tmp.method_set = setMethod(node, tmp.method);
		
			else if (node->type == "redirect_status"){
			if (context == LOCATION_HANDLER)
				tmp.redirect_status_set = setBoolean(node, tmp.redirect_status, "redirect status");
			else
				throw std::runtime_error("Error : the 'redirect_status' directive can only be used inside a Location block");
		}
		
		else if (node->type == "redirect_path"){
			if (context == LOCATION_HANDLER)
				tmp.redirect_url_set = setPath(node, tmp.redirect_url);
			else
				throw std::runtime_error("Error : the 'redirect_path' directive can only be used inside a Location block");
		}
		
		else if (node->type == "root")
			tmp.root_set = setPath(node, tmp.root);
	
		else if (node->type == "server"){
			// if (context == SERVER_HANDLER)
			// 	tmp.servers_set = setServer(node, tmp.servers);
			// else
				throw std::runtime_error("Error : the 'server' directive cannot be used inside Location or Server blocks");
		}

		else if (node->type == "server_name"){
			if (context == SERVER_HANDLER)
				tmp.server_names_set = setServerNames(node, tmp.server_names);
			else
				throw std::runtime_error("Error : the 'server' directive cannot be used inside Location or Server blocks");
		}
		else if (node->type == "keepalive_timeout"){
			if (context == GLOBAL_HANDLER || context == SERVER_HANDLER)
				tmp.timeout_ka_set = setNumericValue<int>(node, tmp.timeout_ka, 1, SYS_TM_KA);
			else
				throw std::runtime_error("Error : the 'keepalive_timeout' directive cannot be used inside a Location block");
		}

		else if (node->type == "read_write_timeout"){
			if (context == GLOBAL_HANDLER || context == SERVER_HANDLER)
				tmp.timeout_rw_set = setNumericValue<int>(node, tmp.timeout_rw, 1, SYS_TM_RW);
			else
				throw std::runtime_error("Error : the 'read_write_timeout' directive cannot be used inside a Location block");
		}

		else if (node->type == "try_files"){
			if (context == LOCATION_HANDLER)
				tmp.try_files_set = setTryFiles(node, tmp.try_files);
			else
				throw std::runtime_error("Error : the 'try_files' directive can only be used inside a Location block");
		}

		else if (node->type == "upload_path"){
			if (context == LOCATION_HANDLER)
				tmp.upload_path_set = setPath(node, tmp.upload_path);
			else
				throw std::runtime_error("Error : the 'upload_path' directive can only be used inside a Location block");
		}
}

void	CParser::pushToConfig(tempConfig_s& tmp, Config& dest){

	if (tmp.autoindex_set)
		dest.setAutoIndex(tmp.autoindex);
	if (tmp.charset_set)
		dest.setCharset(tmp.charset);
	if (tmp.error_pages_set)
		dest.setErrorPages(tmp.error_pages);
	if (tmp.index_set)
		dest.setIndex(tmp.index);
	if (tmp.max_body_set)
		dest.setMaxBodySize(tmp.max_body);
	if (tmp.max_connections_set)
		dest.setMaxConnections(tmp.max_connections);
	if (tmp.max_header_set)
		dest.setMaxHeaderSize(tmp.max_header_size);
	if (tmp.method_set)
		dest.setMethod(tmp.method);
	if (tmp.root_set)
		dest.setRootDir(tmp.root);
	if (tmp.servers_set)
		dest.setServers(tmp.servers);
	if (tmp.timeout_ka_set)
		dest.setTimeoutKA(tmp.timeout_ka);
	if (tmp.timeout_rw_set)
		dest.setTimeoutRW(tmp.timeout_rw);
}

void	CParser::pushToServer(tempConfig_s& tmp, CServer& dest){
	if (tmp.charset_set)
		dest.setCharset(tmp.charset);
	if (tmp.error_pages_set)
		dest.setErrorPages(tmp.error_pages);
	if (tmp.index_set)
		dest.setIndex(tmp.index);
	if (tmp.listen_set)
		dest.setListen(tmp.listen);
	if (tmp.location_set)
		dest.setLocations(tmp.locations);
	if (tmp.max_body_set)
		dest.setMaxBodySize(tmp.max_body);
	if (tmp.max_header_set)
		dest.setMaxHeaderSize(tmp.max_header_size);
	if (tmp.method_set)
		dest.setMethod(tmp.method);
	if (tmp.root_set)
		dest.setRootDir(tmp.root);
	if (tmp.server_names_set)
		dest.setServerNames(tmp.server_names);
	if (tmp.timeout_ka_set)
		dest.setTimeoutKA(tmp.timeout_ka);
	if (tmp.timeout_rw_set)
		dest.setTimeoutRW(tmp.timeout_rw);
}

void	CParser::pushToLocation(tempConfig_s& tmp, CLocation& dest){
	
	if (tmp.alias_set)
		dest.setAlias(tmp.alias);
	if (tmp.autoindex_set)
		dest.setAutoIndex(tmp.autoindex);
	if (tmp.cgi_map_set)
		dest.setCGIMap(tmp.cgi_map);
	if (tmp.charset_set)
		dest.setcharset(tmp.charset);
	if (tmp.error_pages_set)
		dest.setErrorPages(tmp.error_pages);
	if (tmp.index_set)
		dest.setIndex(tmp.index);
	if (tmp.max_body_set)
		dest.setMaxBodySize(tmp.max_body);
	if (tmp.method_set)
		dest.setMethod(tmp.method);
	if (tmp.modifier_set)
		dest.setModifier(tmp.modifier);
	if (tmp.redirect_set)
		dest.setRedirect(tmp.redirect);
	if (tmp.root_set)
		dest.setRootDir(tmp.root);
	if (tmp.root_set)
		dest.setTryFiles(tmp.try_files);
	if (tmp.upload_path_set)
		dest.setUploadPath(tmp.upload_path);
	if (tmp.uri_set)
		dest.setURI(tmp.uri);
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		SETTERS 		<<<<<<
>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
*/

bool	CParser::setBoolean(node_t* node, bool& state, const std::string& var_name){

	if (!isOnlyArg(node->args))
		throw std::runtime_error("Error: multiple arguments when only one permitted : " + node->args[0] + " " + createArgsString(node->args));

	std::string lowercase = to_lower(node->args[0]);

	if ( lowercase == "true")
		state = true;
	else if (lowercase == "false")
		state = false;
	else if (lowercase.length() == 1 && ( lowercase == "1" || lowercase == "t"))
			state = true;
	else if (lowercase.length() == 1 && (lowercase == "0" || lowercase == "f"))
			state = false;
	else
		throw std::runtime_error("Error: value must be boolean (T/F/t/f/true/false/TRUE/FALSE/1/0) : " + var_name + " " +  node->args[0]);
	return true;
}

/*
	CGI Handling

	CGI path must point to an executable file (that handles given cgi type(s))
	If not, then should throw error (if strict) or warn (don't add to map)

	If multiple cgi extensions provided with same
*/
bool	CParser::setCGImap(node_t* node, std::map<std::string, std::string>& cgi_map){

	size_t size = node->args.size();

	if (size <= 1)
		throw std::runtime_error("Error: invalid cgi_path : must contain path and at least one extension type : " + node->type + " " + createArgsString(node->args));
	std::string path = node->args[0];
	std::string ext;
	for (size_t i = 1; i < size; i++){
		if (node->args[i][0] == '.') // consider anything starting '.' a valid cgi type
			cgi_map[node->args[i]] = path;
		else
			throw std::runtime_error("Error: invalid cgi_path : " + node->type + " " + createArgsString(node->args));
	}
	return true;
}

bool	CParser::setcharset(node_t* node, std::vector<std::string>& charset){

	size_t size = node->args.size();

	for (size_t i = 0; i < size; i++){
		if (isValidCharset(node->args[i]))
			charset.push_back(node->args[i]);
		else
			throw std::runtime_error("Error: invalid directive : unsupported charset : "  + node->type + " " + createArgsString(node->args));
	}
	return true;
}

bool	CParser::setErrorPage(node_t* node, std::vector<errorPage_s>& error_pages){

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
	return true;
}

bool	CParser::setIndex(node_t* node, std::vector<std::string>& index){

	size_t size = node->args.size();

	for (size_t i = 0; i < size - 1; i++)
			index.push_back(node->args[i]);
	return true;
}

bool	CParser::setModifier(std::string& str, LocationMatchType_e& modifier){
	if (str.length() != 1)
		throw std::runtime_error("Error: invalid modifier : " + str);

	if (str[0] == '=')
		modifier = EXACT;		// http uri must match location.uri exactly
	else if (str[0] == '^')
		modifier = PREFIX; 		// if uri matches, return straight away without checking other locations (https://chatgpt.com/share/6995da46-ff54-800f-b363-7aa166db7e2e)
	else
		throw std::runtime_error("Error: invalid modifier : " + str);
	return true;
}

bool	CParser::setMethod(node_t* node, std::vector<std::string>& method){

	size_t size = node->args.size();

	for (size_t i = 0; i < size; i++){
		if (isValidMethod(node->args[i]))
			method.push_back(node->args[i]);
		else
			throw std::runtime_error("Error: invalid method"); // change to log entry for config
	}
	return true;
}

bool	CParser::setServerNames(node_t* node, std::vector<std::string>& names){

	size_t size = node->args.size();

	for (size_t i = 0; i < size - 1; i++)
			names.push_back(node->args[i]);
	return true;
}

bool	CParser::setPorts(node_t* node, std::vector<listen_s>& ports){

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
	return true;
}

bool	CParser::setPath(node_t* node, std::string& path){

	if (!isOnlyArg(node->args))
		throw std::runtime_error("Error: multiple arguments when only one permitted : " + node->args[0] + " " + createArgsString(node->args));
	path = node->args[0];
	return true;
}

bool	CParser::setTryFiles(node_t* node, tryFiles_s& try_files){

	size_t size = node->args.size();

	for (size_t i = 0; i < size; i++){
		if (i < size - 1)
			try_files.try_paths.push_back(node->args[i]);
		else if (!convertToType(try_files.default_error, node->args[i]))
			throw std::runtime_error("Error: invalid error_page number in 'try file' directive : " + node->args[i]);
	}
	return true;
}

template <typename T>
bool	CParser::setNumericValue(node_t* node, T& target, T min, T max){

	T val = 0;

	if (isOnlyArg(node->args) && convertToType(val, node->args[0])){
		if (val <= max && val >= min)
			target = val;
		else
			throw std::runtime_error("Error: value out of range : ");
	}
	else
		throw std::runtime_error("Error: value not of expected type : ");
	return true;
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


/*

bool	CParser::setServer(node_t* server_node, std::vector<CServer>& servers){
	CServer server;
	bool autoindex = false;
	std::vector<std::string> charset;
	std::vector<errorPage_s> error_pages;
	std::vector<std::string> index;
	std::vector<listen_s> listen;
	std::vector<CLocation> locations;
	long long max_body;
	long long max_header;
	std::vector<std::string> method;
	std::string root;
	std::vector<std::string> server_names;
	int timeout_ka;
	int timeout_rw;

	bool autoindex_set = false;
	bool charset_set = false;
	bool error_pages_set = false;
	bool index_set;
	bool listen_set = false;
	bool location_set = false;
	bool max_body_set = false;
	bool max_header_set = false;
	bool method_set = false;
	bool root_set = false;
	bool server_names_set = false;
	bool timeout_ka_set = false;
	bool timeout_rw_set = false;

	  for (size_t i = 0; i < server_node->children.size(); i++){

	  	node_t* node = server_node->children[i];
	  	if (node->type == "server")
			throw std::runtime_error("Error : server blocks cannot contain server blocks");
	  	else if (node->type == "autoindex")
			autoindex_set = setBoolean(node, autoindex);
	  	else if (node->type == "charset")
			charset_set = setcharset(node, charset);
	  	else if (node->type == "error_page")
			error_pages_set = setErrorPage(node, error_pages);
	  	else if (node->type == "index")
			index_set = setIndex(node, index);
	  	else if (node->type == "listen")
			listen_set = setPorts(node, listen);
	  	else if (node->type == "location")
			setLocation(node, locations);
	  	else if (node->type == "max_body_size")
			max_body_set = setNumericValue<long long>(node, max_body, 0, SYS_MAX_BODY);
	  	else if (node->type == "max_header_size")
			max_header_set = setNumericValue<long long>(node, max_header, 1, SYS_MAX_HEAD);
	  	else if (node->type == "method")
			method_set = setMethod(node, method);
		else if (node->type == "root")
			root_set = setPath(node, root);
		else if (node->type == "server_name")
			server_names_set = setServerNames(node, server_names);
		else if (node->type == "keepalive_timeout")
			timeout_ka_set = setNumericValue<int>(node, timeout_ka, 1, SYS_TM_KA);
		else if (node->type == "read_write_timeout")
			timeout_rw_set = setNumericValue<int>(node, timeout_rw, 1, SYS_TM_RW);
	}
	if (autoindex_set)
		server.setAutoIndex(autoindex);
	if (charset_set)
		server.setcharset(charset);
	if (error_pages_set)
		server.setErrorPages(error_pages);
	if (index_set)
		server.setIndex(index);
	if (listen_set)
		server.setListen(listen);
	if (location_set)
		server.setLocations(locations);
	if (max_body_set)
		server.setMaxBodySize(max_body);
	if (max_header_set)
		server.setMaxHeaderSize(max_header);
	if (method_set)
		server.setMethod(method);
	if (root_set)
		server.setRootDir(root);
	if (server_names_set)
		server.setServerNames(server_names);
	if (timeout_ka_set)
		server.setTimeoutKA(timeout_ka);
	if (timeout_rw_set)
		server.setTimeoutRW(timeout_rw);

	servers.push_back(server);
	return true;
}

bool	CParser::setLocation(node_t* location_node, std::vector<CLocation>& locations){

	CLocation 							location;

	std::string							alias;
	bool								autoindex;
	std::map<std::string, std::string>	cgi_map;
	std::vector<std::string>			charset;
	std::vector<errorPage_s>			error_pages;
	std::vector<std::string>			index;
	long long							max_body;
	std::vector<std::string>			method;
	LocationMatchType_e					modifier;
	redirect_s							redirect;
	bool								redirect_status;
	std::string							redirect_url;
	std::string							root;			// Can't have both root and alias
	std::string							upload_path;
	std::string							uri;

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
			setCGImap(node, cgi_map);
		else if (node->type == "charset")
			setcharset(node, charset);
		else if (node->type == "error_page")
			setErrorPage(node, error_pages);
		else if (node->type == "index")
			setIndex(node, index);
		else if (node->type == "max_body_size")
			setNumericValue<long long>(node, max_body, 0, SYS_MAX_BODY);
		else if (node->type == "method")
			setMethod(node, method);
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
	location.setCGIMap(cgi_map);
	location.setcharset(charset);
	location.setErrorPages(error_pages);
	location.setIndex(index);
	location.setMaxBodySize(max_body);
	location.setMethod(method);
	location.setAutoIndex(autoindex);
	location.setModifier(modifier);
	location.setRedirect(redirect);
	location.setRootDir(root);
	location.setUploadPath(upload_path);
	location.setURI(uri);

	locations.push_back(location);
	return true;
} */