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
	std::map<std::string, std::string>	cgi_map;			/*	LOCATION	*/
	std::vector<std::string>			charsets;			/*	GLOBAL/SERVER/LOCATION	*/
	std::vector<errorPage_s> 			error_pages;		/*	GLOBAL/SERVER/LOCATION	*/
	std::vector<std::string>			index;				/*	GLOBAL/SERVER/LOCATION	*/
	std::vector<listen_s>				listen;				/*	SERVER	*/
	std::vector<CLocation>				locations;			/*	SERVER	*/
	long long							max_body;			/*	GLOBAL/SERVER/LOCATION	*/
	int									max_connections;	/*	GLOBAL/SERVER/LOCATION	*/
	long long							max_header_size;	/*	GLOBAL/SERVER/LOCATION	*/
	std::vector<std::string>			methods;			/*	GLOBAL/SERVER/LOCATION	*/
	LocationMatchType_e					modifier;			/*	LOCATION	*/
	redirect_s							redirect;			/*	LOCATION	*/
	bool								redirect_status;	/*	LOCATION	*/	
	std::string							redirect_url;		/*	LOCATION	*/	
	std::string	 						root;				/*	GLOBAL/SERVER/LOCATION	*/		
	std::vector<CServer>				servers;			/*	GLOBAL/SERVER/LOCATION	*/
	std::vector<std::string>			server_names;		/*	SERVER	*/
	int									timeout_ka;			/*	GLOBAL/SERVER	*/		
	int									timeout_rw;			/*	GLOBAL/SERVER	*/
	std::vector<tryFiles_s>				try_files;			/*	LOCATION	*/
	std::string							upload_path;		/*	LOCATION	*/
	std::string							uri;				/*	LOCATION	*/

	bool	alias_set;
	bool	autoindex_set;
	bool	cgi_map_set;
	bool	charset_set;
	bool	error_pages_set;
	bool	index_set;
	bool	listen_set;
	bool	location_set;
	bool	max_body_set;
	bool	max_connections_set;
	bool	max_header_set;
	bool	methods_set;
	bool	modifier_set;
	bool	redirect_set;
	bool	redirect_status_set;
	bool	redirect_url_set;
	bool	root_set;
	bool	server_names_set;
	bool	servers_set;
	bool	timeout_ka_set;
	bool	timeout_rw_set;
	bool	try_files_set;
	bool	upload_path_set;
	bool	uri_set;

	tempConfig_s() : 
		  autoindex(false),
		  max_body(0),
		  max_header_size(0),
		  timeout_ka(0),
		  timeout_rw(0),
		  alias_set(false),
		  autoindex_set(false),
		  cgi_map_set(false),
		  charset_set(false),
		  error_pages_set(false),
		  index_set(false),
		  listen_set(false),
		  location_set(false),
		  max_body_set(false),
		  max_connections_set(false),
		  max_header_set(false),
		  methods_set(false),
		  redirect_status(false),
		  modifier_set(false),
		  redirect_set(false),
		  redirect_status_set(false),
		  redirect_url_set(false),
		  root_set(false),
		  server_names_set(false),
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
		node_t*			addNode(const std::string& node_type);
		void			populateHandlers(void);
		bool			checkKeywords(const std::string& context, const std::string& value);
		bool			isValidMethod(const std::string& str);
		bool			isValidCharset(const std::string& str);

	private:

		std::ifstream							_stream;
		Tokeniser*								_tokeniser;
		node_t*									_rawConfig;

						/* CONSTRUCTORS */
						CParser( void );
						CParser( const CParser& other );

						/* COPY ASSIGNMENT */
		CParser&		operator=(const CParser& other);

		void			setConfig(Config& config);

		bool			setAlias(node_t* node, fileInfo_s& alias);
		bool			setBoolean(node_t* node, bool& state);
		bool			setCGImap(node_t* node, std::map<std::string, std::string>& cgi_map);
		bool			setCharsets(node_t* node, std::vector<std::string>& charsets);
		bool			setErrorPage(node_t* node, std::vector<errorPage_s>& error_pages);
		bool			setGlobalVar(node_t* node, Config& config);
		bool			setIndex(node_t* node, std::vector<std::string>& index);
		bool			setLocation(node_t* node, std::vector<CLocation>& locations);
		bool			setMethods(node_t* node, std::vector<std::string>& methods);
		bool			setModifier(std::string& str, LocationMatchType_e& modifier);
		bool			setPath(node_t* node, std::string& path);
		bool			setPorts(node_t* node, std::vector<listen_s>& ports);
		bool			setServer(node_t* node, std::vector<CServer>& servers);
		bool			setServerNames(node_t* node, std::vector<std::string>& server_names);
		void			setVariable(tempConfig_s& tmp, node_t* node, HandlerContext_e context);
		
		template<typename T>
		bool			setNumericValue(node_t* node, T& target, T min, T max);

						/* HANDLERS */
	
		template <typename T>
		bool			handleDirective(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleAlias(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleAutoIndex(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleCharsets(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleCGIMap(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleErrorPages(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleIndex(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleLocation(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleModifier(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleMaxBody(node_t* node, T& tmp, HandlerContext_e context);
		
		template <typename T>
		void			handleMaxConnections(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleMaxHeader(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleMethods(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handlePorts(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleRedirect(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T> 
		void			handleRedirectStatus(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T> 
		void			handleRedirectURL(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleRoot(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleServerNames(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleTimeoutKA(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T>
		void			handleTimeoutRW(node_t* node, T& tmp, HandlerContext_e context);

		template <typename T> 
		void			pushToConfig(tempConfig_s& tmp, T& dest, HandlerContext_e context);
		// void			pushToServer(tempConfig_s& tmp, CServer& server);
		// void			pushToLocation(tempConfig_s& tmp, CLocation& location);

						/* VALIDATION */
		std::vector<std::string>	_global_keywords;
		std::vector<std::string>	_server_keywords;
		std::vector<std::string>	_location_keywords;

		bool			checkAliasRoot(const std::string& alias, const std::string& root); //Check that only one of the two is present
};

template <typename T>
std::string toString(const T val);
