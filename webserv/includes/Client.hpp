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
enum eConn {
	CONN_CLOSE,
	CONN_KEEP_ALIVE
};

class Client {

	public:
								/* CONSTRUCTORS */
								//Client(Config* config);
								//Client(Config* config, int new_fd);
								Client(listenInstance_s* listen_instance, int new_fd);
								Client(const Client &other);
								Client &operator=(const Client &other);

								/* DESTRUCTOR + CLEANUP */
								~Client();
			void	 			clear_out_buff();

								/* STATE CHANGE */
			void 				change_state(enum cState state);

								/* GETTERS */
			eConn				getConn(void);
			std::string& 		get_msg();

								/* HANDLERS */
			bool				handleClientRead(const char *buffer, size_t len);
			int 				if_file_upload(Request &request);
			void				handle_request();

								/* WRITING */
			void 				append_write(std::string str);
			void				generateLogMessage(void);

	private:

		//	Config*				_config;
			listenInstance_s*	_listen_instance;
			int					_fd;

			CServer*			_matched_server;
			CLocation*			_matched_location;

			Request				_request;
			Response*			_response;
			//Response			_response;

			enum eConn			_conn;
			enum cState			_state;
			
			bool 				_has_output;
			time_t				_last_active;

			std::string			_log_msg;
			std::string			_out_buff;

};

std::ostream&	operator<<(std::ostream& os, const fieldMap_t headers);
std::ostream&	operator<<(std::ostream& os, Request& request);