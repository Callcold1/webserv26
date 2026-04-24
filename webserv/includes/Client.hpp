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

enum cState {
	CONNECTED,
	READ,
	WRITE,
	ERROR,
	CLOSE,
};

enum eConn {
	CONN_CLOSE,
	CONN_KEEP_ALIVE
};

class Client {

	public:
								/* CONSTRUCTORS */
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
			int			 		getFD();
			int					getServerTimeout();
			time_t				getLastActivity();

								/* HANDLERS */
			bool				handleClientRead(const char *buffer, size_t len);
			void				handleZeroRead(void);
			int 				if_file_upload(Request &request);
			void				handle_request();

								/* WRITING */
			std::string			getStatusString(void);
			void 				append_write(std::string str);
			void				generateLogMessage(void);
								/* SETTERS */
			void				setLastActivity();

	private:

			listenInstance_s*	_listen_instance;
			int					_fd;

			const CServer*		_matched_server;
			const CLocation*	_matched_location;

			Request				_request;
			Response*			_response;

			enum eConn			_conn;
			enum cState			_state;

			bool 				_has_output;
			time_t				_last_active;

			std::string			_log_msg;
			std::string			_out_buff;

};

std::ostream&	operator<<(std::ostream& os, const fieldMap_t headers);
std::ostream&	operator<<(std::ostream& os, Request& request);
std::ostream&	operator<<(std::ostream& os, const std::vector<std::pair <fileInfo_s, std::string> >& upload_paths);
std::ostream&	operator<<(std::ostream& os, const reqPaths_s& paths);
std::ostream&	operator<<(std::ostream& os, const respond_t& re);

std::string	printReqStatus(Request& _request, int fd);
