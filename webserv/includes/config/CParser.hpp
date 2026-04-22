/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CParser.hpp  		                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 15:18:33 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/17 10:25:45 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "utilityFunctions.hpp"
#include "Log.hpp"
#include "Tokeniser.hpp"
#include "Parser.hpp"
#include "Config.hpp"

#include <cstring>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>

class Config;

struct tempConfig_s {
	std::string							alias;				/*	LOCATION	*/
	bool								autoindex;			/*	GLOBAL/SERVER/LOCATION	*/
	std::map<std::string, std::string>	cgi_map;			/*	LOCATION/SERVER	*/
	std::vector<std::string>			charset;			/*	GLOBAL/SERVER/LOCATION	*/
	std::map<int, std::string> 			error_pages;		/*	GLOBAL/SERVER/LOCATION	*/
	std::vector<std::string>			index;				/*	GLOBAL/SERVER/LOCATION	*/
	std::vector<listen_s>				listen;				/*	SERVER	*/
	std::vector<CLocation>				locations;			/*	SERVER	*/
	long long							max_body;			/*	GLOBAL/SERVER/LOCATION	*/
	int									max_connections;	/*	GLOBAL/SERVER/LOCATION	*/
	long long							max_header_size;	/*	GLOBAL/SERVER/LOCATION	*/
	std::vector<std::string>			method;				/*	GLOBAL/SERVER/LOCATION	*/
	std::map<std::string, std::vector<std::string> > mime;				/*	GLOBAL */
	LocationMatchType_e					modifier;			/*	LOCATION	*/
	redirect_s							redirect;			/*	LOCATION	*/
	bool								redirect_status;	/*	LOCATION	*/	
	std::string							redirect_url;		/*	LOCATION	*/	
	std::string	 						root;				/*	GLOBAL/SERVER/LOCATION	*/		
	std::vector<CServer>				servers;			/*	GLOBAL/SERVER/LOCATION	*/
	std::vector<std::string>			server_names;		/*	SERVER	*/
	int									timeout_ka;			/*	GLOBAL/SERVER	*/		
	int									timeout_rw;			/*	GLOBAL/SERVER	*/
	tryFiles_s							try_files;			/*	LOCATION	*/
	std::string							upload_path;		/*	LOCATION	*/
	std::string							uri;				/*	LOCATION	*/

	bool	alias_set;
	bool	autoindex_set;
	bool	cgi_map_set;
	bool	charset_set;
	bool	error_page_set;
	bool	index_set;
	bool	listen_set;
	bool	location_set;
	bool	max_body_set;
	bool	max_connections_set;
	bool	max_header_set;
	bool	method_set;
	bool	mime_set;
	bool	modifier_set;
	bool	redirect_set;
	bool	redirect_status_set;
	bool	redirect_url_set;
	bool	root_set;
	bool	server_name_set;
	bool	servers_set;
	bool	timeout_ka_set;
	bool	timeout_rw_set;
	bool	try_files_set;
	bool	upload_path_set;
	bool	uri_set;

	tempConfig_s() : 
		  autoindex(false),
		  max_body(0),
		  max_connections(0),
		  max_header_size(0),
		  redirect_status(false),
		  timeout_ka(0),
		  timeout_rw(0),
		  alias_set(false),
		  autoindex_set(false),
		  cgi_map_set(false),
		  charset_set(false),
		  error_page_set(false),
		  index_set(false),
		  listen_set(false),
		  location_set(false),
		  max_body_set(false),
		  max_connections_set(false),
		  max_header_set(false),
		  method_set(false),
		  mime_set(false),
		  modifier_set(false),
		  redirect_set(false),
		  redirect_status_set(false),
		  redirect_url_set(false),
		  root_set(false),
		  server_name_set(false),
		  servers_set(false),
		  timeout_ka_set(false),
		  timeout_rw_set(false),
		  try_files_set(false),
		  upload_path_set(false),
		  uri_set(false)
		  {}
};

class CParser : public Parser
{
	enum HandlerContext_e{
		GLOBAL_HANDLER,
		SERVER_HANDLER,
		LOCATION_HANDLER
	};

	public:
						/* CONSTRUCTORS */
						CParser( const std::string& filename);

						/* DESTRUCTORS */
						~CParser( void );

		node_t*			parse(void);

		void			initKeywords(void);

		void			openConfigFile(const std::string& filename);
		void			parseConfigFile(Config& config);
		void			tokeniseStream(void);
		void			parseTokens(void);

		void			parseNext(node_t* parent, tokeniser_s awaited);
		node_t*			addNode(const std::string& node_type, const std::string& line_num);
		void			populateHandlers(void);
		bool			checkKeywords(const std::string& context, const std::string& value);
		bool			isValidMethod(const std::string& str);
		bool			isValidMime(const std::string& str);
		bool			isValidCharset(const std::string& str);
	
	private:

		std::ifstream	_stream;
		Tokeniser*		_tokeniser;
		node_t*			_rawConfig;

						/* CONSTRUCTORS */
						CParser( void );
						CParser( const CParser& other );

						/* COPY ASSIGNMENT */
		CParser&		operator=(const CParser& other);

		void			setConfig(Config& config);

		bool			setAlias(node_t* node, fileInfo_s& alias);
		bool			setBoolean(node_t* node, bool& state);
		bool			setCGImap(node_t* node, std::map<std::string, std::string>& cgi_map);
		bool			setCharset(node_t* node, std::vector<std::string>& charset);
		bool			setErrorPage(node_t* node, std::map<int, std::string>& error_pages);
		bool			setGlobalVar(node_t* node, Config& config);
		bool			setIndex(node_t* node, std::vector<std::string>& index);
		bool			setLocation(node_t* node, std::vector<CLocation>& locations);
		bool			setMethod(node_t* node, std::vector<std::string>& method);
		bool			setMime(node_t* node, std::map<std::string, std::vector<std::string> >& mime);
		bool			setMimeBlock(node_t* mime_node, std::map<std::string, std::vector<std::string> >& mime);
		bool			setModifier(std::string& str, LocationMatchType_e& modifier);
		bool			setPath(node_t* node, std::string& path);
		bool			setPorts(node_t* node, std::vector<listen_s>& ports);
		bool			setServer(node_t* node, std::vector<CServer>& servers);
		bool			setServerName(node_t* node, std::vector<std::string>& server_names);
		bool			setTryFiles(node_t* node, tryFiles_s& try_files);
		void			setVariable(tempConfig_s& tmp, node_t* node, HandlerContext_e context);
		
		void			pushToConfig(tempConfig_s& tmp, Config& dest);
		void			pushToServer(tempConfig_s& tmp, CServer& server);
		void			pushToLocation(tempConfig_s& tmp, CLocation& location);

		template<typename T>
		bool			setNumericValue(node_t* node, T& target, T min, T max);
		
						/* HANDLERS */
		void			handleAlias(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleAutoIndex(node_t* node, tempConfig_s& tmp);
		void			handleCharset(node_t* node, tempConfig_s& tmp);
		void			handleCGIMap(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleErrorPages(node_t* node, tempConfig_s& tmp);
		void			handleIndex(node_t* node, tempConfig_s& tmp);
		void			handleListen(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleLocation(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleMaxBody(node_t* node, tempConfig_s& tmp);
		void			handleMaxConnections(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleMaxHeader(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleMethod(node_t* node, tempConfig_s& tmp);
		//void			handleMime(node_t*, tempConfig_s& tmp, HandlerContext_e context);
		void			handleMimeBlock(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handlePorts(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleRedirectStatus(node_t* node, tempConfig_s& tmp, HandlerContext_e context); 
		void			handleRedirectURL(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleRoot(node_t* node, tempConfig_s& tmp);
		void			handleServer(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleServerNames(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleTimeoutKA(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleTimeoutRW(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleTryFiles(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
		void			handleUploadPath(node_t* node, tempConfig_s& tmp, HandlerContext_e context);
						
						/* VALIDATION */
		std::vector<std::string>	_global_keywords;
		std::vector<std::string>	_server_keywords;
		std::vector<std::string>	_location_keywords;
		std::vector<std::string>	_mime_keywords;

		bool			checkDuplicatePorts(const std::vector<listen_s>& ports);
		bool			checkAliasRoot(const std::string& alias, const std::string& root); //Check that only one of the two is present

		// To be written
		void 			validateConfig(Config& config);
		void 			validateServer(const CServer& server);
		void 			validateLocation(const CLocation& location);
	
};
