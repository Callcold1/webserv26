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

#include "Config.hpp"

#define MAX_CONN 5

typedef std::pair<int, sockaddr_in> sock_pair;

class Server {

	public:

		/* Constructors and destructor */
		Server(Config &config);
		Server(const Server& other);
		Server&	operator=( const Server& other);
		~Server(void);

	private:

		void createSockets(int ports[]);

		sock_pair sockets[10];

};