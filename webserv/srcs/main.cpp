/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 10:25:15 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/16 17:16:37 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include "Config.hpp"
#include "ConfigParser.hpp"
#include "Log.hpp"

int	main(int argc, char** argv){

	std::string config_path;

	if (argc == 2)
		config_path = argv[1];

	else if (argc < 2) {
		Log::instance().log(Log::INFO, Log::TYPE_CONF, \
			"No config file path provided - attempting to use \"webserv/default.conf\"");
		config_path = "../default.conf";
	}

	else {
		Log::instance().log(Log::ERROR, Log::TYPE_ERROR, \
			"Too many arguments provided: the program can be started with no" \
				"argument (default.conf will be used), or with a valid configuration filepath");
		return 1;
	}

	try {
		//ConfigParser cp(config_path); // if this doesn't work try "./webserv/default.conf"
		Config* config = ConfigParser::parseConfigFile(config_path);
	}

	catch (std::exception* e){
		Log::instance().log(Log::ERROR, Log::TYPE_ERROR, e->what());
		return 1;
	}
	
	return 0;
}