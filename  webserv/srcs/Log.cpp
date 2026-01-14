/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 13:35:51 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/14 15:41:42 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include <iostream>

Log::Log() : _out(&std::cerr), _level(LOG_LEVEL_INFO), _type(LOG_TYPE_CONF) {}

Log& Log::instance() {
    static Log instance;
    return instance;
}

void Log::setLogLevel(LogLevel level) {
    _level = level;
}

void Log::setOutput(std::ostream* out) {
    _out = out;
}

void Log::getFileStream(std::fstream* logfile, const LogType type){
		
	switch (type){
		 
	    case LOG_TYPE_CONF : {logfile = _conf; break; }
		case LOG_TYPE_REQUEST_PARSING : {logfile = _request_parsing; break; }
	    case LOG_TYPE_REQUESTS : {logfile = _requests; break; }
		case LOG_TYPE_ACCESS : {logfile = _access; break; }
		case LOG_TYPE_RESPONSE : {logfile = _response; break; }
	    case LOG_TYPE_CGI : {logfile = _cgi; break; }
	    case LOG_TYPE_ERROR : {logfile = _errors; break; }
		default : {logfile = NULL; break; }
	}
}

void Log::log(LogLevel level, const LogType type, const std::string& msg) {
    if (level > _level)
        return;

	std::fstream* logfile;
	
	 getFileStream(logfile, type);

	 // Printing to std::cerr and appropriate file : Create separate functions? 
    (*_out) << timestamp()
            << " [" << levelToString(level) << "] "
			<< " [" << typeToString(type) << "] "
            << msg << "\n";

	// Need to check logic for writing to file?
    (*logfile) << timestamp() 
            << " [" << levelToString(level) << "] "
			<< " [" << typeToString(type) << "] "
            << msg << "\n";
}

void Log::log(LogLevel level, const LogType type, int fd, const std::string& msg) {
    if (level > _level)
        return;

	std::fstream* logfile;
		
    (*_out) << timestamp()
            << " [" << levelToString(level) << "] "
            << "fd=" << fd << " "
            << msg << "\n";
}

std::string Log::levelToString(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_DEBUG: return "DEBUG";
    }
    return "UNKNOWN";
}

std::string Log::typeToString(LogType type) {
    switch (type) {
        case LOG_TYPE_CONF: return "ERROR";
        case LOG_TYPE_REQUEST_PARSING:  return "WARN";
        case LOG_TYPE_REQUESTS:  return "INFO";
        case LOG_TYPE_ACCESS: return "DEBUG";
        case LOG_TYPE_RESPONSE: return "DEBUG";
        case LOG_TYPE_CGI: return "DEBUG";
        case LOG_TYPE_ERROR: return "DEBUG";
    }
    return "UNKNOWN";
}

std::string Log::timestamp() {
    char buf[32];
    std::time_t now = std::time(NULL);
    std::tm* t = std::localtime(&now);

    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    return std::string(buf);
}

inline void log_conf(int fd, const std::string& msg){
	Log::instance().log(Log::LOG_LEVEL_DEBUG, Log::LOG_TYPE_CONF, fd, msg);
}

inline void log_request_parsing(int fd, const std::string& msg){
	Log::instance().log(Log::LOG_LEVEL_DEBUG, Log::LOG_TYPE_REQUEST_PARSING, fd, msg);
}

inline void log_requests(int fd, const std::string& msg){
	Log::instance().log(Log::LOG_LEVEL_DEBUG, Log::LOG_TYPE_REQUESTS, fd, msg);
}

inline void log_access(int fd, const std::string& msg){
	Log::instance().log(Log::LOG_LEVEL_DEBUG, Log::LOG_TYPE_ACCESS, fd, msg);
}

inline void log_response(int fd, const std::string& msg){
	Log::instance().log(Log::LOG_LEVEL_DEBUG, Log::LOG_TYPE_RESPONSE, fd, msg);
}

inline void log_cgi(int fd, const std::string& msg){
	Log::instance().log(Log::LOG_LEVEL_DEBUG, Log::LOG_TYPE_CGI, fd, msg);
}

inline void log_error(int fd, const std::string& msg){
	Log::instance().log(Log::LOG_LEVEL_ERROR, Log::LOG_TYPE_ERROR, fd, msg);
}
