/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 15:18:33 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/16 17:41:51 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "utility_functions.hpp"
#include <fstream>
#include <sstream>

class ConfigParser
{
	public:
									/* CONSTRUCTORS */
									ConfigParser( void );

									/* DESTRUCTORS */
									~ConfigParser( void );
	
									/* COPY ASSIGNMENT */
		ConfigParser&				operator=(const ConfigParser& other);

		static Config*				parseConfigFile(std::string& filename);
	
	private:
									/* COPY CONSTRUCTOR */
									ConfigParser(const ConfigParser& other);

		template<typename T>
		T							parseValue(const std::string &line);
		
		void						parseGlobalAttributes(Config* config, std::ifstream& stream);
		std::vector<std::string>	parseMethods(const std::string &line);

		void						parseServerBlock(Config* config, std::ifstream& stream);
		
		void						parseLocation(Config* config, std::ifstream& file, std::string& oldLine);
		std::string 				parseLocationPath(const std::string &line);
};
