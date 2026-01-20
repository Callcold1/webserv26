#include <iostream>
#include <cassert>
#include <vector>
#include "Request.hpp"
#include "core_def.hpp"
#include "RequestParser.hpp"
#include "TestClasses/testUtils.hpp"
#include "Config.hpp"
#include "ConfigParser.hpp"
#include <cstring>

typedef std::string (StrReqF)(Request*);
typedef int (IntReqF)(Request*);

struct test_s{
	const char* _req_line;
	const char* _exp_val;
	int			_exp_code; 
	StrReqF* 	_f;
	IntReqF*	_g;
	
	test_s( const char* req_str, const char* exp_val, int exp_code, StrReqF f, IntReqF g ):
		_req_line(req_str), _exp_val(exp_val), _exp_code(exp_code), _f(f), _g(g){}
};

std::string getReqStr(Request* request){
	return request->_version;
}

int getReqInt(Request* request){
	return request->_errNum;
}

int main(int argc, char** argv) {

	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}
	ConfigParser cp(argv[1]);
	Config* config = cp.parseServerblocks();

	std::vector<test_s> tests;

	// FIRST ASSERT CHECKS IF HTTP VERSION IS PARSED AS EXPECTED
	// SECOND ASSERT IS CHECKS ERRNUM MATCHES THE EXPECTED RESPONSE CODE
	tests.push_back(test_s("GET https://user@example.com/path HTTP/1.2\r\n", "HTTP/1.2", 400, &getReqStr, &getReqInt));
	tests.push_back(test_s("GET https://user@example.com/path HTTP/1.1\r\n", "HTTP/1.1", 200, &getReqStr, &getReqInt));
	tests.push_back(test_s("GET https://user@example.com/path HTTP/1.0\r\n", "HTTP/1.0", 200, &getReqStr, &getReqInt));
	tests.push_back(test_s("GET https://user@example.com/path HTTP/2.0\r\n", "HTTP/2.0", 505, &getReqStr, &getReqInt));
	tests.push_back(test_s("GET https://user@example.com/path HTTP/3.0\r\n", "HTTP/3.0", 505, &getReqStr, &getReqInt));
	tests.push_back(test_s("GET https://user@example.com/path HTP/1.1\r\n", "HTP/1.1", 400, &getReqStr, &getReqInt));
	tests.push_back(test_s("GET https://user@example.com/path HTTP1.2\r\n", "HTTP1.2", 400, &getReqStr, &getReqInt));
	for (size_t i = 0; i < tests.size(); i++){
		
		Request* request = new Request(config);

		request->parseRequest(tests[i]._req_line, strlen(tests[i]._req_line));
		assert (tests[i]._f(request) == tests[i]._exp_val);
		assert (tests[i]._g(request) == tests[i]._exp_code);
		std::cout << "Test " << i << " : success" << std::endl;
	}
    return 0;
}

/*
VSCODE WATCH PARAMETERS
tests[i]._f(request)
tests[i]._exp_val
tests[i]._g(request)
(tests[i]._g(request) == tests[i]._exp_code)
*/