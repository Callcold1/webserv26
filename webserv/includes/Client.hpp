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

// TEMPORARY? 
// I THINK WE NEED SEPARATE STATE TRACKING FOR THE CONNECTION
// IF THE STATUS IS WRITE, THE SERVER NEEDS TO KNOW WHAT TO DO NEXT...
// HOWEVER, IF WE UPDATE THE STATE FROM WRITE TO CLOSE/ERROR/READ/CONNECTED AFTER HANDLING THEN eConn IS NOT REQUIRED
// COULD DO WITH A DEFAULT SETTING THAT ISN'T CLOSE OR KEEP-ALIVE?)
// IF WE REMOVE THIS, ALSO REMOVE getConn and _conn
enum eConn{
	CONN_CLOSE,
	CONN_KEEP_ALIVE
};

class Client
{
public:

	Client(Config* config);
	Client(Config* config, int new_fd);
	Client(listenInstance_s* listen_instance, int new_fd);
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
	eConn		getConn();
	//CServer*	findBestServerMatch(const std::string& host) const;

	void handle_request();
	
	private:
		
		int					fd;
		char				in_buff[1000];
		Request				request;
		Response*			response;
		std::string			out_buff;
		enum cState			_state;
		enum eConn			_conn;
		time_t				last_active;
		
		Config*				_config;
		listenInstance_s*	_listen_instance;
		CServer*			_matched_server;
		CLocation*			_matched_location;
		enum eParse			_p_state;
		enum eReqState		_r_state;
		bool				_keep_alive;		//update to true if header => Connection: keep-alive
		size_t				_read_cycles;		// Not sure if we need this, but used to allocate memory for a new request. Will also be required if we are queuing requests?
};
