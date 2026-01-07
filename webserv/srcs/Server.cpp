#include "Server.hpp"

//Server::Server(Config &config){
Server::Server(Config& config){
		
	createSockets(config.ports);
}

Server::~Server( void )
{
	for(int i = 0; i < NUM_SOCK; i++)
	{	
		close(sockets[i]);
	}
}

void	Server::createSockets(int ports[]){
	for(int i = 0; i < NUM_SOCK; i++)
	{
		int new_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		int flags = fcntl(new_sock_fd, F_GETFL, 0);
		fcntl(new_sock_fd, F_SETFL, flags | O_NONBLOCK);
		
		
		struct sockaddr_in address;
		
		address.sin_family = AF_INET;
		address.sin_port = htons(ports[i]);
		address.sin_addr.s_addr = INADDR_ANY;
		
		if (bind(new_sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
			perror("bind failed");
			return;  // todo: error handling fix
		}
		
		if (listen(new_sock_fd, 5) < 0) {
			perror("listen failed");
			return;  // todo: error handling fix
		}
		sockets[i] = new_sock_fd;
	}
}

int server_setup(Server &server)
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
}
