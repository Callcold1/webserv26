#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cctype>

enum	eParse 			{ P_PARSE, P_AWAIT, P_DONE, P_ERROR };
enum	eReqState		{ P_REQLINE, P_REQLINE_PARAMS, V_REQLINE, P_HEADERS, V_HEADERS, P_BODY, V_BODY, P_TRAILERS, V_TRAILERS, REQ_DONE };
enum	eURIstates		{ SCHEME, AUTH, USER, HOST, PORT, PATH, QUERY, FRAG, URI_ERROR, URI_END };
enum	uri_types		{ ABSOLUTE_FORM, ASTERISK, AUTH_FORM, ORIGIN_FORM };
enum	eParseContext	{ ABSOLUTE_URI, ORIGIN_URI, USER_NAME, USER_PASS, QUERY_KEY, QUERY_VAL, FIELD_NAME, FIELD_VAL, PARAM_KEY, PARAM_VAL, CHUNKSIZE};

std::string escape_char(char c) {
    switch (c) {
        case '\r': return "'\\r'";
        case '\n': return "'\\n'";
        case '\'': return "'\\''";
        case '\\': return "'\\\\'";
        case ' ': return "' '";
        default:
            if (std::isprint(static_cast<unsigned char>(c))) {
                return std::string("'") + c + "'";
            } else {
                std::ostringstream oss;
                oss << "'\\x"
                    << std::hex << std::setw(2) << std::setfill('0')
                    << (int)(unsigned char)c << "'";
                return oss.str();
            }
    }
}

std::string to_char_array(const std::string& s) {
    std::ostringstream oss;
    for (size_t i = 0; i < s.size(); ++i) {
        oss << escape_char(s[i]);
        if (i + 1 < s.size())
            oss << ",";
    }
    return oss.str();
}

struct TestSpec {
    // request
    std::string method;
    std::string uri;
    std::string version;
    std::vector<std::pair<std::string, std::string> > headers;
    std::string body;
    bool chunked;

    // expected
    int uri_type;
    int port_int;
    std::string scheme, auth, user, pass, host, port, path, query, frag;

    int errNum;
    std::string errMess;
    std::string p_state;
};

std::string build_raw_request(const TestSpec& t) {
    std::ostringstream req;

    req << t.method << " " << t.uri << " " << t.version << "\r\n";

    for (size_t i = 0; i < t.headers.size(); ++i) {
        req << t.headers[i].first << ": "
            << t.headers[i].second << "\r\n";
    }

    req << "\r\n";

    if (t.chunked) {
        if (!t.body.empty()) {
            std::ostringstream hex;
            hex << std::hex << t.body.size();
            req << hex.str() << "\r\n";
            req << t.body << "\r\n";
        }
        req << "0\r\n\r\n";
    } else {
        req << t.body;
    }

    return req.str();
}

void emit_raw_str(int index, const std::string& raw) {
    std::cout << "const char raw_str" << index << "[] = {"
              << to_char_array(raw) << "};\n\n";
}

void emit_test_profile(int i, const TestSpec& t) {
    std::cout << "testProfile* test" << i << " = new testProfile;\n";
    std::cout << "test" << i << "->request_str = \""
              << t.method << " " << t.uri << " " << t.version << "\";\n";
    std::cout << "test" << i << "->method = \"" << t.method << "\";\n";
    std::cout << "test" << i << "->version = \"" << t.version << "\";\n";
    std::cout << "test" << i << "->uri_str = \"" << t.uri << "\";\n";

    std::cout << "test" << i << "->uri.uri_type = " << t.uri_type << ";\n";
    std::cout << "test" << i << "->uri.port_int = " << t.port_int << ";\n";

    std::cout << "test" << i << "->uri.scheme = \"" << t.scheme << "\";\n";
    std::cout << "test" << i << "->uri.auth = \"" << t.auth << "\";\n";
    std::cout << "test" << i << "->uri.user = \"" << t.user << "\";\n";
    std::cout << "test" << i << "->uri.pass = \"" << t.pass << "\";\n";
    std::cout << "test" << i << "->uri.host = \"" << t.host << "\";\n";
    std::cout << "test" << i << "->uri.port = \"" << t.port << "\";\n";
    std::cout << "test" << i << "->uri.path = \"" << t.path << "\";\n";
    std::cout << "test" << i << "->uri.query = \"" << t.query << "\";\n";
    std::cout << "test" << i << "->uri.frag = \"" << t.frag << "\";\n";

    std::cout << "test" << i << "->errNum = " << t.errNum << ";\n";
    std::cout << "test" << i << "->errMess = \"" << t.errMess << "\";\n";
    std::cout << "test" << i << "->p_state = " << t.p_state << ";\n";

    std::cout << "test" << i << "->body = \"" << t.body << "\";\n";
    std::cout << "test" << i << "->hasBody = true;\n";
    std::cout << "test" << i << "->isChunked = "
              << (t.chunked ? "true" : "false") << ";\n\n";
}

int main() {
    std::vector<TestSpec> tests;

    TestSpec t3;
    t3.method = "POST";
    t3.uri = "/upload";
    t3.version = "HTTP/1.1";
    t3.headers.push_back(std::make_pair("Host", "example.com"));
    t3.headers.push_back(std::make_pair("Transfer-Encoding", "chunked"));
    t3.body = "Wikipedia";
    t3.chunked = true;

    t3.uri_type = ORIGIN_FORM;
    t3.port_int = 80;
    t3.path = "/upload";

    t3.errNum = 200;
    t3.errMess = "Parsing Complete";
    t3.p_state = "P_DONE";

    tests.push_back(t3);

    for (size_t i = 0; i < tests.size(); ++i) {
        int index = i + 3;
        std::string raw = build_raw_request(tests[i]);
        emit_raw_str(index, raw);
        emit_test_profile(index, tests[i]);
    }
}
