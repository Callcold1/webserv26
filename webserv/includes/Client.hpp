#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>

#include "Config.hpp"
#include "Log.hpp"
#include "RequestTypes.hpp"
#include "Request.hpp"
#include "RequestValidator.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"

// Could keep this simple (READ, WRITE, ERROR)
// Alternatively, get very granular?
enum cState {
	CONNECTED,
	READ,
	WRITE,
	ERROR,
	CLOSE,
};

class Client
{
public:

	Client(Config* config);
	Client(Config* config, int new_fd);
	Client(const Client &other);
	Client &operator=(const Client &other);

	~Client();

	void 		change_state(enum cState state);
	void 		append_read(char *buffer, size_t len);
	void 		append_write(std::string str);
	std::string get_msg();
	void 		clear_out_buff();
	
	bool 		has_output;
	//Response	response;
	
	bool		handleClientRead(const char *buffer, size_t len);
	
	private:
		
		int				fd;
		char			in_buff[1000];
		Request			request;
		Request*		_request_ptr;
		std::string		out_buff;
		enum cState		_state;
		time_t			last_active;
		
		Config*			_config;
		CServer			_matched_server;
		enum eParse		_p_state;
		enum eReqState	_r_state;
		bool			_keep_alive;		//update to true if header => Connection: keep-alive
		size_t			_read_cycles;		// Not sure if we need this, but used to allocate memory for a new request. Will also be required if we are queuing requests?
};
