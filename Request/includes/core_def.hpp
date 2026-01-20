/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_def.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:51:39 by kmooney           #+#    #+#             */
/*   Updated: 2025/06/17 18:01:06 by kmooney          ###   ########.fr       */
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

/*
enum serv_action{
	//	SERVER TASKS
	AWAIT_CHUNKS,		//	Transfer-encoding chunked and last chunk not received
	CONTENT_READ,		//	If Content-Length is present, read the exact amount of data
	
	// SERVER FLAGS TO UPDATE
	CLOSE_CONNECTION,	//	Header - Connection : close
	KEEP_ALIVE,			//	Header - Connection : keep-alive
	CLOSE_ERROR,		//	Closed due to error
	
	// HANDLER TASKS
	// FURTHER PROCESSING
	CHUNK_DECODE,		//	If the request body uses chunked encoding, decode it before processing.
	DECOMPRESS_CONTENT,	//	If a Content-Encoding (gzip, deflate, etc.) is present, decompress the body before processing.
	HANDLER_COOKIES,	//	Handle cookies if we're dealing with
	HANDLER_RANGE,		//	Handle range request
	SAVE_CONTENT,		//	If the request is a file upload (POST/PUT), store the file properly.
	
	HANDLER_CGI,		//	Request is for CGI Route
	// RESONSE SIDE PROCESSING
	COMPRESS_CONTENT,	//	Compress response content
	RESPOND_CHUNKED,	//	IF response size is unknown, send chunked response
	REDIRECT,			//	If a redirection status (301, 302, 307, 308) is needed, return a response with a Location header
	
	// STRAIGHT TO RESPONSE?
	RESPOND_ERROR,		//	Generate simple HTML response - error
	RESPOND_ERROR_AUTH,	//	If an Authorization header is required but missing or incorrect, return 401 Unauthorized
	RESPOND_ERROR_PERM,	//	If the request is forbidden due to permissions (403) or an unsupported method (405), respond accordingly.
	RESPOND_HTML,		//	Send HTML response
	RESPOND_OTHER,		//	Send other format of response JSON?
	RESPOND_STATIC,		//	Request is for a static file
};
*/

typedef	int				fd_t;
typedef	std::string		str_t;

typedef std::map< str_t, str_t >StringMap_t;
typedef std::map< str_t, std::vector<str_t> > StrVecStrMap_t;

typedef void (*fStr)(str_t);
typedef void (*fStr2)(str_t, str_t);

typedef struct file_s {
	fd_t    	fd;
	__mode_t	perm;
	str_t   	filename;
	void		*data;
	file_s(){}
	file_s(str_t name) : filename(name){}
} file_t;

typedef struct log_s {
	void*			data;
	int				data_type;
	int				entry_type;
	
	log_s(void *data, int data_type, int log_type ) : data(data), data_type (data_type), entry_type(log_type){}
} log_t;

