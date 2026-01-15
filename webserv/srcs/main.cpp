/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 10:25:15 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/15 15:03:46 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include "Log.hpp"

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

	Log::instance().log(Log::INFO, Log::TYPE_CONF, "Logging Test");
	Log::instance().log(Log::INFO, Log::TYPE_CONF, "Logging Test 2");

}