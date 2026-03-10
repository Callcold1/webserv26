/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configDefs.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:37:44 by kmooney           #+#    #+#             */
/*   Updated: 2026/03/06 14:09:08 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <iostream>
#include <ctime>
#include <map>
#include <stdint.h>
#include <vector>

class CServer;

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

struct listen_s {
	
	uint32_t	ip;
	bool		is_default;
	uint16_t	port;

	listen_s(void): ip((uint32_t)0), is_default(false), port((uint16_t)0){}
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

	tryFiles_s(void): default_error(0){}
	tryFiles_s(int error) : default_error(error){}
};

enum LocationMatchType_e{
	EXACT,
	PREFIX,
	NONE
};

/* 
	listenInstance_s : 

	fd 				: listening socket fd
	ip 				: listening socket ip
	port 			: listening socket port
	servers 		: servers listening on the ip:port
	def_srv 		: default server for that port (used if no hostname in a request, or hostname not found in name_to_srv map)
	def_srv_name 	: name of default server 
	name_to serv 	: maps a server name to a server
	
	*/
struct listenInstance_s {
    int 							fd;
    uint32_t 						ip;
    uint16_t 						port;
    CServer* 						def_srv;
    std::vector<CServer*> 			servers;
	std::string 					def_srv_name;
	std::map<std::string, CServer*> name_to_srv;

    listenInstance_s( int fd_val, const listen_s& l, std::vector<CServer*>& srvs)
        : fd(fd_val), ip(l.ip), port(l.port), def_srv(NULL), servers(srvs){
    }

    listenInstance_s() : fd(0), ip(0), port(0), def_srv(NULL) {}
};