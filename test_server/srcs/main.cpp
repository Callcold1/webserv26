/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 10:25:15 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/07 13:45:44 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <stdio.h>

/*

Basic server structure is as follows

socket()    → create socket
bind()      → attach address (IP + port)
listen()    → mark as passive
accept()    → accept incoming connection
recv()/send() → communicate
close()     → release socket

*/

int	main(){

	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int flags = fcntl(server_fd, F_GETFL, 0);
	fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = INADDR_ANY;
	
	socklen_t addrlen =sizeof(address);

	if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0) {
	    perror("bind failed");
	    return 1;
	}

	if (listen(server_fd, 5) < 0) {
	    perror("listen failed");
	    return 1;
	}
		
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