/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junruh <junruh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 10:25:15 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/04 18:20:42 by junruh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <cstring>
#include <iostream>

#include "Server.hpp"
#include "ErrorResponse.hpp"
#include "RediResponse.hpp"
#include "StaticResponse.hpp"

/*

Basic server structure is as follows

socket()    → create socket
bind()      → attach address (IP + port)
listen()    → mark as passive
accept()    → accept incoming connection
recv()/send() → communicate
close()     → release socket

*/

int	main()
{
	//Server server;
	
	//server.loop();

	// RediResponse rr(300);
	// ErrorResponse er(404);

	StaticResponse sr(200);
	sr.generate();

	//r.print_response();
	
	return(0);
	
}