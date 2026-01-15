/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 13:35:51 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/15 12:52:21 by kmooney          ###   ########.fr       */
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
	createLogFolder();
	openLogFiles();
	log(INFO, TYPE_CLASS, "Log files opened successfully");
}

Log::~Log(){
	closeLogFiles();
}

bool	createLogFolder(void){

	mode_t mode = 0755;

	if (folderExists(LOG_PATH))
		return true;
	
	if (mkdir(LOG_PATH, mode) != 0)
        return false;						// directory creation failed
    return true;							// directory created
}

bool	folderExists(const std::string &path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;						// folder doesn't exist
    return (info.st_mode & S_IFDIR) != 0;	// check if it's a directory
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
	_base_path = LOG_PATH + filename;

	if (file_exists(_base_path)){
		rotate();
	}
	fd = open(_base_path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
}

// CHECK IF A LOG FILE ALREADY EXISTS
bool	Log::file_exists(const std::string& filepath) const{
	return (access(filepath.c_str(), F_OK) == 0);
}

// INCREASE INDEX OF EXISTING LOG FILES 
void Log::rotate()
{
	int max = 1;

	while (file_exists(rotatedName(max)))
		++max;

	for (int i = max - 1; i >= 1; --i)
		rename(rotatedName(i).c_str(), rotatedName(i + 1).c_str());
	rename(_base_name.c_str(), rotatedName(1).c_str());
}

// CREATE LOG FILE NAME (APPENDED WITH INDEX)
std::string Log::rotatedName(int index) const
{
	std::ostringstream oss;
	oss << _base_name << "." << index;
	return oss.str();
}

// CREATE STATIC INSTANCE OF CLASS
Log& Log::instance() {
	static Log instance;
	return instance;
}

int Log::setOutput(int type) {
	
	int log_fd = 0;

	switch (type){
		case TYPE_ACCESS : {log_fd = _access_fd; break; }
		case TYPE_CGI : {log_fd = _cgi_fd; break; }
		case TYPE_CLASS : {log_fd = _cgi_fd; break; }
		case TYPE_CONF : {log_fd = _conf_fd; break; }
		case TYPE_ERROR : {log_fd = _errors_fd; break; }
		case TYPE_REQUEST_PARSING : {log_fd = _request_parsing_fd; break; }
		case TYPE_REQUESTS : {log_fd = _requests_fd; break; }
		case TYPE_RESPONSE : {log_fd = _response_fd; break; }
		default : { break; }
	}
	return log_fd;
}

void Log::log(LogLevel level, const LogType type, const std::string msg) {

	int log_fd = setOutput(type);

	std::string log_line = timestamp() + " [" + levelToString(level) + "] "
                       + " [" + typeToString(type) + "] " + msg + "\n";

	write(log_fd, log_line.c_str(), log_line.size()); 	// write to file
	(*_out) << log_line; 								// write to stderr
}

void Log::log(LogLevel level, const LogType type, int fd, const std::string msg) {

	int log_fd = setOutput(type);
	
	std::string log_line = timestamp() + " [" + levelToString(level) + "] "
					   + "fd=" + itoa(fd) + " [" + typeToString(type) + "] "
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
		case TYPE_REQUEST_PARSING:  return "LOG_TYPE_REQUEST_PARSING";
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

std::string	itoa(int value)
{
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%d", value);  // need to replace
    return std::string(buf);
}