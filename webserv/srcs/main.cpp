/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 10:25:15 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/22 17:44:26 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include "Config.hpp"
#include "CParser.hpp"
#include "Log.hpp"
#include "Server.hpp"

#define DEFAULT_PATH "default.conf"
#define TOO_MANY_ARGS "Error : too many arguments : the program can be started without arguments (default.conf will be used), or with a valid configuration file path"
#define NO_CLI_ARG "No config file path provided - attempting to use : "
#define ERR_CONFIG_NOT_OPEN "Error : the configuration file could not be opened : "

int	main(int argc, char** argv){

	std::string config_path;

	if (argc == 2)
		config_path = argv[1];

	else if (argc < 2) {
		config_path = DEFAULT_PATH;
		Log::instance().log(Log::INFO, Log::TYPE_CONF, NO_CLI_ARG + config_path);
	}

	else {
		Log::instance().log(Log::ERROR, Log::TYPE_ERROR, TOO_MANY_ARGS);
		return 1;
	}

	try {
		CParser p(config_path);
		Config config;

		p.parseConfigFile(config);
		Server server(config);
		server.loop();
	}

	catch (const std::exception& e){
		Log::instance().log(Log::ERROR, Log::TYPE_ERROR, e.what());
		return 1;
	}
	
	return 0;
}