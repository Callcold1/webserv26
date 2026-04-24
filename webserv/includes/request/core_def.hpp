/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_def.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:51:39 by kmooney           #+#    #+#             */
/*   Updated: 2026/04/24 17:40:57 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <cstddef>
#include <deque>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <set>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <utility>
#include <vector>

#define MESS_BUFFER_SIZE 8192 // Size of message buffer
#define MAX_URI_LENGTH 4096
#define MAX_CONTENT_LENGTH 30000000
#define CONNECTION_TIMEOUT 30 // Timeout time in seconds if client sends no data
#define CHUNK_MAX_SIZE 4196

typedef	int				fd_t;
typedef	std::string		str_t;

typedef std::map< str_t, str_t >StringMap_t;
typedef std::map< str_t, std::vector<str_t> > StrVecStrMap_t;

typedef void (*fStr)(str_t);
typedef void (*fStr2)(str_t, str_t);
