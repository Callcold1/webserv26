/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/06/17 23:35:03 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core_def.hpp"

#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>

#include <dirent.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

/*  SET CREATION */
const std::set<std::string>& 		getHeaderTransferEncodings();

/* CONVERT TO STRING */
template <typename T>
std::string toString(const T val);

/* PARSING OPERATIONS */
void								parseStreamToStrStrMap(std::istringstream& iss, std::map<std::string, std::string>& result, char pair_delim, char kv_delim);
void								parseStreamToStrVecStrMap(std::istringstream& iss, std::map<std::string, std::vector< std::string> >& result, char pair_delim, char kv_delim, char vec_delim);

/* STRING CASE CONVERSION */
std::string							to_lower(std::string str);
std::string							to_upper(std::string str);
void								to_lower_ref(str_t& str);
void								to_upper_ref(str_t& str);

/* STRING MODIFICATION */
void								trimLeadingChar( std::string& str, char target);
void								trimEndChar( std::string& str, char target);
void								trimWS( std::string& str);
void								remove_dot_segments(std::string&);

/* STRING/CHAR TYPE CONVERSION */
int									str_to_int( const std::string str);
int									substring_to_int(const std::string str, int start, int end);
long								hexToLong(const char *hex);
unsigned char						hexCharToUnsignedChar(char c);
bool                                substrToHex( uint64_t& dest, std::string str, size_t start, size_t end);

/* VALID CHARACTER CHECKS */
bool								isValidUTF8(const std::string& str);
bool								isTokenChar(unsigned char c);
bool								isVisibleChar(unsigned char c);

/* PRINTING OPERATIONS */
void								printStrMap(const std::map<std::string, std::string>& data);
void                                printStrVecStrMap(const std::map<std::string, std::vector< std::string> >& data);

/*  FILE OPERATIONS  */
int									getFileType(const std::string& str);
bool								isAccessible(const std::string& path);
std::string&    					pathAppendToRoot(const std::string& path, const std::string& root);
std::pair<std::string, std::string>	pathSplit(std::string& path);

/*  DUPLICATE CHECK */
template<class T>
bool								hasDuplicates(const std::vector<T>& vec);


/*  TEMPLATED ITERATION FUNCTION - FUNCTION POINTER AS PARAM  */
template <typename Container, typename Func>
bool	iterate(const Container& container, Func f);

/*  CHARACTER RANGE CHECKS */
bool	isAlpha(unsigned char c);
bool	isAlphaOrDigit(unsigned char c);
bool	isCtl(unsigned char c);
bool	isDigit(unsigned char c);
bool	isHexDigit(unsigned char c);
bool	isSeparator(unsigned char c);
bool	isSubDelim(unsigned char c);
bool	isTextChar(unsigned char c);
bool	isTokenChar(unsigned char c);
bool	isUnreserved(unsigned char c);
bool	isVisibleChar(unsigned char c);

/*  URI VALIDATION CHECKS */
bool	isSchemeChar(unsigned char c);
bool	isUserinfoChar(unsigned char c);
bool	isHostChar(unsigned char c);
bool	isPchar(unsigned char c);
bool    isPathChar(unsigned char c);
bool	isQueryChar(unsigned char c);
bool	isFragmentChar(unsigned char c);

/*  URI VALIDATION CHECKS */
bool	isFieldValueChar(unsigned char c);
bool	isQuotedParamKeyChar(unsigned char c);
