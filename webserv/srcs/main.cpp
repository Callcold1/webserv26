/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junruh <junruh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 10:25:15 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/23 16:30:06 by junruh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include "CgiResponse.hpp"
#include "Config.hpp"
#include "CParser.hpp"
#include "Log.hpp"
#include "Server.hpp"

#define DEFAULT_PATH "default.conf"
#define TOO_MANY_ARGS "Error : too many arguments : the program can be started without arguments (default.conf will be used), or with a valid configuration file path"
#define NO_CLI_ARG "No config file path provided - attempting to use : "
#define ERR_CONFIG_NOT_OPEN "Error : the configuration file could not be opened : "


int	main()
{
	//Server server;
	
	//server.loop();

	// RediResponse rr(300);
	// ErrorResponse er(404);
	CgiResponse cr(200);
	cr.generate();
	

	
	return(0);
	
	return 0;
}