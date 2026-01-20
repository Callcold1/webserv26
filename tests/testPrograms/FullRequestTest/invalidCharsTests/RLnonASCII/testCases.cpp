#include "tests.hpp"

void	populateTestStringsVector(std::vector<std::string>& strings){

	const char raw_str3[] = {
		'P','O','S','T','\x01',' ','/','u','p','l','o','a','d',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'4','\r','\n','W','i','k','i','\r','\n',
		'5','\r','\n','p','e','d','i','a','\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str3(raw_str3, sizeof(raw_str3));
	strings.push_back(str3);


	const char raw_str4[] = {
		'P','O','S','T',' ','/','e','m','p','t','\x7F','y',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str4(raw_str4, sizeof(raw_str4));
	strings.push_back(str4);

	const char raw_str5[] = {
		'P','O','S','T',' ','/','d','a','\x00','t','a',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','s','e','r','v','e','r','.','c','o','m','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'A',';','e','x','t','=','v','a','l','u','e','\r','\n',
		'H','e','l','l','o','W','o','r','l','d','\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str5(raw_str5, sizeof(raw_str5));
	strings.push_back(str5);

	const char raw_str6[] = {
		'P','O','S','\x19','T',' ','/','l','o','g',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','m','y','h','o','s','t','.','c','o','m','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'3','\r','\n','a','b','c','\r','\n',
		'3','\r','\n','d','e','f','\r','\n',
		'0','\r','\n','T','r','a','i','l','e','r','-','H','e','a','d','e','r',':',' ','v','a','l','u','e','\r','\n',
		'\r','\n'
	};
	const std::string str6(raw_str6, sizeof(raw_str6));
	strings.push_back(str6);

	const char raw_str7[] = {
		'P','O','S','T',' ','/','s','m','a','l','\x1B','l',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','o','r','g','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'1','\r','\n','a','\r','\n',
		'1','\r','\n','b','\r','\n',
		'1','\r','\n','c','\r','\n',
		'0','\r','\n',
		'\r','\n'
	};
	const std::string str7(raw_str7, sizeof(raw_str7));
	strings.push_back(str7);

	const char raw_str8[] = {
		'P','O','\x7F','S','T',' ','/','b','a','d',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','b','a','d','h','o','s','t','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'5','\r','\n','h','e','l','l','o','\r','\n'
	};
	const std::string str8(raw_str8, sizeof(raw_str8));
	strings.push_back(str8);

	const char raw_str9[] = {
		'P','O','S','T',' ','/','b','a','\x80','g',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'Z','Z','\r','\n','f','a','i','l','\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str9(raw_str9, sizeof(raw_str9));
	strings.push_back(str9);

	const char raw_str10[] = {
		'P','O','S','T',' ','/','b','\x19','h','a','d','3',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','e','r','r','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'4','h','e','l','l','o','\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str10(raw_str10, sizeof(raw_str10));
	strings.push_back(str10);

	const char raw_str11[] = {
			'P','O','S','T',' ','/','b','a','d','\x00','4',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','b','u','g','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'5','\r','\n','h','e','l','\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str11(raw_str11, sizeof(raw_str11));
	strings.push_back(str11);

	const char raw_str12[] = {
		'P','O','S','T',' ','/','b','a','d','5','\x10',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','\r','\n',
		'\r','\n',
		'4','\r','\n','d','a','t','a','\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str12(raw_str12, sizeof(raw_str12));
	strings.push_back(str12);

	const char raw_str13[] = {
		'P','O','S','T',' ','/','u','p','\x7F','p','e','r',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','s','i','t','e','.','c','o','m','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'A','\r','\n',
		'0','1','2','3','4','5','6','7','8','9','\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str13(raw_str13, sizeof(raw_str13));
	strings.push_back(str13);

	const char raw_str14[] = {
		'P','O','S','T',' ','/','t','r','a','i','\x1F','l','e','r',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','h','o','s','t','.','c','o','m','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'0','\r','\n',
		'X','-','D','e','b','u','g',':',' ','t','r','u','e','\r','\n',
		'\r','\n'
	};
	const std::string str14(raw_str14, sizeof(raw_str14));
	strings.push_back(str14);

	const char raw_str15[] = {
		'P','O','S','T',' ','/','c','r','\x7F','l','f',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','d','a','t','a','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'B','\r','\n',
		'L','i','n','e','1','\r','\n',
		'L','i','n','e','2','\r','\n',
		'0','\r','\n',
		'\r','\n'
	};
	const std::string str15(raw_str15, sizeof(raw_str15));
	strings.push_back(str15);

	const char raw_str16[] = {
		'P','O','S','T',' ','/','b','i','\x00','g',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','x','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'1','0','\r','\n',
		'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','\r','\n',
		'0','\r','\n',
		'\r','\n'
	};
	const std::string str16(raw_str16, sizeof(raw_str16));
	strings.push_back(str16);


	const char raw_str17[] = {
		'P','O','S','T',' ','/','t','\x19',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','.','c','o','m','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'1','\r','\n',
		'x','\r','\n',
		'0','\r','\n',
		'T',':','1','\r','\n',
		'\r','\n'
	};
	const std::string str17(raw_str17, sizeof(raw_str17));
	strings.push_back(str17);

	const char raw_str18[] = {
		'P','O','S','T',' ','/','n','e','\x80','g',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','b','a','d','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'-','5','\r','\n',
		'n','o','p','e','\r','\n',
		'0','\r','\n',
		'\r','\n'
	};
	const std::string str18(raw_str18, sizeof(raw_str18));
	strings.push_back(str18);

	const char raw_str19[] = {
		'P','O','S','T',' ','/','n','o','d','\x01','a','t','a',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','h','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'3','\r','\n','\r','\n', // chunk size 3, chunk data is 2 bytes: \r\n (should actually be 3 bytes though, so this is malformed)
		'0','\r','\n',
		'\r','\n'
	};
	const std::string str19(raw_str19, sizeof(raw_str19));
	strings.push_back(str19);

	const char raw_str20[] = {
		'P','O','S','T',' ','/','b','a','d','\x7F','t','r','a','i','l','e','r',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','h','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'3','\r','\n','f','o','o','\r','\n',
		'0','\r','\n','I','n','v','a','l','i','d','H','e','a','d','e','r','\r','\n','\r','\n'
	};
	const std::string str20(raw_str20, sizeof(raw_str20));
	strings.push_back(str20);

	const char raw_str21[] = {
		'P','O','S','T',' ','/','s','p','\x00','t','a','c','e',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','h','o','s','t','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'3',' ','\r','\n','a','b','c','\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str21(raw_str21, sizeof(raw_str21));
	strings.push_back(str21);

	const char raw_str22[] = {
		'P','O','S','T',' ','/','j','u','\x7F','n','k',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','h','o','s','t','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'1','\r','\n','x','\r','\n',
		'0','\r','\n',
		'\r','\n',
		'G','A','R','B','A','G','E'
	};
	const std::string str22(raw_str22, sizeof(raw_str22));
	strings.push_back(str22);

	const char raw_str23[] = {
		'P','O','S','T',' ','/','c','a','\x19','s','e','d',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','s','i','t','e','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','C','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'2','\r','\n','h','i','\r','\n',
		'0','\r','\n',
		'\r','\n'
	};
	const std::string str23(raw_str23, sizeof(raw_str23));
	strings.push_back(str23);

	const char raw_str24[] = {
		'P','O','S','T',' ','/','q','?','\x7F','x','=','1',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','s','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'6','\r','\n','q','u','e','r','y','!','\r','\n',
		'0','\r','\n',
		'\r','\n'
	};
	const std::string str24(raw_str24, sizeof(raw_str24));
	strings.push_back(str24);

	const char raw_str25[] = {
		'P','O','S','T',' ','/','l','\x00','f',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','h','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'2','\r','\n','o','k','\n','\r','\n',
		'0','\r','\n',
		'\r','\n'
	};
	const std::string str25(raw_str25, sizeof(raw_str25));
	strings.push_back(str25);

	const char raw_str26[] = {
		'P','O','S','T',' ','/','u','t','\x1','g','9','f','8',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','h','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'4','\r','\n',
		'\xC3','\xA5', // å in UTF-8
		'\xC3','\x9F', // ß in UTF-8
		'\xC3','\xA7', // ç in UTF-8
		'\xC3','\xB8', // ø in UTF-8
		'\r','\n',
		'0','\r','\n',
		'\r','\n'
	};
	const std::string str26(raw_str26, sizeof(raw_str26));
	strings.push_back(str26);

	const char raw_str27[] = {
		'P','O','S','T',' ','/','c','u','s','\x7F','t','o','m',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'4','\r','\n','t','e','s','t','\r','\n',
		'0','\r','\n',
		'X','-','T','r','a','i','l','e','r',':',' ','y','e','s','\r','\n',
		'\r','\n'
	};
	const std::string str27(raw_str27, sizeof(raw_str27));
	strings.push_back(str27);

	const char raw_str28[] = {
		'P','O','S','T',' ','/','m','i','s','\x7F','s','c','r','l','f',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','x','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'1','\r','\n','a','\r','\n',
		'0','\r','\n'
	};
	const std::string str28(raw_str28, sizeof(raw_str28));
	strings.push_back(str28);

	const char raw_str29[] = {
		'P','O','S','T',' ','/','e','\x00','o','f',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','x','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'5','\r','\n','h','e','l','l','o','\r','\n'
	};
	const std::string str29(raw_str29, sizeof(raw_str29));
	strings.push_back(str29);

	const char raw_str30[] = {
		'P','O','S','T',' ','/','e','x','\x19','t','r','a',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','y','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'5','z','\r','\n','w','r','o','n','g','\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str30(raw_str30, sizeof(raw_str30));
	strings.push_back(str30);

	const char raw_str31[] = {
		'P','O','S','T',' ','/','o','o','\x7F','p','s',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','z','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'C','o','n','t','e','n','t','-','L','e','n','g','t','h',':',' ','1','0','\r','\n',
		'\r','\n',
		'2','\r','\n','h','i','\r','\n',
		'0','\r','\n','\r','\n'
	};
	const std::string str31(raw_str31, sizeof(raw_str31));
	strings.push_back(str31);

	const char raw_str32[] = {
		'P','O','S','T',' ','/','t','r','\x00','a','i','l',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','s','i','t','e','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'2','\r','\n','o','k','\r','\n',
		'X','-','B','a','d',':',' ','o','o','p','s','\r','\n'
	};
	const std::string str32(raw_str32, sizeof(raw_str32));
	strings.push_back(str32);


	for (size_t i = 3; i < strings.size() + 3; i++)
	{	
		std::string filename = "./testRequestFiles/str" + iToStr(i);
		writeStringToFile(strings[i - 3], filename);
	}
}

void	printErrorString(int i){

	const char *error[] = {		//THIS IS TO EITHER EXPLAIN EXPECTED RESULT, OR WHAT IS TARGETED IN THE TEST
		"Valid GET request",
		"Valid POST request",
		"Valid DELETE request",
		"Invalid request: Missing HTTP version",
		"Invalid request: Unsupported HTTP method",
		"Invalid request: Malformed request line",
		"Valid GET with query string",
		"Invalid POST: Missing Content-Length header",
		"Valid POST with JSON",
		"Invalid GET: No Host header (HTTP/1.1 requires it)",
		"Valid DELETE with query params",
		"Invalid request: Bad HTTP version",
		"Valid GET: root request",
		"Invalid request: Method in lowercase",
		"Valid POST: form submission",
		"Invalid request: Missing request URI",
		"Valid DELETE: delete resource with slash",
		"Invalid POST: Content-Length mismatch (actual body smaller)",
		"Valid GET with custom header",
		"Invalid GET: Missing space between method and URI",
		"Valid POST with multipart/form-data",
		"Invalid DELETE: Extra characters after HTTP version",
		"Valid GET with fragment (fragment ignored in request)",
		"Invalid POST: Missing empty line before body",
		"Valid GET: multiple headers",
		"Invalid GET: Spaces in URI without encoding",
		"Valid POST: empty body (Content-Length: 0)",
		"Invalid DELETE: lowercase HTTP version",
		"Valid GET with IPv6 host",
		"Invalid POST: Missing Host header and Content-Length"
	};
	std::cout << "\nTest Outcome : " << error[i] << std::endl;
}