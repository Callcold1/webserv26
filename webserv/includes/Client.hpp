#pragma once

#include <iostream>
#include <string>
#include <ctime>

#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"

enum State
{
	CONNECTED,
	READING_H,
	READING_B,
	PROCESSING,
	WRITING
};

class Client
{
public:

	Client(Config* config);
	Client(Config* config, int new_fd);
	Client(const Client &other);
	Client &operator=(const Client &other);
	
	~Client();

	void 		change_state(enum State state);
	void 		append_read(char *buffer, size_t len);
	void 		appendRequest(char *buffer, size_t len);
	void 		append_write(std::string str);
	std::string get_msg();
	void 		clear_out_buff();
	
	bool 		has_output;
	//Response	response;
	bool		isRequestComplete();

private:
	int				fd;
	char			in_buff[1000];
	Request			request;
	std::string		out_buff;
	enum State		state;
	time_t			last_active;

	Config*			_config;	
	CServer			_matched_server;
};
