/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 13:05:00 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/15 12:42:00 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <fstream>
#include <ctime>

// LOGS FOLDER PATH
#define LOG_PATH "./logs/"

// LOG FILE PATHS
#define LOG_ACCESS 				"access.log"			// FOR FILE ACCESS DETAILS
#define LOG_CGI 				"cgi.log"				// FOR CGI DETAILS
#define LOG_CLASS	 			"log_class.log"			// FOR LOG OF LOGGING
#define LOG_CONF 				"conf.log" 		 		// FOR STARTUP/SHUTDOWN/CONFIGURATION DETAILS
#define LOG_ERROR 				"errors.log"			// FOR ERROR DETAILS
#define LOG_REQUEST_PARSING 	"request_parsing.log" 	// FOR REQUEST PARSING DETAILS
#define LOG_REQUESTS 			"request_status.log"	// FOR REQUEST DETAILS
#define LOG_RESPONSE 			"response.log"			// FOR RESPONSE DETAILS

class Log {

public:

	// LOG LEVELS
	enum LogLevel {
		ERROR,
		WARN,
		INFO,
		DEBUG
	};

	// LOG TYPES
	enum LogType {
		TYPE_ACCESS,
		TYPE_CGI,
		TYPE_CLASS,
		TYPE_CONF,
		TYPE_ERROR,
		TYPE_REQUEST_PARSING,
		TYPE_REQUESTS,
		TYPE_RESPONSE
	};

		static Log& 	instance(void);

		void			log(LogLevel level, const LogType type,  const std::string msg);
		void			log(LogLevel level, const LogType type, int fd, const std::string msg);

private:

		std::ostream*	_out;
		int				_access_fd;
		int				_cgi_fd;
		int				_class_fd;
		int				_conf_fd;
		int				_errors_fd;
		int				_request_parsing_fd;
		int				_requests_fd;
		int				_response_fd;

		std::string		_base_name;
		std::string		_base_path;

						// CONSTRUCTORS
						Log();
						~Log();
						Log(const Log&);
		Log&			operator=(const Log&);

						// LOG FILE MANIPULATION
		void			openLogFiles(void);
		void			openLogFile(std::string filename, int& fd);
		bool			file_exists(const std::string& filename) const;
		void			rotate();
		std::string 	rotatedName(int index) const;
		void			closeLogFiles(void);

						// CONVERT ENUMS TO STRINGS FOR OUTPUT
		std::string		levelToString(LogLevel level);
		std::string 	typeToString(LogType type);
		
						// SET LOG
		void			setLogLevel(LogLevel level);
		void			setLogType(LogType type);
		
						// SET OUTPUT DESTINATIONS
		int				setOutput(int type);

		std::string		timestamp();

};

std::string	itoa(int value);
bool		folderExists(const std::string &path);
bool		createLogFolder(void);

#endif