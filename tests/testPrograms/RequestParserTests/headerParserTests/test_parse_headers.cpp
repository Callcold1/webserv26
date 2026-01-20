#include <iostream>
#include <cassert>
#include "../../../Request/includes/Request.hpp"
#include "../../../Config/includes/Config.hpp"
#include "../../../Config/includes/ConfigParser.hpp"
#include <cstring>

bool	test_parse_headers(Config* config, const std::string& str, std::string& headerStr, int num){

	Request		req(str, config);
	bool		error = true;
	bool		errState = false;
	
	std::cout << "Test " << num <<  "\n=======\n" << std::endl;
	
	req.parseHeaders(headerStr); 
	req.test_validateHeaders();
	
	StrVecStrMap_t headers = req.getRequestHeaders();
	
	std::cout << "\nTEST PRINT HEADERS\n================\n" << std::endl;
	
	for (StrVecStrMap_t::iterator it = headers.begin(); it != headers.end(); it++){
		int i = 0;
		
		std::cout << "Key :" + it->first << std::endl;
		for (std::vector<std::string>::iterator itVec = it->second.begin(); itVec != it->second.end(); itVec++){
			std::cout << "Val " << i << " :" + *itVec << std::endl;
			i++;
		}
		if (it->first.empty()) {
			std::cout << "\nError : Key :" << "string is empty\n";
			error = errState;
		}
		if ((std::isspace(it->first[it->first.length() - 1]))) {
			std::cout << "\nError : Key :" << "does not end in space\n";
			error = errState;
		}
		if (it->second.empty()) {
			std::cout << "\nError : Val :" << "string is empty\n";
			error = errState;
		}																				
		std::cout << std::endl;
	}
	return error;
}

int	main(int argc, char* argv[]){
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}
	ConfigParser cp(argv[1]);
	Config* config = cp.parseServerblocks();

	std::string	request = "GET https://user@example.com/path HTTP/1.1\r\n";
	
	/*  VALID HEADER */
	std::string	headerStr0 = "Host: example.com\r\nUser-Agent: curl/7.68.0\r\nAccept:*\r\n\r\n";
	std::string	headerStr1 = "Connection: keep-alive\r\nCache-Control: no-cache\r\n\r\n";
	std::string	headerStr2 = "Content-Type: text/html; charset=UTF-8\r\nServer: CustomServer/1.0\r\n\r\n";
	std::string	headerStr3 = "ETag: \"abc123\"\r\nLast-Modified: Wed, 21 Oct 2023 07:28:00 GMT\r\n\r\n";
	std::string	headerStr4 = "Accept: text/html, application/json\r\nReferer: https://example.com\r\n\r\n";
	std::string	headerStr5 = "Accept-Language: en-US,en;q=0.9\r\nUpgrade-Insecure-Requests: 1\r\n\r\n";
	std::string	headerStr6 = "TestHeader: a,b,c,d\r\nUpgrade-Insecure-Requests: 1\r\n\r\n";

	/* INVALID HEADERS */
	std::string	headerStr7 = "Host example.com\r\nUser-Agent curl/7.68.0\r\n\r\n";												//Each header must have a colon (:) separating the key and value.
	std::string	headerStr8 = ": value\r\nHost: example.com\r\n\r\n"; 															// A header key cannot be empty.
	std::string	headerStr9 = "X-Header@: value\r\n\r\n"; 																		// Header names must only contain printable ASCII characters except spaces and special symbols like @.
	std::string	headerStr10 = "Header Name : value\r\n\r\n"; 																	// Spaces before the colon are not allowed.
	std::string	headerStr11 = "HeaderName:value\r\n\r\n"; 																		// There should be a space after the colon (:), e.g., "HeaderName: value".
	std::string	headerStr12 = "Content-Length:\r\n\r\n"; 																		// A header value cannot be empty unless it's explicitly allowed by HTTP specifications.
	std::string	headerStr13 = "X-Header: This is a long\r\nheader value\r\n\r\n"; 												// If a header value needs to span multiple lines, the subsequent lines must be indented with at least one space (X-Header: This is a long\r\n value\r\n)
	std::string	headerStr14 = "X-Header: value \x01\x02\x03\r\n\r\n"; 															// Control characters (ASCII < 32, except horizontal tab) are not allowed in header values.
	std::string	headerStr15 = "Content-Length: abc123\r\n\r\n"; 																// Content-Length must be a numeric value.
	std::string	headerStr16 = "Content-Length: -100\r\n\r\n"; 																	// Content-Length cannot be negative.
	std::string	headerStr17 = "Content-Length: 999999999999999999999999999\r\n\r\n";											// An excessively large Content-Length may cause integer overflow issues.
	std::string	headerStr18 = "Accept-Encoding: gzip, deflate,\r\n\r\n"; 														// A comma-separated list must not end with a trailing comma.
	std::string	headerStr19 = "X-Header: Value\r\n Another line\r\n\r\n"; 														// Header continuations require a leading space (X-Header: Value\r\n Another line is valid, but Another line without a space is not).
	std::string	headerStr20 = "Host: example.com\r\nUser-Agent: curl/7.68.0\r\n\r\n\r\n"; 										// Only one CRLF (\r\n\r\n) should separate headers from the body.
	std::string	headerStr21 = "Header:Name: value\r\n\r\n"; 																	// Header names cannot contain colons (:).
	std::string	headerStr22 = "Transfer-Encoding: CHUNKED\r\n\r\n"; 															// HTTP headers are case-insensitive, but values like chunked in Transfer-Encoding are typically lowercase per convention.
	std::string	headerStr23 = " Host: example.com\r\n\r\n"; 																	// Headers cannot start with whitespace.
	std::string headerStr24 = "X-Header: " + std::string(8192, 'A') + "\r\n\r\n";															// Some servers have limits (e.g., 8 KB for Apache) on header line length.
	std::string	headerStr25 = "Set-Cookie: sessionid=abc123\r\nSet-Cookie: userid=xyz456\r\n\r\n"; 								// Some headers (like Set-Cookie) allow duplicates, but others (like Content-Length) must be combined correctly.
	std::string	headerStr26 = "Connection: close\r\nHost: example.com\r\nContent-Length: 10\r\n\r\nGET / HTTP/1.1\r\n\r\n"; 	// Attempting to inject another HTTP request within headers is invalid and a security risk.
	
    assert(test_parse_headers(config, request, headerStr0, 0));
	assert(test_parse_headers(config, request, headerStr1, 1));
    assert(test_parse_headers(config, request, headerStr2, 2));
    assert(test_parse_headers(config, request, headerStr3, 3));
    assert(test_parse_headers(config, request, headerStr4, 4));
	assert(test_parse_headers(config, request, headerStr5, 5));
    assert(test_parse_headers(config, request, headerStr6, 6));
/*	assert(test_parse_headers(config, request, headerStr7, 7));
    assert(test_parse_headers(config, request, headerStr8, 8));
	assert(test_parse_headers(config, request, headerStr9, 9));
    assert(test_parse_headers(config, request, headerStr10, 10));
    assert(test_parse_headers(config, request, headerStr11, 11));
    assert(test_parse_headers(config, request, headerStr12, 12));
    assert(test_parse_headers(config, request, headerStr13, 13));
    assert(test_parse_headers(config, request, headerStr14, 14));
	assert(test_parse_headers(config, request, headerStr15, 15));
    assert(test_parse_headers(config, request, headerStr16, 16));
    assert(test_parse_headers(config, request, headerStr17, 17));
    assert(test_parse_headers(config, request, headerStr18, 18));
	assert(test_parse_headers(config, request, headerStr19, 19));
	assert(test_parse_headers(config, request, headerStr20, 20));
	assert(test_parse_headers(config, request, headerStr21, 21));
	assert(test_parse_headers(config, request, headerStr22, 22));
	assert(test_parse_headers(config, request, headerStr23, 23));
	assert(test_parse_headers(config, request, headerStr24, 24));
	assert(test_parse_headers(config, request, headerStr25, 25));
	assert(test_parse_headers(config, request, headerStr26, 26)); */
    return 0;
}

/*
VALID TESTS
✅ "Host: example.com\r\nUser-Agent: curl/7.68.0\r\nAccept:*\r\n\r\n"
✅ "Connection: keep-alive\r\nCache-Control: no-cache\r\n\r\n"
✅ "Content-Type: text/html; charset=UTF-8\r\nServer: CustomServer/1.0\r\n\r\n"
✅ "ETag: \"abc123\"\r\nLast-Modified: Wed, 21 Oct 2023 07:28:00 GMT\r\n\r\n"
✅ "Accept: text/html, application/json\r\nReferer: https://example.com\r\n\r\n"
✅ "Accept-Language: en-US,en;q=0.9\r\nUpgrade-Insecure-Requests: 1\r\n\r\n"
✅ "TestHeader: a, b, c, d\r\nUpgrade-Insecure-Requests: 1\r\n\r\n"

INVALID TESTS

1. Missing Colon (Invalid Header Format)
"Host example.com\r\nUser-Agent curl/7.68.0\r\n\r\n" //Each header must have a colon (:) separating the key and value.

2. Empty Header Name
": value\r\nHost: example.com\r\n\r\n" // A header key cannot be empty.

3. Invalid Characters in Header Name
"X-Header@: value\r\n\r\n" // Header names must only contain printable ASCII characters except spaces and special symbols like @.

4. Space Before Colon
"Header Name : value\r\n\r\n" // Spaces before the colon are not allowed.

5. Space After Colon Without Leading Space
"HeaderName:value\r\n\r\n" // There should be a space after the colon (:), e.g., "HeaderName: value".

6. Empty Header Value
"Content-Length:\r\n\r\n" // A header value cannot be empty unless it's explicitly allowed by HTTP specifications.

7. Header Value Spanning Multiple Lines (Without Proper Folding)
"X-Header: This is a long\r\nheader value\r\n\r\n" // If a header value needs to span multiple lines, the subsequent lines must be indented with at least one space (X-Header: This is a long\r\n value\r\n)

8. Forbidden Characters in Header Value
"X-Header: value \x01\x02\x03\r\n\r\n" // Control characters (ASCII < 32, except horizontal tab) are not allowed in header values.

9. Invalid Content-Length (Non-Numeric Value)
"Content-Length: abc123\r\n\r\n" // Content-Length must be a numeric value.

10. Negative Content-Length
"Content-Length: -100\r\n\r\n" // Content-Length cannot be negative.

11. Unreasonably Large Content-Length
"Content-Length: 999999999999999999999999999\r\n\r\n" // An excessively large Content-Length may cause integer overflow issues.

12. Trailing Comma in List-Type Header
"Accept-Encoding: gzip, deflate,\r\n\r\n" // A comma-separated list must not end with a trailing comma.

13. Unexpected Newline in Value (Improper Folding)
"X-Header: Value\r\n Another line\r\n\r\n" // Header continuations require a leading space (X-Header: Value\r\n Another line is valid, but Another line without a space is not).

14. Extra CRLF After Header Section
"Host: example.com\r\nUser-Agent: curl/7.68.0\r\n\r\n\r\n" // Only one CRLF (\r\n\r\n) should separate headers from the body.

15. Invalid Character in Header Name (Colon in Key)
"Header:Name: value\r\n\r\n" // Header names cannot contain colons (:).

16. Invalid Transfer-Encoding (Chunked Without Proper Case)
"Transfer-Encoding: CHUNKED\r\n\r\n" // HTTP headers are case-insensitive, but values like chunked in Transfer-Encoding are typically lowercase per convention.

17. Unexpected Whitespace at Beginning of Header Line
" Host: example.com\r\n\r\n" // Headers cannot start with whitespace.

18. Header Line Too Long
"X-Header: + "A" * 8192 + "\r\n\r\n" // Some servers have limits (e.g., 8 KB for Apache) on header line length.

19. Duplicate Headers Without Proper Combination
"Set-Cookie: sessionid=abc123\r\nSet-Cookie: userid=xyz456\r\n\r\n" // Some headers (like Set-Cookie) allow duplicates, but others (like Content-Length) must be combined correctly.

20. Forbidden Header Field Name (Injection Attempt)
"Connection: close\r\nHost: example.com\r\nContent-Length: 10\r\n\r\nGET / HTTP/1.1\r\n\r\n" // Attempting to inject another HTTP request within headers is invalid and a security risk.

*/