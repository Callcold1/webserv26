#pragma once

#include <iostream>
#include <cassert>
//#include "tests.hpp"
#include "RequestTypes.hpp"

# ifndef DEBUG1
# 	define debug1(x) ((x))
# else
#	define debug1(x)
# endif

# ifndef DEBUG2
# 	define debug2(y) ((y))
# else
#	define debug2(y)
# endif

typedef bool (*TestFunction)(Request* request, struct testProfile* test, std::ostream& os);

struct testProfile {
	std::vector<std::pair<TestFunction, const char*> > tests;
	std::vector<void *> results;

	std::string 		method, uri_str, version, request_str;
	uri_t				uri;
	fieldMap_t 			headers;
	std::string 		body;
	std::string 		boundary;
	std::string 		defaultMultiType;
	unsigned int		errNum;
	std::string			errMess;
	eParse				p_state;
	eReqState			r_state;
	uint64_t			content_length, unparsed_length;
	bool 				hasBody, isChunked, isMultiPart, bodyUTF8;

	testProfile():method(""), uri_str(""), version(""), request_str(""), headers(), body(""), boundary(""), defaultMultiType(""), errNum(200),
		errMess("OK"),p_state(), r_state(), content_length(0), unparsed_length(0), hasBody(false), isChunked(false), isMultiPart(false), bodyUTF8(false){}

	void add(TestFunction func, const char* message) {
		tests.push_back(std::make_pair(func, message));
	}

	void run(Request* request) {
		{
			for (size_t i = 0; i < tests.size(); i++) {
				std::ostringstream os;
				bool result = tests[i].first(request, this, os);

				if (result)
					std::cout << std::left << std::setw(16) << tests[i].second << GRE "[OK]\n" DEF << os.str() << std::endl;
				else 
					std::cout << std::left << std::setw(16) << tests[i].second << RED "[FAILED]\n" DEF << os.str() << std::endl;
			}
		}
	}
};

template <class T>
void	testOutput(std::ostream& os, T& parsed, T& expected);

// REQUEST LINE
bool	compare_request_str(Request* request, testProfile* test, std::ostream& os);
bool	compare_request_method(Request* request, testProfile* test, std::ostream& os);
bool 	compare_request_version(Request* request, testProfile* test, std::ostream& os);
// URI
bool	compare_uri_str(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_type(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_port_int(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_auth(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_frag(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_host(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_pass(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_path(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_port(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_query(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_scheme(Request* request, testProfile* test, std::ostream& os);
bool	compare_uri_user(Request* request, testProfile* test, std::ostream& os);
// ERROR COMPARISON
bool 	compare_errNum(Request* request, testProfile* test, std::ostream& os);
bool 	compare_errMess(Request* request, testProfile* test, std::ostream& os);
//PARSING STATE COMPARISON
bool	compare_p_state(Request* request, testProfile* test, std::ostream& os);
// BODY
bool	compare_body(Request* request, testProfile* test, std::ostream& os);
bool	compare_hasBody(Request* request, testProfile* test, std::ostream& os);
bool	compare_isChunked(Request* request, testProfile* test, std::ostream& os);