#include "Server.hpp"

//Server::Server(Config &config){
Server::Server(Config& config){
		
	createSockets(config.ports);
}

Server::~Server( void )
{
}

void	Server::createSockets(int ports[]){
	for(int i = 0; i < 5; i++)
	{
		sock_pair new_pair;
		new_pair.first = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		int flags = fcntl(new_pair.first, F_GETFL, 0);
		fcntl(new_pair.first, F_SETFL, flags | O_NONBLOCK);
		
		
		struct sockaddr_in address;
		
		new_pair.second.sin_family = AF_INET;
		new_pair.second.sin_port = htons(ports[i]);
		new_pair.second.sin_addr.s_addr = INADDR_ANY;
		
		if (bind(new_pair.first, (struct sockaddr *)&new_pair.second, sizeof(new_pair.second)) < 0) {
			perror("bind failed");
			return;
		}
		
		if (listen(new_pair.first, 5) < 0) {
			perror("listen failed");
			return;
		}
		sockets[i] = new_pair;
	}
}

int server(void)
{
	
	int	clientSocket = 0;

	while (true){
		clientSocket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
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
	close(server_fd);

}
