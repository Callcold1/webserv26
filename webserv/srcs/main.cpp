/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jake <jake@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 10:25:15 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/03 21:06:53 by jake             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <cstring>
#include <iostream>

#include "Server.hpp"
#include "ErrorResponse.hpp"
#include "RediResponse.hpp"

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

	RediResponse r(404);

	r.print_response();
	
	return(0);
	
}