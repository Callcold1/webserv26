#include "tests.hpp"

void	populateTestStringsVector(std::vector<std::string>& strings){

	const char raw_str3[] = {
	    'P','O','S','T',' ','/','j','u','n','k',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','h','o','s','t','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '1','\r','\n','x','\r','\n',
	    '0','\r','\n',
	    '\r','\n',
	    'G','A','R','B','A','G','E'
	};

	const std::string str3(raw_str3, sizeof(raw_str3));
	strings.push_back(str3);

	for (size_t i = 3; i < strings.size() + 3; i++)
	{	
		std::string filename = "./testRequestFiles/str" + iToStr(i);
		writeStringToFile(strings[i - 3], filename);
	}
}

void	printErrorString(int i){

	const char *error[] = {
		"Valid Message ",
	};
	std::cout << "\nExpected Result : " << error[i] << std::endl;
}

void	testProfileCreation(std::vector<testProfile *>& testProfiles){
// 3. Invalid - junk after final zero
/* "POST /junk HTTP/1.1\r\n"
"Host: host\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"1\r\nx\r\n"
"0\r\n\r\nGARBAGE"
 */
	testProfile* test3 = new testProfile;
	test3->request_str = "POST /junk HTTP/1.1";
	test3->method = "POST";
	test3->version = "HTTP/1.1";
	test3->uri_str = "/junk";
	test3->uri.uri_type = ORIGIN_FORM;
	test3->uri.port_int = 80;
	test3->uri.auth = "";
	test3->uri.frag = "";
	test3->uri.host = "";
	test3->uri.pass = "";
	test3->uri.path = "/junk";
	test3->uri.port = "";
	test3->uri.query = "";
	test3->uri.scheme = "";
	test3->uri.user = "";
	test3->errNum = 400;
	test3->errMess = "Parsing Complete";
	test3->p_state = P_ERROR;
	test3->body = "x";
	test3->hasBody = true;
	test3->isChunked = true;

	test3->add(compare_request_str,"Request String ");
	test3->add(compare_request_method, "Method ");
	test3->add(compare_request_version, "Version ");
	test3->add(compare_uri_str,"URI String ");
	test3->add(compare_uri_type,"URI Type ");
	test3->add(compare_uri_port_int,"URI Port int ");
	test3->add(compare_uri_auth,"URI auth ");
	test3->add(compare_uri_frag,"URI frag ");
	test3->add(compare_uri_host,"URI host ");
	test3->add(compare_uri_pass,"URI pass ");
	test3->add(compare_uri_path,"URI path ");
	test3->add(compare_uri_port,"URI port string ");
	test3->add(compare_uri_query,"URI query ");
	test3->add(compare_uri_scheme,"URI scheme ");
	test3->add(compare_uri_user,"URI user ");
	test3->add(compare_errNum,"errNum ");
	test3->add(compare_errMess,"errMess ");
	test3->add(compare_p_state,"p_state ");
	test3->add(compare_body,"body ");
	test3->add(compare_hasBody,"hasBody ");
	test3->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test3);
}
