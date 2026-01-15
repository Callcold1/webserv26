#pragma once

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

#define MAX_CONN 5
#define NUM_SOCK 10
#define BUF_SIZE 1024

class Server {

	public:

		/* Constructors and destructor */
		//Server(Config &config);
		Server();
		//Server(const Server& other);
		//Server&	operator=( const Server& other);
		~Server(void);

		//int get_socket(int index);
		int loop();
		void conn_new_clients();
		void client_action();
	private:

		void createSockets(int ports[]);

		struct pollfd sockets[NUM_SOCK + MAX_CONN];
		std::map<int, Client> clients;

};