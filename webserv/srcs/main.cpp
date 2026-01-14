/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jake <jake@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 10:25:15 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/13 17:51:56 by jake             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <cstring>
#include <iostream>

#include "Server.hpp"

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
	Server server;
	
	server.loop();
}