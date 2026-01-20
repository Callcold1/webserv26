#include "tests.hpp"

template <class T>
void	testOutput(std::ostream& os, T& parsed, T& expected){
	os << std::left << std::setw(15) << "Parsed"  << std::setw(3) << " : " << parsed << std::endl;
	os << std::left << std::setw(15) << "Expected"  << std::setw(3) << " : " << expected;
}

// REQUEST LINE
bool compare_request_str(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_req_line, test->request_str));
	os << std::endl;
	return request->_req_line.compare(test->request_str) == 0;
}
bool compare_request_method(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_method, test->method));
	os << std::endl;
	return request->_method.compare(test->method) == 0;
}
bool compare_request_version(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_version, test->version));
	os << std::endl;
	return request->_version.compare(test->version) == 0;
}

// URI
bool compare_uri_str(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri_str, test->uri_str));
	os << std::endl;
	return request->_uri_str.compare(test->uri_str) == 0;
}
bool compare_uri_type(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.uri_type, test->uri.uri_type));
	os << std::endl;
	return request->_uri.uri_type == test->uri.uri_type;
}
bool compare_uri_port_int(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.port_int, test->uri.port_int));
	os << std::endl;
	return request->_uri.port_int == test->uri.port_int;
}
bool compare_uri_auth(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.auth, test->uri.auth));
	os << std::endl;
	return request->_uri.auth.compare(test->uri.auth) == 0;
}
bool compare_uri_frag(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.frag, test->uri.frag));
	os << std::endl;
	return request->_uri.frag.compare(test->uri.frag) == 0;
}
bool compare_uri_host(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.host, test->uri.host));
	os << std::endl;
	return request->_uri.host.compare(test->uri.host) == 0;
}
bool compare_uri_pass(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.pass, test->uri.pass));
	os << std::endl;
	return request->_uri.pass.compare(test->uri.pass) == 0;
}
bool compare_uri_path(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.path, test->uri.path));
	os << std::endl;
	return request->_uri.path.compare(test->uri.path) == 0;
}
bool compare_uri_port(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.port, test->uri.port));
	os << std::endl;
	return request->_uri.port.compare(test->uri.port) == 0;
}
bool compare_uri_query(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.query, test->uri.query));
	os << std::endl;
	return request->_uri.query.compare(test->uri.query) == 0;
}
bool compare_uri_scheme(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.scheme, test->uri.scheme));
	os << std::endl;
	return request->_uri.scheme.compare(test->uri.scheme) == 0;
}
bool compare_uri_user(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_uri.user, test->uri.user));
	os << std::endl;
	return request->_uri.user.compare(test->uri.user) == 0;
}

// ERROR
bool compare_errNum(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_errNum, test->errNum));
	os << std::endl;
	return request->_errNum == test->errNum;
}
bool compare_errMess(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_errMess, test->errMess));
	os << std::endl;
	return request->_errMess.compare(test->errMess) == 0;
}
// STATE
bool compare_p_state(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_p_state, test->p_state));
	os << std::endl;
	return request->_p_state == test->p_state;
}

// BODY
bool compare_body(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_body, test->body));
	os << std::endl;
	return request->_body == test->body;
}

bool compare_hasBody(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_hasBody, test->hasBody));
	os << std::endl;
	return request->_hasBody == test->hasBody;
}

bool compare_isChunked(Request* request, testProfile* test, std::ostream& os) {
	DEBUG2(testOutput(os, request->_isChunked, test->isChunked));
	os << std::endl;
	return request->_isChunked == test->isChunked;
}