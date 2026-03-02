/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilityFunctions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2026/03/02 14:45:27 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>

#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


// PATH STRUCT
struct fileInfo_s
{
	bool inaccessible;
	bool is_dir;
	bool is_exec;
	bool is_file;
	bool is_symlink;
	bool not_found;
	__mode_t permissions;
	std::string path;

	fileInfo_s() : inaccessible(false), is_dir(false), is_exec(false), is_file(false), is_symlink(false), not_found(false), permissions(0), path("") {}
};

template <typename T, T Min, T Max>
struct StaticRange
{
    static bool test(T value){
	    return value >= Min && value <= Max;
	}
};

/* STRING CASE CONVERSION */
std::string							to_lower(std::string str);
std::string							to_upper(std::string str);
void								to_lower_ref(std::string& str);
void								to_upper_ref(std::string& str);

/* STRING MODIFICATION */
void								trimLeadingChar( std::string& str, char target);
void								trimEndChar( std::string& str, char target);
void								trimWS( std::string& str);

/* STRING/CHAR TO NUMBER TYPE CONVERSION */
int									str_to_int( const std::string str);
int									substring_to_int(const std::string str, int start, int end);
long								hexToLong(const char *hex);
unsigned char						hexCharToUnsignedChar(char c);
bool                                substrToHex( uint64_t& dest, std::string str, size_t start, size_t end);

/* VALID CHARACTER CHECKS */
bool								isValidUTF8(const std::string& str);
bool								isTokenChar(unsigned char c);
bool								isVisibleChar(unsigned char c);

/* CHARACTER TYPE CONVERSION */
std::string							UTF8ToISO8859_1(const std::string& utf8_str);
std::string							ISO8859_1ToUTF8(const std::string& iso_str);

/*  FILE OPERATIONS  */
int									getFileType(const std::string& str);
bool								isAccessible(const std::string& path);
std::string&    					pathAppendToRoot(const std::string& path, const std::string& root);
void								getFilePermissions(fileInfo_s& target);
fileInfo_s*							getFileInfo(std::string& path, std::string& root_dir);
fileInfo_s*							getFileInfo(std::string& path);
void								getFileInfo(std::string& path, fileInfo_s& fileinfo);

/*	FILEPATH MANIPULATION  */
void								appendPathToRoot(fileInfo_s* target, std::string& path, std::string& root_dir);
std::pair<std::string, std::string>	pathSplit(std::string& path);
std::vector<std::string> 			split_path(std::istringstream &iss);
std::string 						rebuild_path(std::vector<std::string> segments);
void 								remove_dot_segments(std::string &str);

/*  DUPLICATE CHECK */
template<class T>
bool								hasDuplicates(const std::vector<T>& vec);

/*  TEMPLATED ITERATION FUNCTION - FUNCTION POINTER AS PARAM  */
template <typename Container, typename Func>
bool								iterate(const Container& container, Func f);

/*  BASIC CHARACTER CHECKS */
bool								isAlpha(unsigned char c);
bool								isDigit(unsigned char c);
bool								isAlphaOrDigit(unsigned char c);
bool								isHexDigit(unsigned char c);

/*  ABNF TYPE CHARACTER CHECKS  */
bool								isCtl(unsigned char c);
bool								isDelimiter(unsigned char c);
bool								isSubDelim(unsigned char c);
bool								isTextChar(unsigned char c);
bool								isTokenChar(unsigned char c);
bool								isUnreserved(unsigned char c);
bool								isVisibleChar(unsigned char c);
bool								isSeparator(unsigned char c); 

/*  URI SPECIFIC ABNF CHECKS */
bool								isSchemeChar(unsigned char c);
bool								isUserinfoChar(unsigned char c);
bool								isHostChar(unsigned char c);
bool								isPchar(unsigned char c);
bool    							isPathChar(unsigned char c);
bool								isQueryChar(unsigned char c);
bool								isFragmentChar(unsigned char c);

/*  FIELD SPECIFIC ABNF CHECKS */
bool								isFieldValueChar(unsigned char c);
bool								isQuotedParamKeyChar(unsigned char c);

/* PARSING OPERATIONS */
void								parseStreamToStrStrMap(std::istringstream& iss, std::map<std::string, std::string>& result, char pair_delim, char kv_delim);
void								parseStreamToStrVecStrMap(std::istringstream& iss, std::map<std::string, std::vector< std::string> >& result, char pair_delim, char kv_delim, char vec_delim);

/* PRINTING OPERATIONS */
void								printStrMap(const std::map<std::string, std::string>& data);
void                                printStrVecStrMap(const std::map<std::string, std::vector< std::string> >& data);

/*  SET CREATION */
const std::set<std::string>& 		getHeaderTransferEncodings();

/* 	VECTOR OPERATIONS */
bool		isOnlyArg(std::vector<std::string>& args);
std::string	createArgsString(std::vector<std::string> args);

template <typename T>
bool	convertToType(T& target, const std::string& str){
	std::istringstream stream(str);
	stream >> target;

    if (stream.fail())
        return false;
		
	 stream >> std::ws;
	
	return stream.eof();
}

/* IP ADDRESS */
void	printVisible(const std::string& s);
void	resolveAddress(const std::string& ip_str, const std::string& port_str, uint32_t& ip, uint16_t& port);
void	splitPortInfo(const std::string& port_info, uint32_t& ip, uint16_t& port);

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		CONVERSION TO STRING	<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

template <typename T>
std::string toString(const T val)
{
	std::stringstream stream;
	stream << val;
	return stream.str();
}

template <typename T>
std::string	printstream(T& src){

	std::ostringstream os;

	os << src << std::endl;
	std::string str = os.str();
	return str;
}