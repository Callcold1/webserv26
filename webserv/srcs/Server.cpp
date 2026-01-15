#include "Server.hpp"

//Server::Server(Config &config)
Server::Server()
{
	//createSockets(config.ports);
	int ports[] = {8080, 2020, 2025, 3030};
	for (int i = NUM_SOCK; i < NUM_SOCK + MAX_CONN; i++)
	{
		sockets[i].fd = -1;
		sockets[i].events = POLLIN | POLLOUT;
	}
	try
	{
		createSockets(ports);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Fatal: could not set up server: " << e.what() << "\nExiting..." << '\n';
		for (int i = 0; i < NUM_SOCK; i++)
		{
			if (sockets[i].fd != -1)
				close(sockets[i].fd);
		}
		exit(1);
	}
}

Server::~Server(void)
{
	for(int i = 0; i < NUM_SOCK + MAX_CONN; i++)
	{	
		if (sockets[i].fd != -1)
			close(sockets[i].fd);
	}
}

void	Server::createSockets(int ports[])
{
	for(int i = 0; i < NUM_SOCK; i++)
	{
		//creating new socket:
		int new_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (new_sock_fd == -1)
    		throw std::runtime_error("socket() failed!");
		//set flags:
		int flags = fcntl(new_sock_fd, F_GETFL, 0);
		if (flags == -1)
			throw std::runtime_error("fcntl(F_GETFL) failed");

		if (fcntl(new_sock_fd, F_SETFL, flags | O_NONBLOCK) == -1)
			throw std::runtime_error("fcntl(F_SETFL) failed");
		
		//binding to address:
		struct sockaddr_in address;
		
		address.sin_family = AF_INET;
		address.sin_port = htons(ports[i]);
		address.sin_addr.s_addr = INADDR_ANY;
		
		if (bind(new_sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		{
			perror("bind");
			throw(std::runtime_error("bind() failed!"));
		}
		
		if (listen(new_sock_fd, 5) < 0) //?
		{
			throw(std::runtime_error("listen() failed!"));
		}
		sockets[i].fd = new_sock_fd;
		sockets[i].events = POLLIN;
	}
}

void Server::conn_new_clients()
{
	int i, j;
	int	new_client = 0;

	for (i = 0; i < NUM_SOCK; i++)
	{
		if (sockets[i].revents & (POLLERR | POLLHUP | POLLNVAL))
		{
			close(sockets[i].fd);
			sockets[i].fd = -1;
			std::cout << "Invalid poll request, connection failed" << std::endl;
			continue;
		}
		if (sockets[i].revents & POLLIN)
		{
			if ((new_client = accept(sockets[i].fd, NULL, NULL)) == -1)
				continue;
			int flags = fcntl(new_client, F_GETFL, 0);
			if (flags == -1 || fcntl(new_client, F_SETFL, flags | O_NONBLOCK) == -1)
			{
				perror("fcntl");
				close(new_client);
				continue;
			}
			for (j = NUM_SOCK; j < NUM_SOCK + MAX_CONN; j++)
			{
				if (sockets[j].fd == -1)
				{
					sockets[j].fd = new_client;
					sockets[j].events = POLLIN;
					clients.insert(std::pair<int,Client>(new_client,Client(new_client)));
					break;
				}
			}
			if (j == NUM_SOCK + MAX_CONN)
			{
				std::cout << "Maximum connections reached, couldn't connect!" << std::endl; 
				close(new_client);
			}
			else
				std::cout << "client connected!" << std::endl;
		}
	}
}

void Server::client_action()
{
	char buffer[BUF_SIZE];

	for (int i = NUM_SOCK; i < NUM_SOCK + MAX_CONN; i++)
	{
		if (sockets[i].fd != -1 && (sockets[i].revents & POLLIN))
		{
			size_t valread = recv(sockets[i].fd, buffer, BUF_SIZE - 1, 0);
			if (valread == 0)
			{
				close(sockets[i].fd);
				clients.erase(sockets[i].fd);
				sockets[i].fd = -1;
				std::cout << "Client disconnected" << std::endl;
			}
			else if (valread < 0)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					continue;
				perror("read: ");
				close(sockets[i].fd);
				sockets[i].fd = -1;
				std::cout << "failed connection: Client was disconnected" << std::endl;
			}
			else
			{
				buffer[valread] = '\0';
				//send(clients[i].fd, buffer, valread, 0);
				clients.at(sockets[i].fd).append_read(buffer, valread);
				std::cout << sockets[i].fd << " said: " << buffer << std::endl;
			}
		} 
	}
}

int Server::loop()
{

	while (true)
	{
		int activity = poll(sockets, NUM_SOCK + MAX_CONN, -1);
		if (activity == -1)
		{
			perror("poll: ");
		}
		else if (activity > 0)
		{
			//handling new client connections:
			Server::conn_new_clients();
			//handling client actions:
			Server::client_action();
		}
	}
}


/*int server_setup_old(Server &server)
{
	int	clientSocket = 0;

	while (true){
		clientSocket = accept(server_fd, NULL, NULL);
		if (clientSocket > 0){
			while (true){
			
				char buffer[1024] = { 0 };
				if (clientSocket > 0){
					std::cout << "client connected : fd = " << clientSocket << std::endl;
					
					ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
					if (bytesRead != 0) {
						buffer[bytesRead] = '\0';
						std::cout << "Message from client: " << buffer << std::endl;
					}
					else
						break;
				}
			}
			close(clientSocket);
		}
	}
}*/
