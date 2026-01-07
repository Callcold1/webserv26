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
#define NUM_SOCK 10

class Server {

	public:

		/* Constructors and destructor */
		Server(Config &config);
		Server(const Server& other);
		Server&	operator=( const Server& other);
		~Server(void);

		int get_socket(int index);
	private:

		void createSockets(int ports[]);

		int sockets[NUM_SOCK];

};