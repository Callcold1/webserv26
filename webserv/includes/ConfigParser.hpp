/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 15:18:33 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 16:00:19 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "utility_functions.hpp"
#include <fstream>
#include <sstream>

enum config_parser_state{
	CONFIG_PARSER_COMMENT,
	CONFIG_PARSER_COMPLETE,
	CONFIG_PARSER_ERROR,
	CONFIG_PARSER_GLOBAL,
	CONFIG_PARSER_HTTP,
	CONFIG_PARSER_LOCATION,
	CONFIG_PARSER_SERVER,
	CONFIG_PARSER_VALIDATION
};

class ConfigParser
{
	public:
									/* CONSTRUCTORS */
									ConfigParser( void );
									ConfigParser( const ConfigParser& other );

									/* DESTRUCTORS */
									~ConfigParser( void );
	
									/* COPY ASSIGNMENT */
		ConfigParser&				operator=(const ConfigParser& other);

		static Config&				parseConfigFile(std::string& filename);
	
	private:

									/* COPY CONSTRUCTOR */
									ConfigParser(const ConfigParser& other);

		template<typename T>
		T							parseValue(const std::string &line);
		
		static void					parseComment(Config& config, std::ifstream& stream);
		static void					parseGlobalAttributes(Config& config, std::ifstream& stream);
		static void					parseHttpBlock(Config& config, std::ifstream& stream);
		static void					parseLocationBlock(Config& config, std::ifstream& stream);
		static void					parseServerBlock(Config& config, std::ifstream& stream);
		
		std::vector<std::string>	parseMethods(const std::string &line);
		std::string 				parseLocationPath(const std::string &line);

									// Validation

};
