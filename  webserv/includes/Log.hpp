/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 13:05:00 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/14 15:37:08 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <ostream>
#include <ctime>

// LOG FILE PATHS
#define LOG_PATH_CONF 				"../logs/log_conf" 		 		// FOR STARTUP/SHUTDOWN/CONFIGURATION DETAILS
#define LOG_PATH_REQUEST_PARSING 	"../logs/log_request_parsing" 	// FOR REQUEST PARSING DETAILS
#define LOG_PATH_REQUESTS 			"../logs/log_request_status"	// FOR REQUEST DETAILS
#define LOG_PATH_ACCESS 			"../logs/log_access"			// FOR FILE ACCESS DETAILS
#define LOG_PATH_RESPONSE 			"../logs/log_response"			// FOR RESPONSE DETAILS
#define LOG_PATH_CGI 				"../logs/log_cgi"				// FOR CGI DETAILS
#define LOG_PATH_ERROR 				"../logs/log_errors"			// FOR ERROR DETAILS

class Log {

public:

	// LOG LEVELS
	enum LogLevel {
	    LOG_LEVEL_ERROR,
	    LOG_LEVEL_WARN,
	    LOG_LEVEL_INFO,
	    LOG_LEVEL_DEBUG
	};

	// LOG TYPES
		enum LogType {
	    LOG_TYPE_CONF,
		LOG_TYPE_REQUEST_PARSING,
	    LOG_TYPE_REQUESTS,
		LOG_TYPE_ACCESS,
		LOG_TYPE_RESPONSE,
	    LOG_TYPE_CGI,
	    LOG_TYPE_ERROR
	};

		static Log& 	instance();

		void			setLogLevel(LogLevel level);
		void			setOutput(std::ostream* out);

		void			log(LogLevel level, const LogType type,  const std::string& msg);
		void			log(LogLevel level, const LogType type, int fd, const std::string& msg);


private:

		std::ostream*	_out;
		std::fstream*	_conf;
		std::fstream*	_request_parsing;
		std::fstream*	_requests;
		std::fstream*	_access;
		std::fstream*	_response;
		std::fstream*	_cgi;
		std::fstream*	_errors;

		LogLevel		_level;
		LogType			_type;

						Log();
						Log(const Log&);
		Log&			operator=(const Log&);

		std::string		levelToString(LogLevel level);
		std::string 	typeToString(LogType type);
		void 			getFileStream(std::fstream* logfile, const LogType type);

		std::string		timestamp();

};

#endif