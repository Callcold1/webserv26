#include "RequestTypes.hpp"
#include "Request.hpp"
#include "RequestValidator.hpp"
#include "RequestParser.hpp"

#include <iostream>
#include <iomanip>

template <typename T>
std::string toString(const T val)
{
	std::stringstream stream;
	stream << val;
	return stream.str();
}

void	printHeaders(fieldMap_t headers){
	for (fieldMap_t::iterator it = headers.begin(); it != headers.end(); it++) {
		std::string headerName = it->first;
		std::vector<field_t*>& fieldList = it->second;

		for (size_t i = 0; i < fieldList.size(); i++) {
			field_t* field = fieldList[i];
			std::cout << std::left << std::setw(20) << "Header " << std::setw(3) << " : " << headerName << "\n";
			std::cout << std::left << std::setw(20) << "  Value " << std::setw(3) << " : " << field->val << "\n";

			for (size_t j = 0; j < field->params.size(); j++) {
				std::cout << std::left << std::setw(20) << "    Param " << std::setw(3) << " : " << field->params[j].first
						  << " = " << field->params[j].second << "\n";
			}
		}
		std::cout << std::endl;
	}
}

void	printRequest(Request* request){

	std::string status;
	switch(request->getParseStatus()){
		case 0	:{status = "P_PARSE"; break;}
		case 1	:{status = "P_AWAIT"; break;}
		case 2	:{status = "P UNPROC"; break;}
		case 3	:{status = "P DONE"; break;}
		case 4	:{status = "P ERROR"; break;}
		default	:{status = "UNKNOWN"; break;}
	}

	std::cout << std::left << std::setw(20) << "Status"  << std::setw(3) << " : " << status << std::endl;
	std::cout << std::left << std::setw(20) << "Method" << std::setw(3) << " : " << request->_method << std::endl;
	std::cout << std::left << std::setw(20) << "URI" << std::setw(3) << " : " << request->_uri_str << std::endl;
	std::cout << std::left << std::setw(20) << "Version" << std::setw(3) << " : " << request->_version <<  std::endl;
	std::cout << std::left << std::setw(20) << "Body" << std::setw(3) << " : " << request->_body << std::endl;
	std::cout << std::left << std::setw(20) << "ErrMess" << std::setw(3) << " : " << request->_errMess << std::endl;
	std::cout << std::left << std::setw(20) << "ErrNum" << std::setw(3) << " : " << request->_errNum << std::endl << std::endl;
	printHeaders(request->_headers);
	return;
}

size_t  len(const char* str){
	int i = 0;
	if (!str)
		return (i);
	while (*str++)
		i++;
	return i;
}

/*
enum serv_action{
	//	SERVER TASKS
	AWAIT_CHUNKS,		//	Transfer-encoding chunked and last chunk not received
	CONTENT_READ,		//	If Content-Length is present, read the exact amount of data
	
	// SERVER FLAGS TO UPDATE
	CLOSE_CONNECTION,	//	Header - Connection : close
	KEEP_ALIVE,			//	Header - Connection : keep-alive
	CLOSE_ERROR,		//	Closed due to error
	
	// HANDLER TASKS
	// FURTHER PROCESSING
	CHUNK_DECODE,		//	If the request body uses chunked encoding, decode it before processing.
	DECOMPRESS_CONTENT,	//	If a Content-Encoding (gzip, deflate, etc.) is present, decompress the body before processing.
	HANDLER_COOKIES,	//	Handle cookies if we're dealing with
	HANDLER_RANGE,		//	Handle range request
	SAVE_CONTENT,		//	If the request is a file upload (POST/PUT), store the file properly.
	
	HANDLER_CGI,		//	Request is for CGI Route
	// RESONSE SIDE PROCESSING
	COMPRESS_CONTENT,	//	Compress response content
	RESPOND_CHUNKED,	//	IF response size is unknown, send chunked response
	REDIRECT,			//	If a redirection status (301, 302, 307, 308) is needed, return a response with a Location header
	
	// STRAIGHT TO RESPONSE?
	RESPOND_ERROR,		//	Generate simple HTML response - error
	RESPOND_ERROR_AUTH,	//	If an Authorization header is required but missing or incorrect, return 401 Unauthorized
	RESPOND_ERROR_PERM,	//	If the request is forbidden due to permissions (403) or an unsupported method (405), respond accordingly.
	RESPOND_HTML,		//	Send HTML response
	RESPOND_OTHER,		//	Send other format of response JSON?
	RESPOND_STATIC,		//	Request is for a static file
};
*/

bool	server(const char *input){

	static Request*				request;
	static size_t				read_cycles;

	if (!input)
		return false;

	size_t	length = len(input);
	size_t	unprocessed = length;

	while (unprocessed > 0)
	{
		if (read_cycles == 0) {
			read_cycles++;
			request = new Request;
		}
		request->parseRequest(input + length - unprocessed, unprocessed);
		unprocessed = request->_parser->getUnparsedLength();
		switch(request->getParseStatus()){
			case P_PARSE			: std::cout << "P_PARSE" << std::endl; return true;
			case P_AWAIT			: std::cout << "P_AWAIT" << std::endl; return true;
			//case P_UNPROC			: std::cout << "P_UNPROC" << std::endl; return true;
			case P_DONE				: { printRequest(request); delete request; read_cycles = 0; break; }
			case P_ERROR			: { printRequest(request); delete request; read_cycles = 0; return false; }
			default					: break;
		}
		if (unprocessed == 0)
			return false;
	}
	return true;
}

extern bool server(const char* input);

void run_tests() {
	const char* tests[] = {
		// Test 1: Basic GET request
		"GET / HTTP/1.1\r\nHost: localhost\r\n\r\n",

		// Test 2: GET with query parameters
		"GET /index.html?x=1&y=2 HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n",

		// Test 3: POST with Content-Length
		"POST submit HTTP/1.1\r\nHost: localhost\r\nContent-Type: text/html\r\nContent-Length: 11\r\n\r\nHello World",

		// Test 4: POST with chunked transfer encoding
		"POST /data HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\nContent-Type: text/html\r\n\r\n1\r\na\r\n1\r\nb\r\n0\r\n\r\n",

		// Test 5: Invalid method
		"FOO /invalid HTTP/1.1\r\nHost: localhost\r\n\r\n",

		// Test 6: Multiple headers, keep-alive
		"GET /test HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\nUser-Agent: TestClient\r\nAccept: */*\r\n\r\n",

		// Test 7: Malformed chunk size
		"POST /data HTTP/1.1\r\nHost: localhost\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\n\r\ng\r\ninvalid\r\n0\r\n\r\n",

		// Test 8: Multiple requests in one input
		"GET / HTTP/1.1\r\nHost: localhost\r\n\r\nGET /about HTTP/1.1\r\nHost: localhost\r\n\r\n",

		// Test 9: Empty request
		"",

		// Test 10: GET with fragmented headers
		"GET /index.html HTTP/1.1\r\nHost:localhost\r\n\r\n",

		// Test 11: POST with Content-Length and Content-Type
		"POST /submit HTTP/1.1\r\nHost: localhost\r\nContent-Type: application/json\r\nContent-Length: 11\r\n\r\nHello World",

	};

	for (int t = 0; t < 11; t++) {
		const char* input = tests[t];
		int length = strlen(input);

		for (int i = 0; i <= length; i++) {
			bool connection = true;
			std::string temp(input, i);
			std::string temp2(input + i);
			std::cout << "Test " << t + 1 << ", Fragment " << i << "\n========" << std::endl;
			if (connection) 
				connection = server(temp.c_str());
			if (connection)
				connection = server(temp2.c_str());
		}
	}
}

int main() {
	run_tests();
}
