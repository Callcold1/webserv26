/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configDefs.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:37:44 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/18 17:42:46 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <iostream>
#include <ctime>
#include <map>
#include <stdint.h>
#include <vector>

// DEFAULT CONFIGURATION FILE (VALUES ARE ARBITARY JUST NOW)
#define DEFAULT_CONF 		"../default.conf"	// path to the default configuration file
#define DEFAULT_MAX_BODY 	1000000000			// default maximum body size (used when config file is provided, but property is missing)
#define DEFAULT_MAX_CONN 	1000				// default maximum connections to server (used when config file is provided, but property is missing)
#define DEFAULT_MAX_HEAD 	1000000000			// default maximum header size (used when config file is provided, but property is missing)
#define DEFAULT_TM_KA 		600					// default keep-alive timeout in seconds (used when config file is provided, but property is missing)
#define DEFAULT_TM_RW 		600					// default read-write timeout in seconds (used when config file is provided, but property is missing)

// SYSTEM MAXIMUMS (VALUES ARE ARBITARY JUST NOW)
#define SYS_MAX_BODY 		4000000000			// maximum permissible body size (if given size is higher, this value is applied)
#define SYS_MAX_CONN 		1024				// maximum permissible connections to server (if given size is higher, this value is applied)
#define SYS_MAX_HEAD 		4000000000			// maximum permissible body size (if given size is higher, this value is applied)
#define SYS_TM_KA 			1000				// maximum permissible keep-alive timeout in seconds (if given size is higher, this value is applied)
#define SYS_TM_RW 			1000				// maximum permissible read-write timeout in seconds (if given size is higher, this value is applied)

//class CServer; // Forward declaration
//class CLocation; // Forward declaration

struct errorPage_s {
	int			_error_num;
	std::string	_path;
	
	errorPage_s(void){}
	errorPage_s(int num, const std::string& path) : _error_num(num), _path(path){}
};

struct listen_s {
	
	uint32_t	ip;
	bool		is_default;
	uint16_t	port;

	listen_s(void){}
	listen_s(uint32_t ip_addr, bool default_port, uint16_t port_num):
		ip(ip_addr), is_default(default_port), port(port_num){};
};

struct redirect_s {
    int status;
    std::string url;

	redirect_s(void){}
	redirect_s(int stat, const std::string& url_str) : status(stat), url(url_str){}
};

struct tryFiles_s {
    std::vector<std::string> try_paths;
    int default_error;

	tryFiles_s(void){}
	tryFiles_s(int error) : default_error(error){}
};

enum LocationMatchType_e{
	EXACT,
	PREFIX,
	NONE
};