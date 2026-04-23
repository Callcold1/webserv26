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

CParser::CParser( const std::string& filename ) : Parser(){
	openConfigFile(filename);
	Log::instance().log(Log::INFO, Log::TYPE_CLASS, "Config file successfully opened : " + filename, false);
}

CParser::~CParser( void ){
	_stream.close();
}

CParser::CParser(const CParser &other): Parser(other._tokens){(void)other;}

CParser& CParser::operator=(const CParser &other) {
	(void) other;
    return *this;
}

void	CParser::openConfigFile(const std::string& filename)
{
	_stream.open(filename.c_str());

	if (!_stream.is_open()){
		throw std::runtime_error("Could not open configuration file : " + filename);
	}
}

void	CParser::parseConfigFile(Config& config){
	
	//Log::instance().log(Log::INFO, Log::TYPE_CLASS, "Config parsing started....", false);
	std::ostringstream oss;
	try {
		tokeniseStream();
		initKeywords();

		parseTokens();
	//	Log::instance().log(Log::INFO, Log::TYPE_CONF, "Parsed to _raw_config", false);
		
		setConfig(config);
	//	Log::instance().log(Log::DEBUG, Log::TYPE_CONF, "Config object created : initial parsing complete : \n\n" + printstream(config), false);

		config.propagateVarsToChildren();
		//Log::instance().log(Log::DEBUG, Log::TYPE_CONF, "Variables propagated from global -> server -> location : \n\n" + printstream(config), false);
		
		config.mapServersToPorts();
		config.checkForOverlappingPorts();
		delete _rawConfig;
		Log::instance().log(Log::INFO, Log::TYPE_CLASS, "Config parsing successfully completed....", false);
	}
	catch (std::exception& e){
		delete _rawConfig;
		Log::instance().log(Log::ERROR, Log::TYPE_CONF, e.what(), true);
		throw std::runtime_error(e.what());
	}
}

void	CParser::tokeniseStream(void){
	try{
		tokenContext_t context("{};", false, false);
		Tokeniser tokeniser(_stream, context);
		_tokens = tokeniser.getTokens();
	}
	catch ( std::exception& e){
		Log::instance().log(Log::ERROR, Log::TYPE_CONF, e.what(), true);
		throw std::runtime_error(e.what());
	} 
}

void	CParser::parseTokens(void){
	try {
		_rawConfig = parse();
	}
	catch (std::exception& e) {
		Log::instance().log(Log::ERROR, Log::TYPE_CONF, e.what(), true);
		throw std::runtime_error(e.what());
	}
}

node_t*	CParser::parse(void) {

	node_t* root = new node_t;
	root->type = "global";
	root->line_num = "0";

	try {
		if (_tokens[_pos].type == T_WORD)
			parseNext(root, tokeniser_s(T_EOF, "", Tokeniser::getLineCount()));
		else {
			delete root;
			throw std::runtime_error("Invalid config file structure");
		}
	}
	catch (const std::exception& e){
		delete root;
		Log::instance().log(Log::ERROR, Log::TYPE_CONF, e.what(), true);
		throw std::runtime_error(e.what());
	}
	return root;
}

void	CParser::parseNext(node_t* parent, tokeniser_s awaited){

	while (!(_tokens[_pos].type == awaited.type && _tokens[_pos].value == awaited.value) && _tokens[_pos].type != T_EOF){

		TokenType_e type = _tokens[_pos].type;
		std::string value = _tokens[_pos].value;
		std::string line_num = toString(_tokens[_pos].line_num + 1);

		if (check(T_WORD, "server")){
			consume();
			expect(T_SYMBOL, "{");
			parent->children.push_back(addNode(value, line_num));
			parseNext(parent->children.back(), tokeniser_s(T_SYMBOL, "}", Tokeniser::getLineCount()));
		}
		else if (check(T_WORD, "mime_types")){
			consume();
			expect(T_SYMBOL, "{");
			parent->children.push_back(addNode(value, line_num));
			parseNext(parent->children.back(), tokeniser_s(T_SYMBOL, "}", Tokeniser::getLineCount()));
		}
		else if (check(T_WORD, "location")){
			parent->children.push_back(addNode(value, line_num));
			consume();
			for (size_t i = 0; i < 3; i++){		
				tokeniser_s next = peek();
				if (next.type == T_WORD){
					parent->children.back()->args.push_back(next.value);
					consume();
				}
				else if (next.type == T_SYMBOL && next.value == "{"){
					consume();
					break;
				}
				else
					throw std::runtime_error("Invalid token in line number : "  + line_num + "location block :" + next.value);
			}
			parseNext(parent->children.back(), tokeniser_s(T_SYMBOL, "}", Tokeniser::getLineCount()));
		}
		else if (type == T_WORD && checkKeywords(parent->type, value)){
			parent->children.push_back(addNode(value, line_num));
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
					throw std::runtime_error("Invalid token in line number : " + line_num + ": expected a ';'");
			}
		}
		else
			throw std::runtime_error("Unexpected token '" + value + "' token number : " + toString(_pos) + ", line number : " + line_num);
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
	else if (context == "mime_types")
		vec = _mime_keywords;
	if (std::find(vec.begin(), vec.end(), value) != vec.end())
		return true;
	return false;
}

node_t*	CParser::addNode(const std::string& node_type, const std::string& line_num){

	node_t* n = new node_t; 
	n->type = node_type;
	n->line_num = line_num;
	return n;
}

void	CParser::initKeywords(void) {

	// LOCATION
	_location_keywords.push_back("alias");
	_location_keywords.push_back("autoindex");
	_location_keywords.push_back("cgi_path");
	_location_keywords.push_back("charset");
	_location_keywords.push_back("error_page");
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
	_server_keywords.push_back("cgi_path");
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
	_global_keywords.push_back("mime_types");

	_mime_keywords.push_back("text/plain");
	_mime_keywords.push_back("text/html");
	_mime_keywords.push_back("text/css");
	_mime_keywords.push_back("text/ISO-8859-1");
	_mime_keywords.push_back("application/octet-stream");
	_mime_keywords.push_back("application/javascript");
	_mime_keywords.push_back("application/pdf");
	_mime_keywords.push_back("image/gif");
	_mime_keywords.push_back("image/jpeg");
	_mime_keywords.push_back("image/png");
	_mime_keywords.push_back("image/x-icon");
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

mime_types			block		yes				map<ext, mime_type>		Append				n/a						Global only

*/

void	CParser::setConfig(Config& config){

	tempConfig_s	tmp;

	for (size_t i = 0; i < _rawConfig->children.size(); i++){

		node_t* node = _rawConfig->children[i];
		setVariable(tmp, node, GLOBAL_HANDLER);
	}
	if (!tmp.servers_set)
		throw std::runtime_error("Configuration file must contain at least one 'server' block");
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
		setVariable(tmp, node, SERVER_HANDLER);
	}
	if (!tmp.listen_set)
		throw std::runtime_error("Server must contain at least one 'Listen' directive ");
	if (!tmp.location_set)
		throw std::runtime_error("Server must contain at least one 'location' block ");
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
		throw std::runtime_error("Invalid number of arguments provided for location uri : "
			+ location_node->type + " " + createArgsString(location_node->args) + " : line " + location_node->line_num);

	for (size_t i = 0; i < location_node->children.size(); i++){

		node_t* node = location_node->children[i];
		setVariable(temp, node, LOCATION_HANDLER);
	}
	if (checkAliasRoot(temp.alias, temp.root))
		throw std::runtime_error("Location contains both alias and root directives");

	temp.redirect.status = temp.redirect_status;
	temp.redirect.url = temp.redirect_url;
	if (temp.redirect_status_set || temp.redirect_url_set )
		temp.redirect_set = true;
	CLocation	location;

	pushToLocation(temp, location);
	locations.push_back(location);
	return true;
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		HANDLERS 		<<<<<<
>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
*/

void	CParser::handleAlias(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == LOCATION_HANDLER)
		tmp.alias_set = setPath(node, tmp.alias);
	else
		throw std::runtime_error("the 'alias' directive can only be used inside a Location block");
}

void	CParser::handleAutoIndex(node_t* node, tempConfig_s& tmp){
	tmp.autoindex_set = setBoolean(node, tmp.autoindex);
}

void	CParser::handleCharset(node_t* node, tempConfig_s& tmp){
	tmp.charset_set = setCharset(node, tmp.charset);
}

void	CParser::handleCGIMap(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == LOCATION_HANDLER || context == SERVER_HANDLER)
		tmp.cgi_map_set = setCGImap(node, tmp.cgi_map);
	else
		throw std::runtime_error("Invalid context for cgi_map directive");
}

void	CParser::handleErrorPages(node_t* node, tempConfig_s& tmp){
	tmp.error_page_set = setErrorPage(node, tmp.error_pages);
}

void	CParser::handleIndex(node_t* node, tempConfig_s& tmp){
	tmp.index_set = setIndex(node, tmp.index);
}

void	CParser::handleListen(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == SERVER_HANDLER)
		tmp.listen_set = setPorts(node, tmp.listen);
	else
		throw std::runtime_error("a 'listen' directive can only exist within a server block");
}

void	CParser::handleLocation(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == SERVER_HANDLER)
		tmp.location_set = setLocation(node, tmp.locations);
	else
		throw std::runtime_error("Invalid context for location directive : a 'location' directive can only exist within a server block");
}

void	CParser::handleMaxBody(node_t* node, tempConfig_s& tmp){
	tmp.max_body_set = setNumericValue<long long>(node, tmp.max_body, 0, SYS_MAX_BODY);
}

void	CParser::handleMaxConnections(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == GLOBAL_HANDLER)
		tmp.max_connections_set = setNumericValue<int>(node, tmp.max_connections, 1, SYS_MAX_CONN);
	else
		throw std::runtime_error("Invalid context for max_connections directive : the 'max_connections' directive cannot be used inside server or location blocks");
}

void	CParser::handleMaxHeader(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == GLOBAL_HANDLER || context == SERVER_HANDLER)
		tmp.max_header_set = setNumericValue<long long>(node, tmp.max_header_size, 1, SYS_MAX_HEAD);
	else
		throw std::runtime_error("Invalid context for max_header_size directive");
}		

void	CParser::handleMethod(node_t* node, tempConfig_s& tmp){
	tmp.method_set = setMethod(node, tmp.method);
}

void	CParser::handleMimeBlock(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == GLOBAL_HANDLER)
		tmp.mime_set = setMimeBlock(node, tmp.mime);
	else
		throw std::runtime_error("Invalid context for mime block : a 'mime' block can only exist in the global context");
}

void	CParser::handleRedirectStatus(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == LOCATION_HANDLER)
		tmp.redirect_status_set = setBoolean(node, tmp.redirect_status);
	else
		throw std::runtime_error("the 'redirect_status' directive can only be used inside a Location block");
}

void	CParser::handleRedirectURL(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == LOCATION_HANDLER)
		tmp.redirect_url_set = setPath(node, tmp.redirect_url);
	else
		throw std::runtime_error("the 'redirect_path' directive can only be used inside a Location block");
}

void	CParser::handleRoot(node_t* node, tempConfig_s& tmp){
	tmp.root_set = setPath(node, tmp.root);
}

void	CParser::handleServer(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == GLOBAL_HANDLER)
		tmp.servers_set = setServer(node, tmp.servers);
	else
		throw std::runtime_error("Invalid context for server block : a 'server' block can only exist in the global context");
}

void	CParser::handleServerNames(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == SERVER_HANDLER)
		tmp.server_name_set = setServerName(node, tmp.server_names);
	else
		throw std::runtime_error("the 'server' directive cannot be used inside Location or Server blocks");
}

void	CParser::handleTimeoutKA(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == GLOBAL_HANDLER || context == SERVER_HANDLER)
		tmp.timeout_ka_set = setNumericValue<int>(node, tmp.timeout_ka, 1, SYS_TM_KA);
	else
		throw std::runtime_error("Invalid context for keepalive_timeout directive : the 'keepalive_timeout' directive cannot be used inside a Location block");
}

void	CParser::handleTimeoutRW(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == GLOBAL_HANDLER || context == SERVER_HANDLER)
		tmp.timeout_rw_set = setNumericValue<int>(node, tmp.timeout_rw, 1, SYS_TM_RW);
	else
		throw std::runtime_error("Invalid context for read_write_timeout directive : the 'read_write_timeout' directive cannot be used inside a Location block");
}

void	CParser::handleTryFiles(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == LOCATION_HANDLER)
		tmp.try_files_set = setTryFiles(node, tmp.try_files);
	else
		throw std::runtime_error("Invalid context for try_files directive : the 'try_files' directive can only be used inside a Location block");
}

void	CParser::handleUploadPath(node_t* node, tempConfig_s& tmp, HandlerContext_e context){
	if (context == LOCATION_HANDLER)
		tmp.upload_path_set = setPath(node, tmp.upload_path);
	else
		throw std::runtime_error("Invalid context for upload_path directive : the 'upload_path' directive can only be used inside a Location block");
}

void	CParser::setVariable(tempConfig_s& tmp, node_t* node, HandlerContext_e context){
	try{
		if (node->type == "alias")
			handleAlias(node, tmp, context);
		else if (node->type == "autoindex")
			handleAutoIndex(node, tmp);
		else if (node->type == "cgi_path")
			handleCGIMap(node, tmp, context);		
		else if (node->type == "charset")
			handleCharset(node, tmp);
		else if (node->type == "error_page")
			handleErrorPages(node, tmp);
		else if (node->type == "index")
			handleIndex(node, tmp);
		else if (node->type == "listen")
			handleListen(node, tmp, context);
		else if (node->type == "location")
			handleLocation(node, tmp, context);
		else if (node->type == "max_body_size")
			handleMaxBody(node, tmp);
		else if (node->type == "max_connections")
			handleMaxConnections(node, tmp, context);
		else if (node->type == "max_header_size")
			handleMaxHeader(node, tmp, context);
		else if (node->type == "method")
			handleMethod(node, tmp);
		else if (node->type == "mime_types")
			handleMimeBlock(node, tmp, context);
		else if (node->type == "redirect_status")
			handleRedirectStatus(node, tmp, context);
		else if (node->type == "redirect_path")
			handleRedirectURL(node, tmp, context);	
		else if (node->type == "root")
			handleRoot(node, tmp);
		else if (node->type == "server")
			handleServer(node, tmp, context);
		else if (node->type == "server_name")
			handleServerNames(node, tmp, context);
		else if (node->type == "keepalive_timeout")
			handleTimeoutKA(node, tmp, context);
		else if (node->type == "read_write_timeout")
			handleTimeoutRW(node, tmp, context);
		else if (node->type == "try_files")
			handleTryFiles(node, tmp, context);
		else if (node->type == "upload_path")
			handleUploadPath(node, tmp, context);
		else
			throw std::runtime_error("Unknown directive '" + node->type + "' in line number : " + node->line_num);
	}
	catch (std::exception& e){
		throw std::runtime_error(e.what());
	}
}

void	CParser::pushToConfig(tempConfig_s& tmp, Config& dest){

	if (tmp.autoindex_set)
		dest.setAutoIndex(tmp.autoindex, true);
	if (tmp.charset_set)
		dest.setCharset(tmp.charset, true);
	if (tmp.error_page_set)
		dest.setErrorPage(tmp.error_pages, true);
	if (tmp.index_set)
		dest.setIndex(tmp.index, true);
	if (tmp.max_body_set)
		dest.setMaxBodySize(tmp.max_body, true);
	if (tmp.max_connections_set)
		dest.setMaxConnections(tmp.max_connections, true);
	if (tmp.max_header_set)
		dest.setMaxHeaderSize(tmp.max_header_size, true);
	if (tmp.method_set)
		dest.setMethod(tmp.method, true);
	if (tmp.mime_set)
		dest.setMime(tmp.mime, true);
	if (tmp.root_set)
		dest.setRootDir(tmp.root, true);
	if (tmp.servers_set)
		dest.setServers(tmp.servers, true);
	if (tmp.timeout_ka_set)
		dest.setTimeoutKA(tmp.timeout_ka, true);
	if (tmp.timeout_rw_set)
		dest.setTimeoutRW(tmp.timeout_rw, true);
}

void	CParser::pushToServer(tempConfig_s& tmp, CServer& dest){
	if (tmp.charset_set)
		dest.setCharset(tmp.charset, true);
	if (tmp.cgi_map_set)
		dest.setCGIMap(tmp.cgi_map, true);
	if (tmp.error_page_set)
		dest.setErrorPage(tmp.error_pages, true);
	if (tmp.index_set)
		dest.setIndex(tmp.index, true);
	if (tmp.listen_set)
		dest.setListen(tmp.listen, true);
	if (tmp.location_set)
		dest.setLocations(tmp.locations, true);
	if (tmp.max_body_set)
		dest.setMaxBodySize(tmp.max_body, true);
	if (tmp.max_header_set)
		dest.setMaxHeaderSize(tmp.max_header_size, true);
	if (tmp.method_set)
		dest.setMethod(tmp.method, true);
	if (tmp.root_set)
		dest.setRootDir(tmp.root, true);
	if (tmp.server_name_set)
		dest.setServerName(tmp.server_names, true);
	if (tmp.timeout_ka_set)
		dest.setTimeoutKA(tmp.timeout_ka, true);
	if (tmp.timeout_rw_set)
		dest.setTimeoutRW(tmp.timeout_rw, true);
}

void	CParser::pushToLocation(tempConfig_s& tmp, CLocation& dest){
	
	if (tmp.alias_set)
		dest.setAlias(tmp.alias, true);
	if (tmp.autoindex_set)
		dest.setAutoIndex(tmp.autoindex, true);
	if (tmp.cgi_map_set)
		dest.setCGIMap(tmp.cgi_map, true);
	if (tmp.charset_set)
		dest.setCharset(tmp.charset, true);
	if (tmp.error_page_set)
		dest.setErrorPage(tmp.error_pages, true);
	if (tmp.index_set)
		dest.setIndex(tmp.index, true);
	if (tmp.max_body_set)
		dest.setMaxBodySize(tmp.max_body, true);
	if (tmp.method_set)
		dest.setMethod(tmp.method, true);
	if (tmp.modifier_set)
		dest.setModifier(tmp.modifier, true);
	if (tmp.redirect_status_set)
		dest.setRedirect(tmp.redirect, true);
	if (tmp.root_set)
		dest.setRootDir(tmp.root, true);
	if (tmp.root_set)
		dest.setTryFiles(tmp.try_files, true);
	if (tmp.upload_path_set)
		dest.setUploadPath(tmp.upload_path, true);
	if (tmp.uri_set)
		dest.setURI(tmp.uri, true);
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		SETTERS 		<<<<<<
>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
*/

bool	CParser::setBoolean(node_t* node, bool& state){

	if (!isOnlyArg(node->args))
		throw std::runtime_error("Multiple arguments when only one permitted : " 
			+ node->type + " " + createArgsString(node->args) + " : line " + node->line_num);

	std::string lowercase = to_lower(node->args[0]);

	if ( lowercase == "true")
		state = true;
	else if (lowercase == "false")
		state = false;
	else if (lowercase.length() == 1 && ( lowercase == "1" || lowercase == "t"))
			state = true;
	else if (lowercase.length() == 1 && (lowercase == "0" || lowercase == "f"))
			state = false;
	else if (lowercase.length() == 2 && ( lowercase == "on"))
			state = true;
	else if (lowercase.length() == 3 && (lowercase == "off"))
			state = false;
	else
		throw std::runtime_error("value must be boolean (T/F/t/f/true/false/TRUE/FALSE/1/0/on/off/ON/OFF) : " 
			+ node->type + " " + createArgsString(node->args) + " : line " + node->line_num);
	return true;
}

bool	CParser::setCGImap(node_t* node, std::map<std::string, std::string>& cgi_map){

	size_t size = node->args.size();

	if (size <= 1)
		throw std::runtime_error("invalid cgi_path : must contain path and at least one extension type : " 
			+ node->type + " " + createArgsString(node->args) + " : line " + node->line_num);
	std::string path = node->args[0];
	std::string ext;
	if (node->args[0][0] == '.')
		ext = node->args[0];
	else
		throw std::runtime_error("invalid cgi_path : " + node->type + " " + createArgsString(node->args));

	for (size_t i = 1; i < size; i++){
		cgi_map[node->args[0]] = path;
	}
	return true;
}

bool	CParser::setCharset(node_t* node, std::vector<std::string>& charset){

	size_t size = node->args.size();

	for (size_t i = 0; i < size; i++){
		if (isValidCharset(node->args[i])){
			if ((std::find(charset.begin(), charset.end(), node->args[i]) == charset.end()))
				charset.push_back(node->args[i]);
		}
		else
			throw std::runtime_error("invalid directive : unsupported charset : " 
				+ node->type + " " + createArgsString(node->args) + " : line " + node->line_num);
	}
	return true;
}

bool	CParser::setErrorPage(node_t* node, std::map<int, std::string>& error_pages){

	size_t size = node->args.size();
	int error_num;
	std::string path;

	if (convertToType(path, node->args[size - 1])){

		// possibly create an array/vector of valid/supported error numbers to use for validation
		for (size_t i = 0; i < size - 1; i++){
			if (convertToType(error_num, node->args[i]) && error_num >= 400 && error_num < 600)
				error_pages[error_num] = path;
			else
				throw std::runtime_error("invalid error_page number : " + node->args[i] + " : line " + node->line_num);
		}
	}
	else
		throw std::runtime_error("invalid error number format : " + node->args[size - 1] + " : line " + node->line_num);
	return true;
}

bool	CParser::setIndex(node_t* node, std::vector<std::string>& index){

	size_t size = node->args.size();

	if (size == 0)
		return false;

	for (size_t i = 0; i < size; i++)
			index.push_back(node->args[i]);
	return true;
}

bool	CParser::setModifier(std::string& str, LocationMatchType_e& modifier){
	if (str.length() != 1)
		throw std::runtime_error("invalid location modifier : " + str);

	if (str[0] == '=')
		modifier = EXACT;		// http uri must match location.uri exactly
	else if (str[0] == '^')
		modifier = PREFIX; 		// if uri matches, return straight away without checking other locations (https://chatgpt.com/share/6995da46-ff54-800f-b363-7aa166db7e2e)
	else
		throw std::runtime_error("invalid location modifier : " + str);
	return true;
}

bool	CParser::setMethod(node_t* node, std::vector<std::string>& method){

	size_t size = node->args.size();

	for (size_t i = 0; i < size; i++){
		if (isValidMethod(node->args[i]))
			method.push_back(node->args[i]);
		else
			throw std::runtime_error("invalid method");
	}
	return true;
}

bool	CParser::setMimeBlock(node_t* mime_node, std::map<std::string, std::vector<std::string> >& mime){

	tempConfig_s tmp;

	for (size_t i = 0; i < mime_node->children.size(); i++){
		node_t* node = mime_node->children[i];
		tmp.mime[node->type] = node->args;
		setMime(node, mime);
	}
	return true;
}

bool	CParser::setMime(node_t* node, std::map<std::string, std::vector<std::string> >& mime){

	if (isValidMime(node->type))
			mime.insert(std::pair<std::string, std::vector<std::string> >(node->type, node->args));
		else
			throw std::runtime_error("invalid mime");
	return true;
}

bool	CParser::setServerName(node_t* node, std::vector<std::string>& names){

	size_t size = node->args.size();

	for (size_t i = 0; i < size; i++)
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
		throw std::runtime_error("multiple arguments when only one permitted : " + node->type + " " + createArgsString(node->args) + " : line " + node->line_num);
	if (node->args.size() == 0 || (node->args.size() == 1 && node->args[0].empty()))
		throw std::runtime_error("no value provided for directive : " + node->type + " : line " + node->line_num);
	path = node->args[0];
	return true;
}

bool	CParser::setTryFiles(node_t* node, tryFiles_s& try_files){

	size_t size = node->args.size();

	if (size == 0)
		return false;

	for (size_t i = 0; i < size; i++){
		if (i < size - 1)
			try_files.try_paths.push_back(node->args[i]);
		else if (!convertToType(try_files.default_error, node->args[i]))
			throw std::runtime_error("invalid error_page number in 'try file' directive : " + node->type + " " + createArgsString(node->args) + " : line " + node->line_num);
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
			throw std::runtime_error("value out of range : ");
	}
	else
		throw std::runtime_error("value not of expected type : ");
	return true;
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
>>>>>>		TYPE VALIDATION		<<<<<<
>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<
*/

bool	CParser::isValidCharset(const std::string& str){
	return (str.compare("utf-8") == 0 || str.compare("iso-8859-1") == 0);
}

bool	CParser::isValidMethod(const std::string& str){
	return (str.compare("GET") == 0 || str.compare("POST") == 0 || str.compare("PUT") == 0 ||
		str.compare("DELETE") == 0 || str.compare("HEAD") == 0 || str.compare("OPTIONS") == 0);
}

bool	CParser::checkAliasRoot(const std::string& alias, const std::string& root){
	return (alias.length() > 0 && root.length() > 0);
}

bool	CParser::isValidMime(const std::string& str){
	return (str.find("/") != std::string::npos);
}
