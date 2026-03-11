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

	public:

	/* Constructors and destructor */
		Server(Config &config);
		//Server();
		//Server(const Server& other);
		//Server&	operator=( const Server& other);
		~Server(void);

		void 	loop();

		std::map<int, Client> _clients;				// Map of client file descriptors to Client objects, used for managing client connections and their associated data
		
	private:

		void	setupListeningSockets(Config& config);
		int		createListeningSocket(const listen_s& port_info);
		void	addListeningSocketsToPoll(void);

		void	listenInstanceCreate(int fd, listen_s listen, std::vector<CServer*>& servers);
		bool	listenInstanceDefaultServer(std::vector<listen_s> listens, listen_s target);
		void	listenInstanceNameServerMap(std::map<std::string, CServer* >& map, CServer* server, std::vector<std::string> names);
		void	setDefaultServerForSocket(CServer* def_srv, uint32_t ip, uint16_t port, std::vector<CServer*>& servers );


		void	handleEvents(void);
		
		void	handleAccept(int index);
		void	setNonBlocking(int fd);
		void 	addClient(int client_fd, int index);
		
		void	handleClientRead(int fd);
		void	handleClientWrite(int fd);
		
		void	disconnectClient(int fd, const std::string& reason);
		void	removeClientFromPoll(int fd);

		int 	getSocketsPosition(int fd);

		Config*							_config;				// Reference to the server's configuration object	
		bool							_running;				// Indicates if the server is currently running	
		int 							_max_conn;				// The maximum number of client connections (from config file)
		int 							_port_count; 			// The sum of the ports from all server blocks
		std::vector<listenInstance_s>	_listen_sockets;		// Vector of listening socket info (fd, port, ip, server config(s) listening on that port)
		std::vector<pollfd> 			_sockets;				// _sockets position is == to _listeningServers position for listening sockets, and is == to client fd for client sockets

};

std::ostream&	operator<<(std::ostream& os, const listenInstance_s& li);
std::ostream&	operator<<(std::ostream& os, const std::map<std::string, CServer*>& map);