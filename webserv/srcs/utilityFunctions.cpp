/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilityFunctions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 15:33:48 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/19 14:09:31 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilityFunctions.hpp"

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

/*
>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		STRING CASE CONVERSION		<<<<<<
>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<
*/
std::string to_lower(std::string str)
{
	for (size_t i = 0; i < str.size(); ++i)
		str[i] = std::tolower(static_cast<unsigned char>(str[i]));
	return str;
}

std::string to_upper(std::string str)
{
	for (size_t i = 0; i < str.size(); ++i)
		str[i] = std::toupper(static_cast<unsigned char>(str[i]));
	return str;
}

void to_lower_ref(std::string &str)
{
	for (size_t i = 0; i < str.size(); ++i)
		str[i] = std::tolower(static_cast<unsigned char>(str[i]));
}

void to_upper_ref(std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = std::toupper(static_cast<unsigned char>(str[i]));
}
/*
>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<
>>>>>>		STRING MODIFICATION		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/
void trimLeadingChar(std::string &str, char target)
{
	while (str[0] == target)
		str.erase(0, 1);
}

void trimEndChar(std::string &str, char target)
{
	while (str[str.size() - 1] == target)
		str.erase(str.size() - 1);
}

/* TRIM WHITESPACE FROM BOTH ENDS OF A STRING */
void trimWS(std::string &str)
{
	size_t end = str.size();

	while (end > 0 && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
		--end;
	}
	str.erase(end);

	size_t start = 0;

	while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) {
		start++;
	}
	str.erase(0, start);
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		STRING/CHAR TO NUMBER TYPE CONVERSION		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

// STRING TO INTEGER CONVERSION
int str_to_int(const std::string str)
{
	int value;

	std::istringstream iss(str);
	iss >> value;
	return value;
}

// SUBSTRING TO INTEGER CONVERSION
int substring_to_int(const std::string str, int start, int end)
{
	std::string substring = str.substr(start, end);
	return (str_to_int(substring));
}

long hexToLong(const char *str)
{
	long result = 0;
	int value;
	char c;

	if (!str)
		return 0;

	while (*str)
	{
		c = *str;
		value = 0;

		if (std::isdigit(c))
			value = c - '0';
		else if (c >= 'A' && c <= 'F')
			value = c - 'A' + 10;
		else if (c >= 'a' && c <= 'f')
			value = c - 'a' + 10;
		else
			break;
		result = (result << 4) | value;
		str++;
	}

	return result;
}

// HEX TO UNSIGNED CHAR CONVERSION
unsigned char hexCharToUnsignedChar(char c)
{
	unsigned char x;

	if (c >= '0' && c <= '9')
	x = c - '0';
	else if (c >= 'A' && c <= 'F')
	x = c - 'A' + 10;
	else if (c >= 'a' && c <= 'f')
	x = c - 'a' + 10;
	else
	return -1;
	return x;
}

// SUBSTRING TO HEX CONVERSION
bool	substrToHex( uint64_t& dest, std::string str, size_t start, size_t end){

	std::string		sub;
	unsigned char	converted = 0;
	size_t			len = end - start;

	if (str.size() < (end - start) || str.size() == 0)
		return false;
	sub = str.substr(start, end);

	for (size_t i = 0; i < len; i++){
		if (!isxdigit(sub[i]))
			return false;
		dest = dest << 4;
		converted = hexCharToUnsignedChar(sub[i]);
		dest += converted;
	}
	return true;
}

bool convertToBool(const std::string& str)
{
	if (str == "true")
		return true;
	if (str == "false")
		return false;

	throw std::runtime_error("Invalid boolean value: " + str);
}


/*
>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		VALID CHARACTER CHECKS		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

// CHECK IF STRING IS A VALID UTF-8 STRING
bool isValidUTF8(const std::string &str)
{
	if (str.empty())
		return true;

	unsigned char c;
	int expectedContinuationBytes = 0;
	size_t i = 0;

	while (str[i])
	{
		c = static_cast<unsigned char>(str[i]);

		if (expectedContinuationBytes == 0)
		{
			if ((c & 0x80) == 0x00)
				expectedContinuationBytes = 0;
			else if ((c & 0xE0) == 0xC0)
				expectedContinuationBytes = 1;
			else if ((c & 0xF0) == 0xE0)
				expectedContinuationBytes = 2;
			else if ((c & 0xF8) == 0xF0)
				expectedContinuationBytes = 3;
			else
				return false;
		}
		else
		{
			if ((c & 0xC0) != 0x80)
				return false;
			expectedContinuationBytes--;
		}
		i++;
	}
	return expectedContinuationBytes == 0;
}

/*
0000	0
0001	1Global
======
Max Body size : 500000
Max Connections : 1024
Max Header size : 8000
Timeout (keep-alive) : 65 seconds
Timeout (read-write) : 200 seconds
0010	2
0011	3
0100	4
0101	5
0110	6
0111	7
1000	8
1001	9
1010	10	A
1011	11	B
1100	12	C
1101	13	D
1110	14	E
1111	15	F

(c & 0x80) == 0x00			 c 		 0xxx xxxx	c must start 0
0xxx 0000
+	 1000 0000

(c & 0xE0) == 0xC0			 c		 110x xxxx	c must start 110
+	 1110 0000
1100 0000

(c & 0xF0) == 0xE0			c		 1110 xxxx	c must start 1110
+	 1111 0000
1110 0000

(c & 0xF8) == 0xF0			c		 1111 0xxx	c must start 1111 0
+	 1111 1000
1111 0000

(c & 0xC0) != 0x80			c		 10xx xxxx	if the condition is true, the byte doesn't begin 11, and isn't a valid continuation byte
+	 1100 0000
!=		 1000 0000
*/

/*
>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		CHARACTER TYPE CONVERSION		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
*/


// CONVERT UTF-8 TO ISO8859_1 FORMAT
std::string	UTF8ToISO8859_1(const std::string& utf8_str) { // PROBABLY WON'T BE REQUIRED BUT THIS TRANSLATES FROM UTF8 TO ISO8859.1

	std::string	iso_str;

	for (size_t i = 0; i < utf8_str.size(); i++) {

		unsigned char c = utf8_str[i];

		if (c <= 127)
			iso_str += c;

		else if ((c & 224) == 192 && i + 1 < utf8_str.size()) {

			unsigned char c1 = utf8_str[i + 1];

			if ((c1 & 192) == 128) {

				unsigned char iso_char = ((c & 31) << 6) | (c1 & 63);

				if (iso_char >= 128) {
					iso_str += iso_char;
					i++;
					continue;
				}
			}
		}
		iso_str += '?';
	}
	return iso_str;
}

// CONVERT ISO8859_1 TO UTF-8 FORMAT
std::string	ISO8859_1ToUTF8(const std::string& iso_str) { // PROBABLY WON'T BE REQUIRED BUT THIS TRANSLATES FROM ISO8859.1 to UTF8

	std::string utf8_str;
	unsigned char c = 0;

	size_t len = iso_str.length();
	for (size_t i = 0; i < len; i++) {
		c = iso_str[i];
		if (c <= 127)
			utf8_str += c;
		else {
			utf8_str += 192 | (c >> 6);
			utf8_str += 128 | (c & 63);
		}
	}
	return utf8_str;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<
>>>>>>		FILE OPERATIONS		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<
*/

int getFileType(const std::string &path)
{
	struct stat fileStat;

	if (stat(path.c_str(), &fileStat) == 0)
	{
		if (S_ISDIR(fileStat.st_mode))
			return 1;
		if (S_ISREG(fileStat.st_mode))
			return 2;
	}
	return 0;
}

bool isAccessible(const std::string &path)
{
	return access(path.c_str(), R_OK) == 0;
}

bool isExecutable(const std::string &path)
{
	return access(path.c_str(), R_OK) == 0;
}

/*
	struct fileInfo_s
	{
		bool inaccessible;		// set to true if path can"t be accessed
		bool is_dir;			// set to true if path is a dir
		bool is_file;			// set to true if path is a file
		bool not_found;			// set to true if file not found
		__mode_t permissions;	// the file permissions
		std::string path;		// the path (root + '/' endpoint )

		fileInfo_s() : inaccessible(false), is_dir(false), is_file(false), not_found(false), permissions(0), path("") {}
	};
*/

void	getFilePermissions(fileInfo_s& fileinfo){
	struct stat fileStat;

	if (stat(fileinfo.path.c_str(), &fileStat) == 0)
	{
		fileinfo.is_dir = S_ISDIR(fileStat.st_mode);
		fileinfo.is_file = S_ISREG(fileStat.st_mode);
		fileinfo.is_symlink = S_ISLNK(fileStat.st_mode);
		fileinfo.is_exec = fileStat.st_mode & S_IXUSR;
		fileinfo.permissions = fileStat.st_mode;
	}
	else if(errno == ENOENT)
		fileinfo.not_found = true;
	else if(errno == EACCES)
		fileinfo.inaccessible = true;

}

void	getFilePermissions(fileInfo_s* fileinfo){
	struct stat fileStat;

	if (stat(fileinfo->path.c_str(), &fileStat) == 0)
	{
		fileinfo->is_dir = S_ISDIR(fileStat.st_mode);
		fileinfo->is_file = S_ISREG(fileStat.st_mode);
		fileinfo->is_symlink = S_ISLNK(fileStat.st_mode);
		fileinfo->is_exec = fileStat.st_mode & S_IXUSR;
		fileinfo->permissions = fileStat.st_mode;
	}
	else if(errno == ENOENT)
		fileinfo->not_found = true;
	else if(errno == EACCES)
		fileinfo->inaccessible = true;

}

void getFileInfo(std::string& path, fileInfo_s& fileinfo){

	fileinfo.path = path;
	getFilePermissions(fileinfo);
}

fileInfo_s* getFileInfo(std::string& path){

	fileInfo_s* fileinfo = new fileInfo_s;
	fileinfo->path = path;
	getFilePermissions(fileinfo);

	return fileinfo;
}

fileInfo_s* getFileInfo(std::string& path, std::string& root_dir){

	fileInfo_s* fileinfo = new fileInfo_s;

	appendPathToRoot(fileinfo, path, root_dir);
	getFilePermissions(fileinfo);

	return fileinfo;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<
>>>>>>	  FILEPATH MANIPULATION		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<
*/

void	appendPathToRoot(fileInfo_s* fileinfo, std::string& path, std::string& root_dir){

	if (root_dir[root_dir.size() - 1] == '/')
		trimEndChar(root_dir, '/');
	if (path[0] == '/')
		trimLeadingChar(path, '/');
	fileinfo->path = root_dir + '/' + path;
}

std::vector<std::string> split_path(std::istringstream &iss)
{
	std::string str;
	std::vector<std::string> segments;

	if (str.substr(0, 1) == "./")
		str = str.substr(2);
	else if (str[0] == '/')
		segments.push_back("/");
	while (std::getline(iss, str, '/'))
	{
		if (str.empty() || str.compare(".") == 0)
			continue;
		else if (str.compare("..") == 0 && segments.size() > 1)
			segments.pop_back();
		else
			segments.push_back(str);
	}
	return segments;
}

std::string rebuild_path(std::vector<std::string> segments)
{
	std::string path;

	if (segments[0] == "/")
		for (size_t i = 1; i < segments.size(); i++)
		{
			path + segments[i];
		}
	return path;
}

void remove_dot_segments(std::string &str)
{
	std::istringstream iss(str);
	std::vector<std::string> segments;

	segments = split_path(iss);
	str = rebuild_path(segments);
	return;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<
>>>>>>	    DUPLICATE CHECKS		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<
*/

// CHECK VECTOR FOR DUPLICATE ENTRIES
template <class T>
bool	hasDuplicates(const std::vector<T>& vec) {

	std::set<std::string> seen;

	for (size_t i = 0; i < vec.size(); i++){

		if (seen.find(vec[i]) != seen.end()) {
			return true;
		}
		seen.insert(vec[i]);
	}
	return false;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<
>>>>>>	 BASIC CHARACTER CHECKS		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<
*/
bool	isAlpha(unsigned char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool	isDigit(unsigned char c) {
	return c >= '0' && c <= '9';
}

bool	isAlphaOrDigit(unsigned char c) {
	return isAlpha(c) || isDigit(c);
}

bool	isHexDigit(unsigned char c) {
	return isDigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<
>>>>>>	  ABNF TYPE CHARACTER CHECKS	<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<
*/

bool	isCtl(unsigned char c) {
	return (c <= 31) || (c == 127);
}

bool	isDelimiter(unsigned char c) {
	//DQUOTE and "(),/:;<=>?@[\]{}"
	switch (c) {
		case '(': case ')': case '<': case '>': case '@':
		case ',': case ';': case ':': case '\\': case '"':
		case '/': case '[': case ']': case '?': case '=':
		case '{': case '}': case ' ': case '\t':
			return true;
		default:
			return false;
	}
}

bool	isSubDelim(unsigned char c) {
	switch (c) {
		case '!': case '$': case '&': case '\'': case '(':
		case ')': case '*': case '+': case ',': case ';':
		case '=':
			return true;
		default:
			return false;
	}
}

bool	isTextChar(unsigned char c) {
	return (c == '\t') || (c >= ' ' && c <= '~') || (c >= 128);
}

bool	isTokenChar(unsigned char c) {
	return (c >= 33 && c <= 126 && !isDelimiter(c));
}

bool	isUnreserved(unsigned char c) {
	return isAlpha(c) || isDigit(c) || c == '-' || c == '.' || c == '_' || c == '~';
}

bool	isVisibleChar(unsigned char c) {
		return c >= '!' && c <= '~';
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<
>>>>>>		URI SPECIFIC ABNF CHECKS		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/

bool isSchemeChar(unsigned char c) {
	return isAlpha(c) || isDigit(c) || c == '+' || c == '-' || c == '.';
}

bool isUserinfoChar(unsigned char c) {
	return isUnreserved(c) || isSubDelim(c) || c == ':';
}

bool isHostChar(unsigned char c) {
	return isUnreserved(c) || isSubDelim(c);
}

bool isPchar(unsigned char c) {
	return isUnreserved(c) || isSubDelim(c) || c == ':' || c == '@' || c == '%';
}

bool isPathChar(unsigned char c){
	return isPchar(c) || c == '/';
}

bool isQueryChar(unsigned char c) {
	return isPchar(c) || c == '/' || c == '?';
}

bool isFragmentChar(unsigned char c) {
	return isQueryChar(c);
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<
>>>>>>		FIELD SPECIFIC ABNF CHECKS		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/

bool isFieldValueChar(unsigned char c) {
	return isVisibleChar(c) || (c >= 128);
}

bool isQuotedParamKeyChar(unsigned char c) {
	return isVisibleChar(c) && c != '\"'  ;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		ITERATION OVER CONTAINER FUNCTION			<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<
*/
template <typename Container, typename Func>
bool	iterate(const Container& container, Func f) {
	typename Container::const_iterator it;
	for (it = container.begin(); it != container.end(); ++it) {
		if (!f(*it))
			return false;
	}
	return true;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		PRINTING OPERATIONS			<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
*/

// Print fileInfo_s
std::ostream&	printFileInfo(std::ostream &os, fileInfo_s* fileinfo)
{
	os << "File Inaccessible: " << fileinfo->inaccessible << std::endl;
	os << "File Is Directory: " << fileinfo->is_dir << std::endl;
	os << "File Is File: " << fileinfo->is_file << std::endl;
	os << "File Not Found: " << fileinfo->not_found << std::endl;
	os << "File Appended Path: " << fileinfo->path << std::endl;
	os << "File Permissions: "
	<< std::oct << std::setfill('0') << std::setw(4) << (fileinfo->permissions & 0777)
	<< std::dec << std::setfill(' ') << std::setw(0) << std::endl;
	return os;
}

void printStrMap(const std::map<std::string, std::string> &data)
{

	std::cout << "PARSED HEADER MAP \n===================" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = data.begin(); it != data.end(); it++)
	{
		int i = 0;
		std::cout << "Key : " + it->first << std::endl;
		if (it->second == "\r")
		{
			std::cout << "[CR]" << std::endl
					  << std::endl;
		}
		else
		{
			std::cout << "Val " << i << " : " + it->second << std::endl;
			i++;
		}
		std::cout << std::endl;
	}
	std::cout << "END OF HEADER MAP \n===================" << std::endl
			  << std::endl;
}

void printStrVecStrMap(const std::map<std::string, std::vector<std::string> > &data) // NEED TO CONSIDER HOW VECTOR FIELDS ARE POPULATED
{
	std::cout << "PARSED HEADER MAP VECTOR \n==========================" << std::endl;
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = data.begin(); it != data.end(); it++)
	{
		int i = 0;
		std::cout << "Key : " + it->first << std::endl;
		for (std::vector<std::string>::const_iterator itVec = it->second.begin(); itVec != it->second.end(); itVec++){
			if (*itVec == "\r")
			{
				std::cout << "[CR]" << std::endl
						  << std::endl;
			}
			else
			{
				std::cout << "Val " << i << " : " + *itVec << std::endl;
			}
			i++;
		}
		std::cout << std::endl;
	}
}

/*
>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		PARSING OPERATIONS			<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
*/

void parseStreamToStrVecStrMap(std::istringstream &iss, std::map<std::string, std::vector<std::string> > &result, char pair_delim, char kv_delim, char vec_delim)
{
	std::string line;
	std::string key;
	std::string value;

	while (std::getline(iss, line, pair_delim))
	{
		std::size_t pos = line.find(kv_delim);

		if (pos == std::string::npos)
			break;
		key = to_lower(line.substr(0, pos));
		value = line.substr(pos + 1);

		std::istringstream vec_stream(value);

		while (std::getline(vec_stream, line, vec_delim)){
			result[key].push_back(line);
		}
	}
	result["mapLastLine"].push_back(line);
}

void parseStreamToStrStrMap(std::istringstream &iss, std::map<std::string, std::string> &result, char pair_delim, char kv_delim)
{
	std::string line;
	std::string key;
	std::string value;

	while (std::getline(iss, line, pair_delim))
	{
		std::size_t pos = line.find(kv_delim);
		if (pos == std::string::npos)
			break;
		key = line.substr(0, pos);
		value = line.substr(pos + 1);
		result[key] = value;
	}
	result["mapLastLine"] = line;
	printStrMap(result);
}

const std::set<std::string> &getHeaderTransferEncodings()
{

	static std::set<std::string> supportedEndodings;
	static int i;

	if (i == 0)
	{
		supportedEndodings.insert(std::string("chunked"));
		// supportedEndodings.insert(std::string("compress"));
		// supportedEndodings.insert(std::string("deflate"));
		// supportedEndodings.insert(std::string("gzip"));
		// supportedEndodings.insert(std::string("identity"));
		// supportedEndodings.insert(std::string("trailers"));
		// supportedEndodings.insert(std::string("x-compress"));
		// supportedEndodings.insert(std::string("x-gzip"));
		i++;
	}
	return supportedEndodings;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		VECTOR OPERATIONS			<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
*/

bool	isOnlyArg(std::vector<std::string>& args){
	return args.size() == 1;
}

std::string	createArgsString(std::vector<std::string> args){
	size_t size = args.size();

	std::string str;

	for (size_t i = 0; i < size; i++){
		str.append(args[i]);
		if (i < size - 1)
			str.append(" ");
	}
	return str;
}

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
>>>>>>		IP ADDRESS			<<<<<<
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
*/

void printVisible(const std::string& s)
{
    for (size_t i = 0; i < s.size(); ++i)
    {
        unsigned char c = s[i];

        if (c == ' ')
            std::cout << "[SPACE]";
        else if (c == '\n')
            std::cout << "[\\n]";
        else if (c == '\r')
            std::cout << "[\\r]";
        else if (c == '\t')
            std::cout << "[\\t]";
        else if (std::isprint(c))
            std::cout << c;
        else
            std::cout << "[0x" << std::hex << (int)c << std::dec << "]";
    }
    std::cout << std::endl;
}

void	resolveAddress(const std::string& ip_str, const std::string& port_str, uint32_t& ip, uint16_t& port) {
	struct addrinfo hints;
	struct addrinfo* res = NULL;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	if (ip_str.empty())
		hints.ai_flags = AI_PASSIVE;

	const char* node = ip_str.empty() ? NULL : ip_str.c_str();

	int status = getaddrinfo(node, port_str.c_str(), &hints, &res);
	if (status != 0)
		throw std::runtime_error(gai_strerror(status));

	if (!res)
		throw std::runtime_error("getaddrinfo returned no result");

	struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(res->ai_addr);

	ip = addr->sin_addr.s_addr;
	port = ntohs(addr->sin_port);

	freeaddrinfo(res);
}

void splitPortInfo(const std::string& port_info, uint32_t& ip, uint16_t& port)
{
	size_t pos = port_info.find(':');
	std::string ip_str;
	std::string port_str;

	if (pos != std::string::npos){
		ip_str = port_info.substr(0, pos);
		port_str = port_info.substr(pos + 1);
	} 
	else {
		ip_str = "";
		port_str = port_info;
	}
	resolveAddress(ip_str, port_str, ip, port);
}