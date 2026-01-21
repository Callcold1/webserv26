// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));

	int flags = fcntl(clientSocket, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("fcntl(F_GETFL) failed");

	if (fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl(F_SETFL) failed");

    // sending data
	std::string message;
	char msg[1024];

	while (true){
		std::cin >> message;
		if (message.length() > 0)
		{
    		send(clientSocket, message.c_str(), message.length(), 0);
			message = "";
		}
		else
			break;
		ssize_t valread = recv(clientSocket, msg, 1024, 0);
		if (valread == 0)
		{
			std::cout << "server disconnected" << std::endl;
		}
		else if (valread < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				continue;
			perror("read: ");
			std::cout << "failed connection: Client was disconnected" << std::endl;
		}
		else
		{
			msg[valread] = '\0';
			std::cout << "server said: " << msg << valread << std::endl;
		}
	}
    close(clientSocket);

    return 0;
}