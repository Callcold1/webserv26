#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

std::string escape_char(char c) {
    switch (c) {
        case '\r': return "'\\r'";
        case '\n': return "'\\n'";
        case '\'': return "'\\''";
        case '\\': return "'\\\'";
        case ' ': return "' '";
        default:
            if (std::isprint(static_cast<unsigned char>(c))) {
                return std::string("'") + c + "'";
            } else {
                std::ostringstream oss;
                oss << "'\\x" << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(static_cast<unsigned char>(c))) << "'";
                return oss.str();
            }
    }
}

std::string convert_to_char_array(const std::string& input) {
    std::ostringstream oss;
    for (size_t i = 0; i < input.length(); ++i) {
        oss << escape_char(input[i]);
        if (i < input.length() - 1) oss << ",";
    }
    return oss.str();
}

int main() {
    std::vector<std::string> requests = {
		"GET /hello%20world HTTP/1.1\r\nHost: example.com\r\n\r\n",
		"GET /folder%2Ffile HTTP/1.1\r\nHost: test.com\r\n\r\n",
		"GET /search?q=open%20ai HTTP/1.1\r\nHost: engine.com\r\n\r\n",
		"GET /percent?value=100%25 HTTP/1.1\r\nHost: percent.com\r\n\r\n",
		"GET /%E2%9C%93 HTTP/1.1\r\nHost: unicode.com\r\n\r\n",
		"GET /foo%3Abar HTTP/1.1\r\nHost: test.com\r\n\r\n",
		"GET /file%3Bversion=1 HTTP/1.1\r\nHost: test.com\r\n\r\n",
		"GET /config?set%3Dvalue HTTP/1.1\r\nHost: test.com\r\n\r\n",
		"GET /home HTTP/1.1\r\nHost: test.com\r\nUser-Agent: MyAgent%2F1.0\r\n\r\n",
		"GET /api?key=123%26mode=edit HTTP/1.1\r\nHost: api.com\r\n\r\n",
		"GET /abc%2fDef HTTP/1.1\r\nHost: site.com\r\n\r\n",
		"GET /user%40mail.com HTTP/1.1\r\nHost: mail.com\r\n\r\n",
		"GET /api%3Ffake=1 HTTP/1.1\r\nHost: api.com\r\n\r\n",
		"GET /%F0%9F%98%81 HTTP/1.1\r\nHost: emoji.com\r\n\r\n",
		"GET / HTTP/1.1\r\nHost: site.com\r\nAccept-Language: en%2Cus\r\n\r\n",
		"GET /invalid%G1 HTTP/1.1\r\nHost: test.com\r\n\r\n",
		"GET /test% HTTP/1.1\r\nHost: test.com\r\n\r\n",
		"GET /path%2 HTTP/1.1\r\nHost: test.com\r\n\r\n",
		"GET / HTTP/1.1\r\nHo%73t: bad.com\r\n\r\n",
		"GET /%E2%28%A1 HTTP/1.1\r\nHost: broken.com\r\n\r\n",
		"GET /mix%20ed%2Zdata HTTP/1.1\r\nHost: mixed.com\r\n\r\n",
		"GET / HTTP/1.1\r\nUser-Agent: Test%zz\r\nHost: test.com\r\n\r\n",
		"GET /bad%0\r\n%0A HTTP/1.1\r\nHost: split.com\r\n\r\n",
		"GET /weird%%20path HTTP/1.1\r\nHost: weird.com\r\n\r\n",
		"GET / HTTP/1.1\r\nX-Header: test%0D%0AMalicious: injected\r\nHost: evil.com\r\n\r\n",
		"GET /some%20thing% HTTP/1.1\r\nHost: error.com\r\n\r\n",
		"GET /api?%bad=value HTTP/1.1\r\nHost: api.com\r\n\r\n",
		"GET /%F8%88%80%80 HTTP/1.1\r\nHost: utf8fail.com\r\n\r\n",
		"GET /file%00name HTTP/1.1\r\nHost: null.com\r\n\r\n",
		"GET /path?query=%3G HTTP/1.1\r\nHost: example.com\r\n\r\n"
    };

    for (size_t i = 0; i < requests.size(); ++i) {
        std::cout << "const char raw_str" << (i + 3) << "[ ] = {" << convert_to_char_array(requests[i]) << "};\n\n";
    }

    return 0;
}
