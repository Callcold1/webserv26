#pragma once

#include "utilityFunctions.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <map>
#include <algorithm>
#include <poll.h>
#include <cerrno>
#include "Config.hpp"
#include "Client.hpp"

#define BUF_SIZE 1024

class CLocation;
class CServer;

class Server {

	struct listenInstance_s {
	    int fd;
	    uint16_t port;
	    uint32_t ip;
	    std::vector<CServer*> configs;
	    CServer* default_server;

	    listenInstance_s(CServer* server_config, int fd_val, const listen_s& listen)
	        : fd(fd_val), port(listen.port), ip(listen.ip), default_server(listen.is_default ? server_config : NULL){
	        
			configs.push_back(server_config);
	    }

	    listenInstance_s() : fd(0), port(0), ip(0), default_server(NULL) {}
	};

	public:

		/* Constructors and destructor */
		Server(Config &config);
		//Server();
		//Server(const Server& other);
		//Server&	operator=( const Server& other);
		~Server(void);

		//int get_socket(int index);
		int loop();
		void conn_new_clients();
		void client_read();
		void client_write();

		void update_poll_events();


		std::map<int, Client> 			_clients;				// Map of client file descriptors to Client objects, used for managing client connections and their associated data
		

	private:

		void	setupListeningSockets(Config& config);
		int		createListeningSocket(const listen_s& port_info);
		void	addListeningSocketsToPoll(void);
		void	addClientSockets(void);

		int 							_max_conn;				// The maximum number of client connections (from config file)
		int 							_port_count; 			// The sum of the ports from all server blocks
		std::vector<listenInstance_s>	_listen_sockets;		// Vector of listening socket info (fd, port, ip, server config(s) listening on that port)
		std::vector<pollfd> 			_sockets;				// _sockets position is == to _listeningServers position for listening sockets, and is == to client fd for client sockets
};
 