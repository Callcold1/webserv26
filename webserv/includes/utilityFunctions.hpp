/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilityFunctions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junruh <junruh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2026/04/14 13:52:26 by junruh           ###   ########.fr       */
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
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


// PATH STRUCT
struct path_t {
	bool absolute;
	bool trailing_slash;
	std::vector<std::string> segments;
};

// FILE INFO STRUCT
struct fileInfo_s
{
	bool 		inaccessible;
	bool 		is_dir;
	bool 		is_exec;
	bool 		is_file;
	bool 		is_symlink;
	bool 		not_found;
	
	time_t		last_modified;
	std::string	mime_type;
	std::string	path;
	__mode_t	permissions;
	off_t		size;

	fileInfo_s() : inaccessible(false), is_dir(false), is_exec(false), is_file(false), is_symlink(false), not_found(false),
					last_modified(0), mime_type(""), path(""), permissions(0), size(0){}
	//fileInfo_s() : inaccessible(false), is_dir(false), is_exec(false), is_file(false), is_symlink(false), not_found(false),
	//				last_modified(0), mime_type(""), path(""), size(0){}

	void reset(void){
		inaccessible = 0;
		is_dir = 0;
		is_exec = 0;
		is_file = 0;
		is_symlink = 0;
		not_found = 0;
		last_modified = 0;
		mime_type.clear();
		path.clear() ;
		permissions = 0;
		size = 0;
	}
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
void								trimLWS(std::string &str);
void								trimTWS(std::string &str);
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
bool 								file_exists(const char* filename);
bool								assignTempfile(std::string& target_path, int& target_fd, const std::string& prefix);

/*	FILEPATH MANIPULATION  */
std::string							appendPathToRoot(std::string path, std::string root_dir);
void								appendPathToRoot(fileInfo_s* target, std::string path, std::string root_dir);
std::string							normalisePath(const std::string& path);
std::string							get_absolute_path(const std::string server_path);

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
bool								isOnlyArg(std::vector<std::string>& args);
std::string							createArgsString(std::vector<std::string> args);
std::string							createArgsString(std::vector<std::string> args, std::string delimiter, bool last);

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
>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		GENERATE UNIQUE NAME STRING		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
*/

std::string createTimestamp(void);
std::string	generate_batch_id(int counter);


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

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		MIME TYPE/MAGIC NUMBER	<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

const std::map<std::string, std::string>&	mimeToMagicMap(void);
std::string									detectMimeFromFilename(const std::string& filename);