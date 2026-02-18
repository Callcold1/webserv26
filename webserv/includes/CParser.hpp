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

class CParser : public Parser
{
	public:
						/* CONSTRUCTORS */
						CParser( const std::string& filename);

						/* DESTRUCTORS */
						~CParser( void );

		node_t*			parse(void);

		void			openConfigFile(const std::string& filename);
		void			parseConfigFile(Config& config);
		void			tokeniseStream(void);
		void			parseTokens(void);

		void			parseNext(node_t* parent, token_t awaited);
		node_t*			addNode(const std::string& node_type);
		void			populateKeywords(void);
		bool			checkKeywords(const std::string& context, const std::string& value);
		bool			isValidMethod(const std::string& str);
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

		void			setAlias(node_t* node, fileInfo_s& alias);
		void			setBoolean(node_t* node, bool& state);
		void			setCGImap(node_t* node, std::map<std::string, std::string>& cgiMap);
		void			setCharsets(node_t* node, std::vector<std::string>& charset);
		void			setErrorPage(node_t* node, std::vector<errorPage_s>& error_pages);
		void			setGlobalVar(node_t* node, Config& config);
		void			setIndex(node_t* node, std::vector<std::string>& index);
		void			setLocation(node_t* node, std::vector<CLocation>& locations);
		void			setMethods(node_t* node, std::vector<std::string>& methods);
		void			setPath(node_t* node, std::string& path);
		void			setPorts(node_t* node, std::vector<listen_s>& ports);
		void			setServer(node_t* node, std::vector<CServer>& servers);
		void			setServerNames(node_t* node, std::vector<std::string>& server_names);
		
		template<typename T>
		void			setNumericValue(node_t* node, T& target, T min, T max);

						/* VALIDATION */
		std::vector<std::string>	_global_keywords;
		std::vector<std::string>	_server_keywords;
		std::vector<std::string>	_location_keywords;

		bool			checkAliasRoot(const std::string& alias, const std::string& root); //Check that only one of the two is present
		void			setModifier(std::string& str, LocationMatchType_e& modifier);
};

template <typename T>
std::string toString(const T val);
