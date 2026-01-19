/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:55:28 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/19 10:13:57 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// SERVER STRUCT
class ServerBlock {

	std::map<int, std::string>			_error_pages;
	std::vector<LocationBlock> 			_locations;				// Vector of location structs
	std::string 						_name;					// Server name
	int 								_port;					// Port that server will listen on
	std::string 						_root_dir;				// Root directory for server
	std::vector<std::string>			_server_methods;		// Methods permitted for all locations on server (can be superceded by location_methods)

} server_t;