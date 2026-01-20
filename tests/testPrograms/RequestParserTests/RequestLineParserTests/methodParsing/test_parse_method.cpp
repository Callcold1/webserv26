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
	return request->_method;
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

	// FIRST ASSERT CHECKS IF METHOD IS PARSED AS EXPECTED
	// SECOND ASSERT IS CHECKS ERRNUM MATCHES THE EXPECTED RESPONSE CODE
	tests.push_back(test_s("GET /path HTTP/1.1\r\n", "GET", 200, &getReqStr, &getReqInt));
	tests.push_back(test_s("GEt /path HTTP/1.1\r\n", "GEt", 400, &getReqStr, &getReqInt));
	tests.push_back(test_s("GeT /path HTTP/1.1\r\n", "GeT", 400, &getReqStr, &getReqInt));
	tests.push_back(test_s("POST https://example.com/damianServer HTTP/1.1\r\n", "POST", 200, &getReqStr, &getReqInt));
	tests.push_back(test_s("Post https://example.com/path HTTP/1.1\r\n", "Post", 400, &getReqStr, &getReqInt));
	tests.push_back(test_s("DELETE https://example.com/path HTTP/1.1\r\n", "DELETE", 200, &getReqStr, &getReqInt));
	tests.push_back(test_s("Delete https://example.com/path HTTP/1.1\r\n", "Delete", 400, &getReqStr, &getReqInt));
	tests.push_back(test_s("MADEUP https://example.com/path HTTP/1.1\r\n", "MADEUP", 501, &getReqStr, &getReqInt));
	tests.push_back(test_s("PUT https://example.com/path HTTP/1.1\r\n", "PUT", 501, &getReqStr, &getReqInt));
	tests.push_back(test_s("GET https://example.com/noGET HTTP/1.1\r\n", "GET", 405, &getReqStr, &getReqInt)); // METHOD FORBIDDEN IN CONFIG ROUTE
	tests.push_back(test_s("GET https://example.com/noGET/info.html HTTP/1.1\r\n", "GET", 405, &getReqStr, &getReqInt)); // METHOD FORBIDDEN IN CONFIG ROUTE
	tests.push_back(test_s("GET https://example.com/noGET/ HTTP/1.1\r\n", "GET", 405, &getReqStr, &getReqInt)); // METHOD FORBIDDEN IN CONFIG ROUTE
	
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