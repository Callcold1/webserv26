/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 13:35:51 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/16 14:18:17 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include <sstream>

Log::Log() : _out(&std::cerr){
	createLogFolders();
	openLogFiles();
	log(INFO, TYPE_CLASS, "Log files opened successfully", false);
}

Log::~Log(){
	closeLogFiles();
}

bool	Log::createLogFolders(void){

	if (!createFolder(LOG_PATH))
		return false;
	if (!createFolder(LOG_CURRENT))
		return false;
	if (!createFolder(LOG_OLD))
		return false;
	return true;
}

bool	Log::createFolder(std::string path){
	if (path.empty())
		return false;

	std::string current;
	size_t i = 0;

	if (path[0] == '/')
	{
		current = "/";
		i = 1;
	}
	while (i < path.size())
	{
		size_t slash = path.find('/', i);

		if (slash == std::string::npos)
			slash = path.size();

		current += path.substr(i, slash - i);

		struct stat st;
		if (stat(current.c_str(), &st) != 0)
		{
			if (mkdir(current.c_str(), 0755) != 0)
				return false;
		}
		else if (!S_ISDIR(st.st_mode))
			return false;
			
		current += "/";
		i = slash + 1;
	}
	return true;
}

// OPEN ALL LOG FILES
void	Log::openLogFiles(void){
	openLogFile(LOG_ACCESS, _access_fd);
	openLogFile(LOG_CGI, _cgi_fd);
	openLogFile(LOG_CLASS, _class_fd);
	openLogFile(LOG_CONF, _conf_fd);
	openLogFile(LOG_ERROR, _errors_fd);
	openLogFile(LOG_REQUEST_PARSING, _request_parsing_fd);
	openLogFile(LOG_REQUESTS, _requests_fd);
	openLogFile(LOG_RESPONSE, _response_fd);
}

// CLOSE ALL LOG FILES
void	Log::closeLogFiles(void){
	if (_access_fd >= 0) close(_access_fd);
	if (_cgi_fd >= 0) close(_cgi_fd);
	if (_class_fd >= 0) close(_class_fd);
	if (_conf_fd >= 0) close(_conf_fd);
	if (_request_parsing_fd >= 0) close(_request_parsing_fd);
	if (_requests_fd >= 0) close(_requests_fd);
	if (_response_fd >= 0) close(_response_fd);
	if (_errors_fd >= 0) close(_errors_fd);
}

// OPEN A SINGLE LOG FILE
void	Log::openLogFile(std::string filename, int& fd){
	
	_base_name = filename;
	_base_path = LOG_CURRENT + filename;

	if (file_exists())
		rotate();
	fd = open(_base_path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
}

// CHECK IF A LOG FILE ALREADY EXISTS
bool	Log::file_exists(const std::string& filepath) const{
	return (access(filepath.c_str(), F_OK) == 0);
}

// CHECK IF A LOG FILE ALREADY EXISTS
bool	Log::file_exists(void) const{
	return (access(_base_path.c_str(), F_OK) == 0);
}

// INCREASE INDEX OF EXISTING LOG FILES 
void Log::rotate()
{
	_old_path = LOG_OLD + _base_name;
	int max = 1;

	while (file_exists(rotatedName(max)))
		++max;

	for (int i = max; i > 0; --i)
		rename(rotatedName(i).c_str(), rotatedName(i + 1).c_str());
	rename(_base_path.c_str(), rotatedName(1).c_str());
}

// CREATE LOG FILE NAME (APPENDED WITH INDEX)
std::string Log::rotatedName(int index) const
{
	std::ostringstream oss;
	oss << _old_path << "." << index;
	return oss.str();
}

// STATIC FUNCTION TO ACCESS CLASS
Log& Log::instance() {
	static Log instance;
	return instance;
}

int Log::setOutput(int type) {
	
	int log_fd = 0;

	switch (type){
		case TYPE_ACCESS : {log_fd = _access_fd; break; }
		case TYPE_CGI : {log_fd = _cgi_fd; break; }
		case TYPE_CLASS : {log_fd = _class_fd; break; }
		case TYPE_CONF : {log_fd = _conf_fd; break; }
		case TYPE_ERROR : {log_fd = _errors_fd; break; }
		case TYPE_REQUEST_PARSING : {log_fd = _request_parsing_fd; break; }
		case TYPE_REQUESTS : {log_fd = _requests_fd; break; }
		case TYPE_RESPONSE : {log_fd = _response_fd; break; }
		default : { break; }
	}
	return log_fd;
}

void Log::log(LogLevel level, const LogType type, const std::string msg, bool print) {

	int log_fd = setOutput(type);

	std::string log_line = timestamp() + " [" + levelToString(level) + "] "
					   + " [" + typeToString(type) + "] " + msg + "\n";

	write(log_fd, log_line.c_str(), log_line.size()); 	// write to file
	if (print)
		(*_out) << log_line; 								// write to stderr
}

void Log::log(LogLevel level, const LogType type, int fd, const std::string msg) {

	int log_fd = setOutput(type);
	
	std::string log_line = timestamp() + " [" + levelToString(level) + "] "
					   + "fd=" + intToString(fd) + " [" + typeToString(type) + "] "
					   + msg + "\n";

	write(log_fd, log_line.c_str(), log_line.size()); 	// write to file
	(*_out) << log_line; 								// write to stderr
}

std::string Log::levelToString(LogLevel level) {
	switch (level) {
		case ERROR: return "ERROR";
		case WARN:  return "WARN";
		case INFO:  return "INFO";
		case DEBUG: return "DEBUG";
	}
	return "UNKNOWN";
}

std::string Log::typeToString(LogType type) {
	switch (type) {
		case TYPE_ACCESS: return "TYPE_ACCESS";
		case TYPE_CGI: return "TYPE_CGI";
		case TYPE_CLASS: return "TYPE_CLASS";
		case TYPE_CONF: return "TYPE_CONF";
		case TYPE_ERROR: return "TYPE_ERROR";
		case TYPE_REQUEST_PARSING:  return "TYPE_REQUEST_PARSING";
		case TYPE_REQUESTS:  return "TYPE_REQUESTS";
		case TYPE_RESPONSE: return "TYPE_RESPONSE";
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

std::string intToString(int value)
{
	std::ostringstream oss;
	oss << value;
	std::cout << oss.str();
	return oss.str();
}