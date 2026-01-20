#include "tests.hpp"

void	populateTestStringsVector(std::vector<std::string>& strings){

	const char raw_str3[] = {
		'P','O','S','T',' ','/','u','p','l','o','a','d',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n',
		'4','\r','\n',
		'W','i','k','i','\r','\n',
		'5','\r','\n',
		'p','e','d','i','a','\r','\n',
		'0','\r','\n',
		'\r','\n'
	};
	const std::string str3(raw_str3, sizeof(raw_str3));
	strings.push_back(str3);

	const char raw_str4[] = {
	    'P','O','S','T',' ','/','e','m','p','t','y',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '0','\r','\n','\r','\n'
	};
	const std::string str4(raw_str4, sizeof(raw_str4));
	strings.push_back(str4);

	const char raw_str5[] = {
	    'P','O','S','T',' ','/','d','a','t','a',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','l','o','g',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','s','m','a','l','l',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','b','a','d',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','b','a','d','h','o','s','t','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '5','\r','\n','h','e','l','l','o','\r','\n'
	};
	const std::string str8(raw_str8, sizeof(raw_str8));
	strings.push_back(str8);

	const char raw_str9[] = {
	    'P','O','S','T',' ','/','b','a','d','2',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','t','e','s','t','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    'Z','Z','\r','\n','f','a','i','l','\r','\n',
	    '0','\r','\n','\r','\n'
	};
	const std::string str9(raw_str9, sizeof(raw_str9));
	strings.push_back(str9);

	const char raw_str10[] = {
	    'P','O','S','T',' ','/','b','a','d','3',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','e','r','r','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '4','h','e','l','l','o','\r','\n',
	    '0','\r','\n','\r','\n'
	};
	const std::string str10(raw_str10, sizeof(raw_str10));
	strings.push_back(str10);

	const char raw_str11[] = {
		'P','O','S','T',' ','/','b','a','d','4',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','b','u','g','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '5','\r','\n','h','e','l','\r','\n',
	    '0','\r','\n','\r','\n'
	};
	const std::string str11(raw_str11, sizeof(raw_str11));
	strings.push_back(str11);

	const char raw_str12[] = {
	    'P','O','S','T',' ','/','b','a','d','5',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','t','e','s','t','\r','\n',
	    '\r','\n',
	    '4','\r','\n','d','a','t','a','\r','\n',
	    '0','\r','\n','\r','\n'
	};
	const std::string str12(raw_str12, sizeof(raw_str12));
	strings.push_back(str12);

	const char raw_str13[] = {
	    'P','O','S','T',' ','/','u','p','p','e','r',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','t','r','a','i','l','e','r',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','c','r','l','f',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','d','a','t','a','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    'C','\r','\n',
	    'L','i','n','e','1','\r','\n',
	    'L','i','n','e','2','\r','\n',
	    '0','\r','\n',
	    '\r','\n'
	};
	const std::string str15(raw_str15, sizeof(raw_str15));
	strings.push_back(str15);

	const char raw_str16[] = {
	    'P','O','S','T',' ','/','b','i','g',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','t',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','n','e','g',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','n','o','d','a','t','a',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','b','a','d','t','r','a','i','l','e','r',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','h','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '3','\r','\n','f','o','o','\r','\n',
	    '0','\r','\n','I','n','v','a','l','i','d','H','e','a','d','e','r','\r','\n','\r','\n'
	};
	const std::string str20(raw_str20, sizeof(raw_str20));
	strings.push_back(str20);

	const char raw_str21[] = {
	    'P','O','S','T',' ','/','s','p','a','c','e',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','h','o','s','t','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '3',' ','\r','\n','a','b','c','\r','\n',
	    '0','\r','\n','\r','\n'
	};
	const std::string str21(raw_str21, sizeof(raw_str21));
	strings.push_back(str21);

	const char raw_str22[] = {
	    'P','O','S','T',' ','/','j','u','n','k',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','c','a','s','e','d',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','q','?','x','=','1',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','l','f',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','u','t','f','8',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','h','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '8','\r','\n',
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
	    'P','O','S','T',' ','/','c','u','s','t','o','m',' ','H','T','T','P','/','1','.','1','\r','\n',
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
	    'P','O','S','T',' ','/','m','i','s','s','c','r','l','f',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','x','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '1','\r','\n','a','\r','\n',
	    '0','\r','\n'
	};
	const std::string str28(raw_str28, sizeof(raw_str28));
	strings.push_back(str28);

	const char raw_str29[] = {
	    'P','O','S','T',' ','/','e','o','f',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','x','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '5','\r','\n','h','e','l','l','o','\r','\n'
	};
	const std::string str29(raw_str29, sizeof(raw_str29));
		strings.push_back(str29);

	const char raw_str30[] = {
	    'P','O','S','T',' ','/','e','x','t','r','a',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','y','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    '5','z','\r','\n','w','r','o','n','g','\r','\n',
	    '0','\r','\n','\r','\n'
	};
	const std::string str30(raw_str30, sizeof(raw_str30));
		strings.push_back(str30);

	const char raw_str31[] = {
	    'P','O','S','T',' ','/','o','o','p','s',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','z','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n',
	    'C','o','n','t','e','n','t','-','L','e','n','g','t','h',':',' ','1','0','\r','\n',
	    '2','\r','\n','h','i','\r','\n',
	    '0','\r','\n','\r','\n'
	};
	const std::string str31(raw_str31, sizeof(raw_str31));
		strings.push_back(str31);

	const char raw_str32[] = {
	    'P','O','S','T',' ','/','t','r','a','i','l',' ','H','T','T','P','/','1','.','1','\r','\n',
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

	const char *error[] = {
		"Valid - simple chunked request",
		"Valid - empty body",
		"Valid - chunk extensions",
		"Valid - trailing headers",
		"Valid - multiple small chunks",
		"Invalid - no final 0-sized chunk",
		"Invalid - chunk size not in hex",
		"Invalid - no \r\n after chunk size",
		"Invalid - incomplete chunk data",
		"Invalid - no Transfer-Encoding",
		"Valid - uppercase hex",
		"Valid - chunk size zero with trailer",
		"Valid - with CRLF inside data",
		"Valid - large chunk",
		"Valid - minimal chunk + trailer",
		"Invalid - negative chunk size",
		"Invalid - missing chunk data",
		"Invalid - malformed trailer",
		"Invalid - space in chunk size",
		"Invalid - junk after final zero",
		"Valid - case-insensitive TE",
		"Valid - chunked with query string",
		"Valid - chunk with trailing LF",
		"Valid - UTF-8 body",
		"Valid - with custom trailer",
		"Invalid - final CRLF missing",
		"Invalid - early EOF",
		"Invalid - invalid hex with extra chars",
		"Invalid - headers after body start",
		"Invalid - trailing headers without 0"
	};
	std::cout << "\nTest Outcome : " << error[i] << std::endl;
}

void	testProfileCreation(std::vector<testProfile *>& testProfiles){
// 3. Valid - simple chunked request
/* "POST /upload HTTP/1.1\r\n"
"Host: example.com\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"4\r\nWiki\r\n"
"5\r\npedia\r\n"
"0\r\n\r\n"
 */
	testProfile* test3 = new testProfile;
	test3->request_str = "POST /upload HTTP/1.1";
	test3->method = "POST";
	test3->version = "HTTP/1.1";
	test3->uri_str = "/upload";
	test3->uri.uri_type = ORIGIN_FORM;
	test3->uri.port_int = 80;
	test3->uri.auth = "";
	test3->uri.frag = "";
	test3->uri.host = "";
	test3->uri.pass = "";
	test3->uri.path = "/upload";
	test3->uri.port = "";
	test3->uri.query = "";
	test3->uri.scheme = "";
	test3->uri.user = "";
	test3->errNum = 200;
	test3->errMess = "Parsing Complete";
	test3->p_state = P_DONE;
	test3->body = "Wikipedia";
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


// 4. Valid - empty body
/* "POST /empty HTTP/1.1\r\n"
"Host: test.com\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"0\r\n\r\n"
 */
	testProfile* test4 = new testProfile;
	test4->request_str = "POST /empty HTTP/1.1";
	test4->method = "POST";
	test4->version = "HTTP/1.1";
	test4->uri_str = "/empty";
	test4->uri.uri_type = ORIGIN_FORM;
	test4->uri.port_int = 80;
	test4->uri.auth = "";
	test4->uri.frag = "";
	test4->uri.host = "";
	test4->uri.pass = "";
	test4->uri.path = "/empty";
	test4->uri.port = "";
	test4->uri.query = "";
	test4->uri.scheme = "";
	test4->uri.user = "";
	test4->errNum = 200;
	test4->errMess = "Parsing Complete";
	test4->p_state = P_DONE;
	test4->body = "";
	test4->hasBody = true;
	test4->isChunked = true;

	test4->add(compare_request_str,"Request String ");
	test4->add(compare_request_method, "Method ");
	test4->add(compare_request_version, "Version ");
	test4->add(compare_uri_str,"URI String ");
	test4->add(compare_uri_type,"URI Type ");
	test4->add(compare_uri_port_int,"URI Port int ");
	test4->add(compare_uri_auth,"URI auth ");
	test4->add(compare_uri_frag,"URI frag ");
	test4->add(compare_uri_host,"URI host ");
	test4->add(compare_uri_pass,"URI pass ");
	test4->add(compare_uri_path,"URI path ");
	test4->add(compare_uri_port,"URI port string ");
	test4->add(compare_uri_query,"URI query ");
	test4->add(compare_uri_scheme,"URI scheme ");
	test4->add(compare_uri_user,"URI user ");
	test4->add(compare_errNum,"errNum ");
	test4->add(compare_errMess,"errMess ");
	test4->add(compare_p_state,"p_state ");
	test4->add(compare_body,"body ");
	test4->add(compare_hasBody,"hasBody ");
	test4->add(compare_isChunked,"isChunked ");	
	testProfiles.push_back(test4);


// 5. Valid - chunk extensions
/* "POST /data HTTP/1.1\r\n"
"Host: server.com\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"A;ext=value\r\nHelloWorld\r\n"
"0\r\n\r\n"
 */
	testProfile* test5 = new testProfile;
	test5->request_str = "POST /data HTTP/1.1";
	test5->method = "POST";
	test5->version = "HTTP/1.1";
	test5->uri_str = "/data";
	test5->uri.uri_type = ORIGIN_FORM;
	test5->uri.port_int = 80;
	test5->uri.auth = "";
	test5->uri.frag = "";
	test5->uri.host = "";
	test5->uri.pass = "";
	test5->uri.path = "/data";
	test5->uri.port = "";
	test5->uri.query = "";
	test5->uri.scheme = "";
	test5->uri.user = "";
	test5->errNum = 200;
	test5->errMess = "Parsing Complete";
	test5->p_state = P_DONE;
	test5->body = "HelloWorld";
	test5->hasBody = true;
	test5->isChunked = true;

	test5->add(compare_request_str,"Request String ");
	test5->add(compare_request_method, "Method ");
	test5->add(compare_request_version, "Version ");
	test5->add(compare_uri_str,"URI String ");
	test5->add(compare_uri_type,"URI Type ");
	test5->add(compare_uri_port_int,"URI Port int ");
	test5->add(compare_uri_auth,"URI auth ");
	test5->add(compare_uri_frag,"URI frag ");
	test5->add(compare_uri_host,"URI host ");
	test5->add(compare_uri_pass,"URI pass ");
	test5->add(compare_uri_path,"URI path ");
	test5->add(compare_uri_port,"URI port string ");
	test5->add(compare_uri_query,"URI query ");
	test5->add(compare_uri_scheme,"URI scheme ");
	test5->add(compare_uri_user,"URI user ");
	test5->add(compare_errNum,"errNum ");
	test5->add(compare_errMess,"errMess ");
	test5->add(compare_p_state,"p_state ");
	test5->add(compare_body,"body ");
	test5->add(compare_hasBody,"hasBody ");
	test5->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test5);


// 6. Valid - trailing headers
/* "POST /log HTTP/1.1\r\n"
"Host: myhost.com\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"3\r\nabc\r\n"
"3\r\ndef\r\n"
"0\r\nTrailer-Header: value\r\n\r\n"

 */	testProfile* test6 = new testProfile;
	test6->request_str = "POST /log HTTP/1.1";
	test6->method = "POST";
	test6->version = "HTTP/1.1";
	test6->uri_str = "/log";
	test6->uri.uri_type = ORIGIN_FORM;
	test6->uri.port_int = 80;
	test6->uri.auth = "";
	test6->uri.frag = "";
	test6->uri.host = "";
	test6->uri.pass = "";
	test6->uri.path = "/log";
	test6->uri.port = "";
	test6->uri.query = "";
	test6->uri.scheme = "";
	test6->uri.user = "";
	test6->errNum = 200;
	test6->errMess = "Parsing Complete";
	test6->p_state = P_DONE;
	test6->body = "abcdef";
	test6->hasBody = true;
	test6->isChunked = true;

	test6->add(compare_request_str,"Request String ");
	test6->add(compare_request_method, "Method ");
	test6->add(compare_request_version, "Version ");
	test6->add(compare_uri_str,"URI String ");
	test6->add(compare_uri_type,"URI Type ");
	test6->add(compare_uri_port_int,"URI Port int ");
	test6->add(compare_uri_auth,"URI auth ");
	test6->add(compare_uri_frag,"URI frag ");
	test6->add(compare_uri_host,"URI host ");
	test6->add(compare_uri_pass,"URI pass ");
	test6->add(compare_uri_path,"URI path ");
	test6->add(compare_uri_port,"URI port string ");
	test6->add(compare_uri_query,"URI query ");
	test6->add(compare_uri_scheme,"URI scheme ");
	test6->add(compare_uri_user,"URI user ");
	test6->add(compare_errNum,"errNum ");
	test6->add(compare_errMess,"errMess ");
	test6->add(compare_p_state,"p_state ");
	test6->add(compare_body,"body ");
	test6->add(compare_hasBody,"hasBody ");
	test6->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test6);


// 7. Valid - multiple small chunks
/* "POST /small HTTP/1.1\r\n"
"Host: test.org\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"1\r\na\r\n"
"1\r\nb\r\n"
"1\r\nc\r\n"
"0\r\n\r\n"
 */
	testProfile* test7 = new testProfile;
	test7->request_str = "POST /small HTTP/1.1";
	test7->method = "POST";
	test7->version = "HTTP/1.1";
	test7->uri_str = "/small";
	test7->uri.uri_type = ORIGIN_FORM;
	test7->uri.port_int = 80;
	test7->uri.auth = "";
	test7->uri.frag = "";
	test7->uri.host = "";
	test7->uri.pass = "";
	test7->uri.path = "/small";
	test7->uri.port = "";
	test7->uri.query = "";
	test7->uri.scheme = "";
	test7->uri.user = "";
	test7->errNum = 200;
	test7->errMess = "Parsing Complete";
	test7->p_state = P_DONE;
	test7->body = "abc";
	test7->hasBody = true;
	test7->isChunked = true;

	test7->add(compare_request_str,"Request String ");
	test7->add(compare_request_method, "Method ");
	test7->add(compare_request_version, "Version ");
	test7->add(compare_uri_str,"URI String ");
	test7->add(compare_uri_type,"URI Type ");
	test7->add(compare_uri_port_int,"URI Port int ");
	test7->add(compare_uri_auth,"URI auth ");
	test7->add(compare_uri_frag,"URI frag ");
	test7->add(compare_uri_host,"URI host ");
	test7->add(compare_uri_pass,"URI pass ");
	test7->add(compare_uri_path,"URI path ");
	test7->add(compare_uri_port,"URI port string ");
	test7->add(compare_uri_query,"URI query ");
	test7->add(compare_uri_scheme,"URI scheme ");
	test7->add(compare_uri_user,"URI user ");
	test7->add(compare_errNum,"errNum ");
	test7->add(compare_errMess,"errMess ");
	test7->add(compare_p_state,"p_state ");
	test7->add(compare_body,"body ");
	test7->add(compare_hasBody,"hasBody ");
	test7->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test7);


// 8. Invalid - no final 0-sized chunk
/* "POST /bad HTTP/1.1\r\n"
"Host: badhost\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"5\r\nhello\r\n"
 */
	testProfile* test8 = new testProfile;
	test8->request_str = "POST /bad HTTP/1.1";
	test8->method = "POST";
	test8->version = "HTTP/1.1";
	test8->uri_str = "/bad";
	test8->uri.uri_type = ORIGIN_FORM;
	test8->uri.port_int = 80;
	test8->uri.auth = "";
	test8->uri.frag = "";
	test8->uri.host = "";
	test8->uri.pass = "";
	test8->uri.path = "/bad";
	test8->uri.port = "";
	test8->uri.query = "";
	test8->uri.scheme = "";
	test8->uri.user = "";
	test8->errNum = 400;
	test8->errMess = "Read Size of Zero : Incomplete message";
	test8->p_state = P_ERROR;
	test8->body = "hello";
	test8->hasBody = true;
	test8->isChunked = true;

	test8->add(compare_request_str,"Request String ");
	test8->add(compare_request_method, "Method ");
	test8->add(compare_request_version, "Version ");
	test8->add(compare_uri_str,"URI String ");
	test8->add(compare_uri_type,"URI Type ");
	test8->add(compare_uri_port_int,"URI Port int ");
	test8->add(compare_uri_auth,"URI auth ");
	test8->add(compare_uri_frag,"URI frag ");
	test8->add(compare_uri_host,"URI host ");
	test8->add(compare_uri_pass,"URI pass ");
	test8->add(compare_uri_path,"URI path ");
	test8->add(compare_uri_port,"URI port string ");
	test8->add(compare_uri_query,"URI query ");
	test8->add(compare_uri_scheme,"URI scheme ");
	test8->add(compare_uri_user,"URI user ");
	test8->add(compare_errNum,"errNum ");
	test8->add(compare_errMess,"errMess ");
	test8->add(compare_p_state,"p_state ");
	test8->add(compare_body,"body ");
	test8->add(compare_hasBody,"hasBody ");
	test8->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test8);


// 9. Invalid - chunk size not in hex
/* "POST /bad2 HTTP/1.1\r\n"
"Host: test\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"ZZ\r\nfail\r\n"
"0\r\n\r\n"
 */
	testProfile* test9 = new testProfile;
	test9->request_str = "POST /bad2 HTTP/1.1";
	test9->method = "POST";
	test9->version = "HTTP/1.1";
	test9->uri_str = "/bad2";
	test9->uri.uri_type = ORIGIN_FORM;
	test9->uri.port_int = 80;
	test9->uri.auth = "";
	test9->uri.frag = "";
	test9->uri.host = "";
	test9->uri.pass = "";
	test9->uri.path = "/bad2";
	test9->uri.port = "";
	test9->uri.query = "";
	test9->uri.scheme = "";
	test9->uri.user = "";
	test9->errNum = 400;
	test9->errMess = "Malformed Body : Invalid chunk size";
	test9->p_state = P_ERROR;
	test9->body = "";
	test9->hasBody = true;
	test9->isChunked = true;

	test9->add(compare_request_str,"Request String ");
	test9->add(compare_request_method, "Method ");
	test9->add(compare_request_version, "Version ");
	test9->add(compare_uri_str,"URI String ");
	test9->add(compare_uri_type,"URI Type ");
	test9->add(compare_uri_port_int,"URI Port int ");
	test9->add(compare_uri_auth,"URI auth ");
	test9->add(compare_uri_frag,"URI frag ");
	test9->add(compare_uri_host,"URI host ");
	test9->add(compare_uri_pass,"URI pass ");
	test9->add(compare_uri_path,"URI path ");
	test9->add(compare_uri_port,"URI port string ");
	test9->add(compare_uri_query,"URI query ");
	test9->add(compare_uri_scheme,"URI scheme ");
	test9->add(compare_uri_user,"URI user ");
	test9->add(compare_errNum,"errNum ");
	test9->add(compare_errMess,"errMess ");
	test9->add(compare_p_state,"p_state ");
	test9->add(compare_body,"body ");
	test9->add(compare_hasBody,"hasBody ");
	test9->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test9);


// 10. Invalid - no \r\n after chunk size
/* "POST /bad3 HTTP/1.1\r\n"
"Host: err\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"4hello\r\n"
"0\r\n\r\n"
 */
	testProfile* test10 = new testProfile;
	test10->request_str = "POST /bad3 HTTP/1.1";
	test10->method = "POST";
	test10->version = "HTTP/1.1";
	test10->uri_str = "/bad3";
	test10->uri.uri_type = ORIGIN_FORM;
	test10->uri.port_int = 80;
	test10->uri.auth = "";
	test10->uri.frag = "";
	test10->uri.host = "";
	test10->uri.pass = "";
	test10->uri.path = "/bad3";
	test10->uri.port = "";
	test10->uri.query = "";
	test10->uri.scheme = "";
	test10->uri.user = "";
	test10->errNum = 400;
	test10->errMess = "Malformed Body : Invalid chunk size";
	test10->p_state = P_ERROR;
	test10->body = "";
	test10->hasBody = true;
	test10->isChunked = true;

	test10->add(compare_request_str,"Request String ");
	test10->add(compare_request_method, "Method ");
	test10->add(compare_request_version, "Version ");
	test10->add(compare_uri_str,"URI String ");
	test10->add(compare_uri_type,"URI Type ");
	test10->add(compare_uri_port_int,"URI Port int ");
	test10->add(compare_uri_auth,"URI auth ");
	test10->add(compare_uri_frag,"URI frag ");
	test10->add(compare_uri_host,"URI host ");
	test10->add(compare_uri_pass,"URI pass ");
	test10->add(compare_uri_path,"URI path ");
	test10->add(compare_uri_port,"URI port string ");
	test10->add(compare_uri_query,"URI query ");
	test10->add(compare_uri_scheme,"URI scheme ");
	test10->add(compare_uri_user,"URI user ");
	test10->add(compare_errNum,"errNum ");
	test10->add(compare_errMess,"errMess ");
	test10->add(compare_p_state,"p_state ");
	test10->add(compare_body,"body ");
	test10->add(compare_hasBody,"hasBody ");
	test10->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test10);


// 11. Invalid - incomplete chunk data
/* "POST /bad4 HTTP/1.1\r\n"
"Host: bug\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"5\r\nhel\r\n"
"0\r\n\r\n"

 */	testProfile* test11 = new testProfile;
	test11->request_str = "POST /bad4 HTTP/1.1";
	test11->method = "POST";
	test11->version = "HTTP/1.1";
	test11->uri_str = "/bad4";
	test11->uri.uri_type = ORIGIN_FORM;
	test11->uri.port_int = 80;
	test11->uri.auth = "";
	test11->uri.frag = "";
	test11->uri.host = "";
	test11->uri.pass = "";
	test11->uri.path = "/bad4";
	test11->uri.port = "";
	test11->uri.query = "";
	test11->uri.scheme = "";
	test11->uri.user = "";
	test11->errNum = 400;
	test11->errMess = "Malformed Body : Invalid chunk termination";
	test11->p_state = P_ERROR;
	test11->body = "";
	test11->hasBody = true;
	test11->isChunked = true;

	test11->add(compare_request_str,"Request String ");
	test11->add(compare_request_method, "Method ");
	test11->add(compare_request_version, "Version ");
	test11->add(compare_uri_str,"URI String ");
	test11->add(compare_uri_type,"URI Type ");
	test11->add(compare_uri_port_int,"URI Port int ");
	test11->add(compare_uri_auth,"URI auth ");
	test11->add(compare_uri_frag,"URI frag ");
	test11->add(compare_uri_host,"URI host ");
	test11->add(compare_uri_pass,"URI pass ");
	test11->add(compare_uri_path,"URI path ");
	test11->add(compare_uri_port,"URI port string ");
	test11->add(compare_uri_query,"URI query ");
	test11->add(compare_uri_scheme,"URI scheme ");
	test11->add(compare_uri_user,"URI user ");
	test11->add(compare_errNum,"errNum ");
	test11->add(compare_errMess,"errMess ");
	test11->add(compare_p_state,"p_state ");
	test11->add(compare_body,"body ");
	test11->add(compare_hasBody,"hasBody ");
	test11->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test11);


// 12. Invalid - no Transfer-Encoding
/* "POST /bad5 HTTP/1.1\r\n"
"Host: test\r\n"
"\r\n"
"4\r\ndata\r\n"
"0\r\n\r\n"
 */
	testProfile* test12 = new testProfile;
	test12->request_str = "POST /bad5 HTTP/1.1";
	test12->method = "POST";
	test12->version = "HTTP/1.1";
	test12->uri_str = "/bad5";
	test12->uri.uri_type = ORIGIN_FORM;
	test12->uri.port_int = 80;
	test12->uri.auth = "";
	test12->uri.frag = "";
	test12->uri.host = "";
	test12->uri.pass = "";
	test12->uri.path = "/bad5";
	test12->uri.port = "";
	test12->uri.query = "";
	test12->uri.scheme = "";
	test12->uri.user = "";
	test12->errNum = 400;
	test12->errMess = "Malformed Request : Headers : content-length or transfer-encoding required for request type";
	test12->p_state = P_ERROR;
	test12->body = "";
	test12->hasBody = false;
	test12->isChunked = false;

	test12->add(compare_request_str,"Request String ");
	test12->add(compare_request_method, "Method ");
	test12->add(compare_request_version, "Version ");
	test12->add(compare_uri_str,"URI String ");
	test12->add(compare_uri_type,"URI Type ");
	test12->add(compare_uri_port_int,"URI Port int ");
	test12->add(compare_uri_auth,"URI auth ");
	test12->add(compare_uri_frag,"URI frag ");
	test12->add(compare_uri_host,"URI host ");
	test12->add(compare_uri_pass,"URI pass ");
	test12->add(compare_uri_path,"URI path ");
	test12->add(compare_uri_port,"URI port string ");
	test12->add(compare_uri_query,"URI query ");
	test12->add(compare_uri_scheme,"URI scheme ");
	test12->add(compare_uri_user,"URI user ");
	test12->add(compare_errNum,"errNum ");
	test12->add(compare_errMess,"errMess ");
	test12->add(compare_p_state,"p_state ");
	test12->add(compare_body,"body ");
	test12->add(compare_hasBody,"hasBody ");
	test12->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test12);


// 13. Valid - uppercase hex
/* "POST /upper HTTP/1.1\r\n"
"Host: site.com\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"A\r\n0123456789\r\n"
"0\r\n\r\n"
 */
	testProfile* test13 = new testProfile;
	test13->request_str = "POST /upper HTTP/1.1";
	test13->method = "POST";
	test13->version = "HTTP/1.1";
	test13->uri_str = "/upper";
	test13->uri.uri_type = ORIGIN_FORM;
	test13->uri.port_int = 80;
	test13->uri.auth = "";
	test13->uri.frag = "";
	test13->uri.host = "";
	test13->uri.pass = "";
	test13->uri.path = "/upper";
	test13->uri.port = "";
	test13->uri.query = "";
	test13->uri.scheme = "";
	test13->uri.user = "";
	test13->errNum = 200;
	test13->errMess = "Parsing Complete";
	test13->p_state = P_DONE;
	test13->body = "0123456789";
	test13->hasBody = true;
	test13->isChunked = true;

	test13->add(compare_request_str,"Request String ");
	test13->add(compare_request_method, "Method ");
	test13->add(compare_request_version, "Version ");
	test13->add(compare_uri_str,"URI String ");
	test13->add(compare_uri_type,"URI Type ");
	test13->add(compare_uri_port_int,"URI Port int ");
	test13->add(compare_uri_auth,"URI auth ");
	test13->add(compare_uri_frag,"URI frag ");
	test13->add(compare_uri_host,"URI host ");
	test13->add(compare_uri_pass,"URI pass ");
	test13->add(compare_uri_path,"URI path ");
	test13->add(compare_uri_port,"URI port string ");
	test13->add(compare_uri_query,"URI query ");
	test13->add(compare_uri_scheme,"URI scheme ");
	test13->add(compare_uri_user,"URI user ");
	test13->add(compare_errNum,"errNum ");
	test13->add(compare_errMess,"errMess ");
	test13->add(compare_p_state,"p_state ");
	test13->add(compare_body,"body ");
	test13->add(compare_hasBody,"hasBody ");
	test13->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test13);


// 14. Valid - chunk size zero with trailer
/* "POST /trailer HTTP/1.1\r\n"
"Host: host.com\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"0\r\nX-Debug: true\r\n\r\n"
 */
	testProfile* test14 = new testProfile;
	test14->request_str = "POST /trailer HTTP/1.1";
	test14->method = "POST";
	test14->version = "HTTP/1.1";
	test14->uri_str = "/trailer";
	test14->uri.uri_type = ORIGIN_FORM;
	test14->uri.port_int = 80;
	test14->uri.auth = "";
	test14->uri.frag = "";
	test14->uri.host = "";
	test14->uri.pass = "";
	test14->uri.path = "/trailer";
	test14->uri.port = "";
	test14->uri.query = "";
	test14->uri.scheme = "";
	test14->uri.user = "";
	test14->errNum = 200;
	test14->errMess = "Parsing Complete";
	test14->p_state = P_DONE;
	test14->body = "";
	test14->hasBody = true;
	test14->isChunked = true;

	test14->add(compare_request_str,"Request String ");
	test14->add(compare_request_method, "Method ");
	test14->add(compare_request_version, "Version ");
	test14->add(compare_uri_str,"URI String ");
	test14->add(compare_uri_type,"URI Type ");
	test14->add(compare_uri_port_int,"URI Port int ");
	test14->add(compare_uri_auth,"URI auth ");
	test14->add(compare_uri_frag,"URI frag ");
	test14->add(compare_uri_host,"URI host ");
	test14->add(compare_uri_pass,"URI pass ");
	test14->add(compare_uri_path,"URI path ");
	test14->add(compare_uri_port,"URI port string ");
	test14->add(compare_uri_query,"URI query ");
	test14->add(compare_uri_scheme,"URI scheme ");
	test14->add(compare_uri_user,"URI user ");
	test14->add(compare_errNum,"errNum ");
	test14->add(compare_errMess,"errMess ");
	test14->add(compare_p_state,"p_state ");
	test14->add(compare_body,"body ");
	test14->add(compare_hasBody,"hasBody ");
	test14->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test14);


// 15. Valid - with CRLF inside data
/* "POST /crlf HTTP/1.1\r\n"
"Host: data\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"C\r\nLine1\r\nLine2\r\n"
"0\r\n\r\n"
 */
	testProfile* test15 = new testProfile;
	test15->request_str = "POST /crlf HTTP/1.1";
	test15->method = "POST";
	test15->version = "HTTP/1.1";
	test15->uri_str = "/crlf";
	test15->uri.uri_type = ORIGIN_FORM;
	test15->uri.port_int = 80;
	test15->uri.auth = "";
	test15->uri.frag = "";
	test15->uri.host = "";
	test15->uri.pass = "";
	test15->uri.path = "/crlf";
	test15->uri.port = "";
	test15->uri.query = "";
	test15->uri.scheme = "";
	test15->uri.user = "";
	test15->errNum = 200;
	test15->errMess = "Parsing Complete";
	test15->p_state = P_DONE;
	test15->body = "Line1\r\nLine2";
	test15->hasBody = true;
	test15->isChunked = true;

	test15->add(compare_request_str,"Request String ");
	test15->add(compare_request_method, "Method ");
	test15->add(compare_request_version, "Version ");
	test15->add(compare_uri_str,"URI String ");
	test15->add(compare_uri_type,"URI Type ");
	test15->add(compare_uri_port_int,"URI Port int ");
	test15->add(compare_uri_auth,"URI auth ");
	test15->add(compare_uri_frag,"URI frag ");
	test15->add(compare_uri_host,"URI host ");
	test15->add(compare_uri_pass,"URI pass ");
	test15->add(compare_uri_path,"URI path ");
	test15->add(compare_uri_port,"URI port string ");
	test15->add(compare_uri_query,"URI query ");
	test15->add(compare_uri_scheme,"URI scheme ");
	test15->add(compare_uri_user,"URI user ");
	test15->add(compare_errNum,"errNum ");
	test15->add(compare_errMess,"errMess ");
	test15->add(compare_p_state,"p_state ");
	test15->add(compare_body,"body ");
	test15->add(compare_hasBody,"hasBody ");
	test15->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test15);


// 16. Valid - large chunk
/* "POST /big HTTP/1.1\r\n"
"Host: x\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"10\r\n0123456789ABCDEF\r\n"
"0\r\n\r\n"
 */
	testProfile* test16 = new testProfile;
	test16->request_str = "POST /big HTTP/1.1";
	test16->method = "POST";
	test16->version = "HTTP/1.1";
	test16->uri_str = "/big";
	test16->uri.uri_type = ORIGIN_FORM;
	test16->uri.port_int = 80;
	test16->uri.auth = "";
	test16->uri.frag = "";
	test16->uri.host = "";
	test16->uri.pass = "";
	test16->uri.path = "/big";
	test16->uri.port = "";
	test16->uri.query = "";
	test16->uri.scheme = "";
	test16->uri.user = "";
	test16->errNum = 200;
	test16->errMess = "Parsing Complete";
	test16->p_state = P_DONE;
	test16->body = "0123456789ABCDEF";
	test16->hasBody = true;
	test16->isChunked = true;

	test16->add(compare_request_str,"Request String ");
	test16->add(compare_request_method, "Method ");
	test16->add(compare_request_version, "Version ");
	test16->add(compare_uri_str,"URI String ");
	test16->add(compare_uri_type,"URI Type ");
	test16->add(compare_uri_port_int,"URI Port int ");
	test16->add(compare_uri_auth,"URI auth ");
	test16->add(compare_uri_frag,"URI frag ");
	test16->add(compare_uri_host,"URI host ");
	test16->add(compare_uri_pass,"URI pass ");
	test16->add(compare_uri_path,"URI path ");
	test16->add(compare_uri_port,"URI port string ");
	test16->add(compare_uri_query,"URI query ");
	test16->add(compare_uri_scheme,"URI scheme ");
	test16->add(compare_uri_user,"URI user ");
	test16->add(compare_errNum,"errNum ");
	test16->add(compare_errMess,"errMess ");
	test16->add(compare_p_state,"p_state ");
	test16->add(compare_body,"body ");
	test16->add(compare_hasBody,"hasBody ");
	test16->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test16);


// 17. Valid - minimal chunk + trailer
/* "POST /t HTTP/1.1\r\n"
"Host: t.com\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"1\r\n\r\n"
"0\r\nT:1\r\n\r\n"
 */
	testProfile* test17 = new testProfile;
	test17->request_str = "POST /t HTTP/1.1";
	test17->method = "POST";
	test17->version = "HTTP/1.1";
	test17->uri_str = "/t";
	test17->uri.uri_type = ORIGIN_FORM;
	test17->uri.port_int = 80;
	test17->uri.auth = "";
	test17->uri.frag = "";
	test17->uri.host = "";
	test17->uri.pass = "";
	test17->uri.path = "/t";
	test17->uri.port = "";
	test17->uri.query = "";
	test17->uri.scheme = "";
	test17->uri.user = "";
	test17->errNum = 200;
	test17->errMess = "Parsing Complete";
	test17->p_state = P_DONE;
	test17->body = "x";
	test17->hasBody = true;
	test17->isChunked = true;

	test17->add(compare_request_str,"Request String ");
	test17->add(compare_request_method, "Method ");
	test17->add(compare_request_version, "Version ");
	test17->add(compare_uri_str,"URI String ");
	test17->add(compare_uri_type,"URI Type ");
	test17->add(compare_uri_port_int,"URI Port int ");
	test17->add(compare_uri_auth,"URI auth ");
	test17->add(compare_uri_frag,"URI frag ");
	test17->add(compare_uri_host,"URI host ");
	test17->add(compare_uri_pass,"URI pass ");
	test17->add(compare_uri_path,"URI path ");
	test17->add(compare_uri_port,"URI port string ");
	test17->add(compare_uri_query,"URI query ");
	test17->add(compare_uri_scheme,"URI scheme ");
	test17->add(compare_uri_user,"URI user ");
	test17->add(compare_errNum,"errNum ");
	test17->add(compare_errMess,"errMess ");
	test17->add(compare_p_state,"p_state ");
	test17->add(compare_body,"body ");
	test17->add(compare_hasBody,"hasBody ");
	test17->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test17);


// 18. Invalid - negative chunk size
/* "POST /neg HTTP/1.1\r\n"
"Host: bad\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"-5\r\nnope\r\n"
"0\r\n\r\n"
 */
	testProfile* test18 = new testProfile;
	test18->request_str = "POST /neg HTTP/1.1";
	test18->method = "POST";
	test18->version = "HTTP/1.1";
	test18->uri_str = "/neg";
	test18->uri.uri_type = ORIGIN_FORM;
	test18->uri.port_int = 80;
	test18->uri.auth = "";
	test18->uri.frag = "";
	test18->uri.host = "";
	test18->uri.pass = "";
	test18->uri.path = "/neg";
	test18->uri.port = "";
	test18->uri.query = "";
	test18->uri.scheme = "";
	test18->uri.user = "";
	test18->errNum = 400;
	test18->errMess = "Malformed Body : Invalid chunk size";
	test18->p_state = P_ERROR;
	test18->body = "";
	test18->hasBody = true;
	test18->isChunked = true;

	test18->add(compare_request_str,"Request String ");
	test18->add(compare_request_method, "Method ");
	test18->add(compare_request_version, "Version ");
	test18->add(compare_uri_str,"URI String ");
	test18->add(compare_uri_type,"URI Type ");
	test18->add(compare_uri_port_int,"URI Port int ");
	test18->add(compare_uri_auth,"URI auth ");
	test18->add(compare_uri_frag,"URI frag ");
	test18->add(compare_uri_host,"URI host ");
	test18->add(compare_uri_pass,"URI pass ");
	test18->add(compare_uri_path,"URI path ");
	test18->add(compare_uri_port,"URI port string ");
	test18->add(compare_uri_query,"URI query ");
	test18->add(compare_uri_scheme,"URI scheme ");
	test18->add(compare_uri_user,"URI user ");
	test18->add(compare_errNum,"errNum ");
	test18->add(compare_errMess,"errMess ");
	test18->add(compare_p_state,"p_state ");
	test18->add(compare_body,"body ");
	test18->add(compare_hasBody,"hasBody ");
	test18->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test18);


// 19. Invalid - missing chunk data
/* "POST /nodata HTTP/1.1\r\n"
"Host: h\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"3\r\n\r\n"
"0\r\n\r\n"
 */
	testProfile* test19 = new testProfile;
	test19->request_str = "POST /nodata HTTP/1.1";
	test19->method = "POST";
	test19->version = "HTTP/1.1";
	test19->uri_str = "/nodata";
	test19->uri.uri_type = ORIGIN_FORM;
	test19->uri.port_int = 80;
	test19->uri.auth = "";
	test19->uri.frag = "";
	test19->uri.host = "";
	test19->uri.pass = "";
	test19->uri.path = "/nodata";
	test19->uri.port = "";
	test19->uri.query = "";
	test19->uri.scheme = "";
	test19->uri.user = "";
	test19->errNum = 400;
	test19->errMess = "Malformed Body : Invalid chunk size";
	test19->p_state = P_ERROR;
	test19->body = "\r\n0";
	test19->hasBody = true;
	test19->isChunked = true;

	test19->add(compare_request_str,"Request String ");
	test19->add(compare_request_method, "Method ");
	test19->add(compare_request_version, "Version ");
	test19->add(compare_uri_str,"URI String ");
	test19->add(compare_uri_type,"URI Type ");
	test19->add(compare_uri_port_int,"URI Port int ");
	test19->add(compare_uri_auth,"URI auth ");
	test19->add(compare_uri_frag,"URI frag ");
	test19->add(compare_uri_host,"URI host ");
	test19->add(compare_uri_pass,"URI pass ");
	test19->add(compare_uri_path,"URI path ");
	test19->add(compare_uri_port,"URI port string ");
	test19->add(compare_uri_query,"URI query ");
	test19->add(compare_uri_scheme,"URI scheme ");
	test19->add(compare_uri_user,"URI user ");
	test19->add(compare_errNum,"errNum ");
	test19->add(compare_errMess,"errMess ");
	test19->add(compare_p_state,"p_state ");
	test19->add(compare_body,"body ");
	test19->add(compare_hasBody,"hasBody ");
	test19->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test19);


// 20. Invalid - malformed trailer
/* "POST /badtrailer HTTP/1.1\r\n"
"Host: h\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"3\r\nfoo\r\n"
"0\r\nInvalidHeader\r\n\r\n"
 */
	testProfile* test20 = new testProfile;
	test20->request_str = "POST /badtrailer HTTP/1.1";
	test20->method = "POST";
	test20->version = "HTTP/1.1";
	test20->uri_str = "/badtrailer";
	test20->uri.uri_type = ORIGIN_FORM;
	test20->uri.port_int = 80;
	test20->uri.auth = "";
	test20->uri.frag = "";
	test20->uri.host = "";
	test20->uri.pass = "";
	test20->uri.path = "/badtrailer";
	test20->uri.port = "";
	test20->uri.query = "";
	test20->uri.scheme = "";
	test20->uri.user = "";
	test20->errNum = 400;
	test20->errMess = "Malformed Trailer : Invalid name";
	test20->p_state = P_ERROR;
	test20->body = "foo";
	test20->hasBody = true;
	test20->isChunked = true;

	test20->add(compare_request_str,"Request String ");
	test20->add(compare_request_method, "Method ");
	test20->add(compare_request_version, "Version ");
	test20->add(compare_uri_str,"URI String ");
	test20->add(compare_uri_type,"URI Type ");
	test20->add(compare_uri_port_int,"URI Port int ");
	test20->add(compare_uri_auth,"URI auth ");
	test20->add(compare_uri_frag,"URI frag ");
	test20->add(compare_uri_host,"URI host ");
	test20->add(compare_uri_pass,"URI pass ");
	test20->add(compare_uri_path,"URI path ");
	test20->add(compare_uri_port,"URI port string ");
	test20->add(compare_uri_query,"URI query ");
	test20->add(compare_uri_scheme,"URI scheme ");
	test20->add(compare_uri_user,"URI user ");
	test20->add(compare_errNum,"errNum ");
	test20->add(compare_errMess,"errMess ");
	test20->add(compare_p_state,"p_state ");
	test20->add(compare_body,"body ");
	test20->add(compare_hasBody,"hasBody ");
	test20->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test20);


// 21. Invalid - space in chunk size
/* "POST /space HTTP/1.1\r\n"
"Host: host\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"3 \r\nabc\r\n"
"0\r\n\r\n"
 */
	testProfile* test21 = new testProfile;
	test21->request_str = "POST /space HTTP/1.1";
	test21->method = "POST";
	test21->version = "HTTP/1.1";
	test21->uri_str = "/space";
	test21->uri.uri_type = ORIGIN_FORM;
	test21->uri.port_int = 80;
	test21->uri.auth = "";
	test21->uri.frag = "";
	test21->uri.host = "";
	test21->uri.pass = "";
	test21->uri.path = "/space";
	test21->uri.port = "";
	test21->uri.query = "";
	test21->uri.scheme = "";
	test21->uri.user = "";
	test21->errNum = 400;
	test21->errMess = "Malformed Body : Invalid chunk size";
	test21->p_state = P_ERROR;
	test21->body = "";
	test21->hasBody = true;
	test21->isChunked = true;

	test21->add(compare_request_str,"Request String ");
	test21->add(compare_request_method, "Method ");
	test21->add(compare_request_version, "Version ");
	test21->add(compare_uri_str,"URI String ");
	test21->add(compare_uri_type,"URI Type ");
	test21->add(compare_uri_port_int,"URI Port int ");
	test21->add(compare_uri_auth,"URI auth ");
	test21->add(compare_uri_frag,"URI frag ");
	test21->add(compare_uri_host,"URI host ");
	test21->add(compare_uri_pass,"URI pass ");
	test21->add(compare_uri_path,"URI path ");
	test21->add(compare_uri_port,"URI port string ");
	test21->add(compare_uri_query,"URI query ");
	test21->add(compare_uri_scheme,"URI scheme ");
	test21->add(compare_uri_user,"URI user ");
	test21->add(compare_errNum,"errNum ");
	test21->add(compare_errMess,"errMess ");
	test21->add(compare_p_state,"p_state ");
	test21->add(compare_body,"body ");
	test21->add(compare_hasBody,"hasBody ");
	test21->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test21);


// 22. Invalid - junk after final zero
/* "POST /junk HTTP/1.1\r\n"
"Host: host\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"1\r\nx\r\n"
"0\r\n\r\nGARBAGE"
 */
	testProfile* test22 = new testProfile;
	test22->request_str = "POST /junk HTTP/1.1";
	test22->method = "POST";
	test22->version = "HTTP/1.1";
	test22->uri_str = "/junk";
	test22->uri.uri_type = ORIGIN_FORM;
	test22->uri.port_int = 80;
	test22->uri.auth = "";
	test22->uri.frag = "";
	test22->uri.host = "";
	test22->uri.pass = "";
	test22->uri.path = "/junk";
	test22->uri.port = "";
	test22->uri.query = "";
	test22->uri.scheme = "";
	test22->uri.user = "";
	test22->errNum = 200;
	test22->errMess = "Parsing Complete";
	test22->p_state = P_DONE;
	test22->body = "x";
	test22->hasBody = true;
	test22->isChunked = true;

	test22->add(compare_request_str,"Request String ");
	test22->add(compare_request_method, "Method ");
	test22->add(compare_request_version, "Version ");
	test22->add(compare_uri_str,"URI String ");
	test22->add(compare_uri_type,"URI Type ");
	test22->add(compare_uri_port_int,"URI Port int ");
	test22->add(compare_uri_auth,"URI auth ");
	test22->add(compare_uri_frag,"URI frag ");
	test22->add(compare_uri_host,"URI host ");
	test22->add(compare_uri_pass,"URI pass ");
	test22->add(compare_uri_path,"URI path ");
	test22->add(compare_uri_port,"URI port string ");
	test22->add(compare_uri_query,"URI query ");
	test22->add(compare_uri_scheme,"URI scheme ");
	test22->add(compare_uri_user,"URI user ");
	test22->add(compare_errNum,"errNum ");
	test22->add(compare_errMess,"errMess ");
	test22->add(compare_p_state,"p_state ");
	test22->add(compare_body,"body ");
	test22->add(compare_hasBody,"hasBody ");
	test22->add(compare_isChunked,"isChunked ");
	
	testProfiles.push_back(test22);


// 23. Valid - case-insensitive TE
/* "POST /cased HTTP/1.1\r\n"
"Host: site\r\n"
"Transfer-Encoding: Chunked\r\n"
"\r\n"
"2\r\nhi\r\n"
"0\r\n\r\n"
 */
	testProfile* test23 = new testProfile;
	test23->request_str = "POST /cased HTTP/1.1";
	test23->method = "POST";
	test23->version = "HTTP/1.1";
	test23->uri_str = "/cased";
	test23->uri.uri_type = ORIGIN_FORM;
	test23->uri.port_int = 80;
	test23->uri.auth = "";
	test23->uri.frag = "";
	test23->uri.host = "";
	test23->uri.pass = "";
	test23->uri.path = "/cased";
	test23->uri.port = "";
	test23->uri.query = "";
	test23->uri.scheme = "";
	test23->uri.user = "";
	test23->errNum = 200;
	test23->errMess = "Parsing Complete";
	test23->p_state = P_DONE;
	test23->body = "hi";
	test23->hasBody = true;
	test23->isChunked = true;

	test23->add(compare_request_str,"Request String ");
	test23->add(compare_request_method, "Method ");
	test23->add(compare_request_version, "Version ");
	test23->add(compare_uri_str,"URI String ");
	test23->add(compare_uri_type,"URI Type ");
	test23->add(compare_uri_port_int,"URI Port int ");
	test23->add(compare_uri_auth,"URI auth ");
	test23->add(compare_uri_frag,"URI frag ");
	test23->add(compare_uri_host,"URI host ");
	test23->add(compare_uri_pass,"URI pass ");
	test23->add(compare_uri_path,"URI path ");
	test23->add(compare_uri_port,"URI port string ");
	test23->add(compare_uri_query,"URI query ");
	test23->add(compare_uri_scheme,"URI scheme ");
	test23->add(compare_uri_user,"URI user ");
	test23->add(compare_errNum,"errNum ");
	test23->add(compare_errMess,"errMess ");
	test23->add(compare_p_state,"p_state ");
	test23->add(compare_body,"body ");
	test23->add(compare_hasBody,"hasBody ");
	test23->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test23);


// 24. Valid - chunked with query string
/* "POST /q?x=1 HTTP/1.1\r\n"
"Host: s\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"6\r\nquery!\r\n"
"0\r\n\r\n"
 */
	testProfile* test24 = new testProfile;
	test24->request_str = "POST /q?x=1 HTTP/1.1";
	test24->method = "POST";
	test24->version = "HTTP/1.1";
	test24->uri_str = "/q?x=1";
	test24->uri.uri_type = ORIGIN_FORM;
	test24->uri.port_int = 80;
	test24->uri.auth = "";
	test24->uri.frag = "";
	test24->uri.host = "";
	test24->uri.pass = "";
	test24->uri.path = "/q";
	test24->uri.port = "";
	test24->uri.query = "x=1";
	test24->uri.scheme = "";
	test24->uri.user = "";
	test24->errNum = 200;
	test24->errMess = "Parsing Complete";
	test24->p_state = P_DONE;
	test24->body = "query!";
	test24->hasBody = true;  
	test24->isChunked = true;

	test24->add(compare_request_str,"Request String ");
	test24->add(compare_request_method, "Method ");
	test24->add(compare_request_version, "Version ");
	test24->add(compare_uri_str,"URI String ");
	test24->add(compare_uri_type,"URI Type ");
	test24->add(compare_uri_port_int,"URI Port int ");
	test24->add(compare_uri_auth,"URI auth ");
	test24->add(compare_uri_frag,"URI frag ");
	test24->add(compare_uri_host,"URI host ");
	test24->add(compare_uri_pass,"URI pass ");
	test24->add(compare_uri_path,"URI path ");
	test24->add(compare_uri_port,"URI port string ");
	test24->add(compare_uri_query,"URI query ");
	test24->add(compare_uri_scheme,"URI scheme ");
	test24->add(compare_uri_user,"URI user ");
	test24->add(compare_errNum,"errNum ");
	test24->add(compare_errMess,"errMess ");
	test24->add(compare_p_state,"p_state ");
	test24->add(compare_body,"body ");
	test24->add(compare_hasBody,"hasBody ");
	test24->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test24);


// 25. Valid - chunk with trailing LF
/* "POST /lf HTTP/1.1\r\n"
"Host: h\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"2\r\nok\n\r\n"
"0\r\n\r\n"
 */
	testProfile* test25 = new testProfile;
	test25->request_str = "POST /lf HTTP/1.1";
	test25->method = "POST";
	test25->version = "HTTP/1.1";
	test25->uri_str = "/lf";
	test25->uri.uri_type = ORIGIN_FORM;
	test25->uri.port_int = 80;
	test25->uri.auth = "";
	test25->uri.frag = "";
	test25->uri.host = "";
	test25->uri.pass = "";
	test25->uri.path = "/lf";
	test25->uri.port = "";
	test25->uri.query = "";
	test25->uri.scheme = "";
	test25->uri.user = "";
	test25->errNum = 200;
	test25->errMess = "Parsing Complete";
	test25->p_state = P_DONE;
	test25->body = "ok";
	test25->hasBody = true;
	test25->isChunked = true;

	test25->add(compare_request_str,"Request String ");
	test25->add(compare_request_method, "Method ");
	test25->add(compare_request_version, "Version ");
	test25->add(compare_uri_str,"URI String ");
	test25->add(compare_uri_type,"URI Type ");
	test25->add(compare_uri_port_int,"URI Port int ");
	test25->add(compare_uri_auth,"URI auth ");
	test25->add(compare_uri_frag,"URI frag ");
	test25->add(compare_uri_host,"URI host ");
	test25->add(compare_uri_pass,"URI pass ");
	test25->add(compare_uri_path,"URI path ");
	test25->add(compare_uri_port,"URI port string ");
	test25->add(compare_uri_query,"URI query ");
	test25->add(compare_uri_scheme,"URI scheme ");
	test25->add(compare_uri_user,"URI user ");
	test25->add(compare_errNum,"errNum ");
	test25->add(compare_errMess,"errMess ");
	test25->add(compare_p_state,"p_state ");
	test25->add(compare_body,"body ");
	test25->add(compare_hasBody,"hasBody ");
	test25->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test25);


// 26. Valid - UTF-8 body
/* "POST /utf8 HTTP/1.1\r\n"
"Host: h\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"4\r\nåßçø\r\n"
"0\r\n\r\n"
 */
	testProfile* test26 = new testProfile;
	test26->request_str = "POST /utf8 HTTP/1.1";
	test26->method = "POST";
	test26->version = "HTTP/1.1";
	test26->uri_str = "/utf8";
	test26->uri.uri_type = ORIGIN_FORM;
	test26->uri.port_int = 80;
	test26->uri.auth = "";
	test26->uri.frag = "";
	test26->uri.host = "";
	test26->uri.pass = "";
	test26->uri.path = "/utf8";
	test26->uri.port = "";
	test26->uri.query = "";
	test26->uri.scheme = "";
	test26->uri.user = "";
	test26->errNum = 200;
	test26->errMess = "Parsing Complete";
	test26->p_state = P_DONE;
	test26->body = "åßçø";
	test26->hasBody = true;
	test26->isChunked = true;

	test26->add(compare_request_str,"Request String ");
	test26->add(compare_request_method, "Method ");
	test26->add(compare_request_version, "Version ");
	test26->add(compare_uri_str,"URI String ");
	test26->add(compare_uri_type,"URI Type ");
	test26->add(compare_uri_port_int,"URI Port int ");
	test26->add(compare_uri_auth,"URI auth ");
	test26->add(compare_uri_frag,"URI frag ");
	test26->add(compare_uri_host,"URI host ");
	test26->add(compare_uri_pass,"URI pass ");
	test26->add(compare_uri_path,"URI path ");
	test26->add(compare_uri_port,"URI port string ");
	test26->add(compare_uri_query,"URI query ");
	test26->add(compare_uri_scheme,"URI scheme ");
	test26->add(compare_uri_user,"URI user ");
	test26->add(compare_errNum,"errNum ");
	test26->add(compare_errMess,"errMess ");
	test26->add(compare_p_state,"p_state ");
	test26->add(compare_body,"body ");
	test26->add(compare_hasBody,"hasBody ");
	test26->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test26);


// 27. Valid - with custom trailer
/* "POST /custom HTTP/1.1\r\n"
"Host: test\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"4\r\ntest\r\n"
"0\r\nX-Trailer: yes\r\n\r\n"
 */
	testProfile* test27 = new testProfile;
	test27->request_str = "POST /custom HTTP/1.1";
	test27->method = "POST";
	test27->version = "HTTP/1.1";
	test27->uri_str = "/custom";
	test27->uri.uri_type = ORIGIN_FORM;
	test27->uri.port_int = 80;
	test27->uri.auth = "";
	test27->uri.frag = "";
	test27->uri.host = "";
	test27->uri.pass = "";
	test27->uri.path = "/custom";
	test27->uri.port = "";
	test27->uri.query = "";
	test27->uri.scheme = "";
	test27->uri.user = "";
	test27->errNum = 200;
	test27->errMess = "Parsing Complete";
	test27->p_state = P_DONE;
	test27->body = "test";
	test27->hasBody = true;
	test27->isChunked = true;

	test27->add(compare_request_str,"Request String ");
	test27->add(compare_request_method, "Method ");
	test27->add(compare_request_version, "Version ");
	test27->add(compare_uri_str,"URI String ");
	test27->add(compare_uri_type,"URI Type ");
	test27->add(compare_uri_port_int,"URI Port int ");
	test27->add(compare_uri_auth,"URI auth ");
	test27->add(compare_uri_frag,"URI frag ");
	test27->add(compare_uri_host,"URI host ");
	test27->add(compare_uri_pass,"URI pass ");
	test27->add(compare_uri_path,"URI path ");
	test27->add(compare_uri_port,"URI port string ");
	test27->add(compare_uri_query,"URI query ");
	test27->add(compare_uri_scheme,"URI scheme ");
	test27->add(compare_uri_user,"URI user ");
	test27->add(compare_errNum,"errNum ");
	test27->add(compare_errMess,"errMess ");
	test27->add(compare_p_state,"p_state ");
	test27->add(compare_body,"body ");
	test27->add(compare_hasBody,"hasBody ");
	test27->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test27);


// 28. Invalid - final CRLF missing
/* 
"POST /misscrlf HTTP/1.1\r\n"
"Host: x\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"1\r\na\r\n"
"0\r\n"
 */
	testProfile* test28 = new testProfile;
	test28->request_str = "POST /misscrlf HTTP/1.1";
	test28->method = "POST";
	test28->version = "HTTP/1.1";
	test28->uri_str = "/misscrlf";
	test28->uri.uri_type = ORIGIN_FORM;
	test28->uri.port_int = 80;
	test28->uri.auth = "";
	test28->uri.frag = "";
	test28->uri.host = "";
	test28->uri.pass = "";
	test28->uri.path = "/misscrlf";
	test28->uri.port = "";
	test28->uri.query = "";
	test28->uri.scheme = "";
	test28->uri.user = "";
	test28->errNum = 400;
	test28->errMess = "Read Size of Zero : Incomplete message";
	test28->p_state = P_ERROR;
	test28->body = "a";
	test28->hasBody = true;
	test28->isChunked = true;

	test28->add(compare_request_str,"Request String ");
	test28->add(compare_request_method, "Method ");
	test28->add(compare_request_version, "Version ");
	test28->add(compare_uri_str,"URI String ");
	test28->add(compare_uri_type,"URI Type ");
	test28->add(compare_uri_port_int,"URI Port int ");
	test28->add(compare_uri_auth,"URI auth ");
	test28->add(compare_uri_frag,"URI frag ");
	test28->add(compare_uri_host,"URI host ");
	test28->add(compare_uri_pass,"URI pass ");
	test28->add(compare_uri_path,"URI path ");
	test28->add(compare_uri_port,"URI port string ");
	test28->add(compare_uri_query,"URI query ");
	test28->add(compare_uri_scheme,"URI scheme ");
	test28->add(compare_uri_user,"URI user ");
	test28->add(compare_errNum,"errNum ");
	test28->add(compare_errMess,"errMess ");
	test28->add(compare_p_state,"p_state ");
	test28->add(compare_body,"body ");
	test28->add(compare_hasBody,"hasBody ");
	test28->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test28);


// 29. Invalid - early EOF
/* 
"POST /eof HTTP/1.1\r\n"
"Host: x\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"  // no 0 chunk
"5\r\nhello\r\n" 
*/
	testProfile* test29 = new testProfile;
	test29->request_str = "POST /eof HTTP/1.1";
	test29->method = "POST";
	test29->version = "HTTP/1.1";
	test29->uri_str = "/eof";
	test29->uri.uri_type = ORIGIN_FORM;
	test29->uri.port_int = 80;
	test29->uri.auth = "";
	test29->uri.frag = "";
	test29->uri.host = "";
	test29->uri.pass = "";
	test29->uri.path = "/eof";
	test29->uri.port = "";
	test29->uri.query = "";
	test29->uri.scheme = "";
	test29->uri.user = "";
	test29->errNum = 400;
	test29->errMess = "Read Size of Zero : Incomplete message";
	test29->p_state = P_ERROR;
	test29->body = "hello";
	test29->hasBody = true;
	test29->isChunked = true;

	test29->add(compare_request_str,"Request String ");
	test29->add(compare_request_method, "Method ");
	test29->add(compare_request_version, "Version ");
	test29->add(compare_uri_str,"URI String ");
	test29->add(compare_uri_type,"URI Type ");
	test29->add(compare_uri_port_int,"URI Port int ");
	test29->add(compare_uri_auth,"URI auth ");
	test29->add(compare_uri_frag,"URI frag ");
	test29->add(compare_uri_host,"URI host ");
	test29->add(compare_uri_pass,"URI pass ");
	test29->add(compare_uri_path,"URI path ");
	test29->add(compare_uri_port,"URI port string ");
	test29->add(compare_uri_query,"URI query ");
	test29->add(compare_uri_scheme,"URI scheme ");
	test29->add(compare_uri_user,"URI user ");
	test29->add(compare_errNum,"errNum ");
	test29->add(compare_errMess,"errMess ");
	test29->add(compare_p_state,"p_state ");
	test29->add(compare_body,"body ");
	test29->add(compare_hasBody,"hasBody ");
	test29->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test29);

// 30. Invalid - invalid hex with extra chars
/* 
"POST /extra HTTP/1.1\r\n"
"Host: y\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"5z\r\nwrong\r\n"
"0\r\n\r\n"
 */
	testProfile* test30 = new testProfile;
	test30->request_str = "POST /extra HTTP/1.1";
	test30->method = "POST";
	test30->version = "HTTP/1.1";
	test30->uri_str = "/extra";
	test30->uri.uri_type = ORIGIN_FORM;
	test30->uri.port_int = 80;
	test30->uri.auth = "";
	test30->uri.frag = "";
	test30->uri.host = "";
	test30->uri.pass = "";
	test30->uri.path = "/extra";
	test30->uri.port = "";
	test30->uri.query = "";
	test30->uri.scheme = "";
	test30->uri.user = "";
	test30->errNum = 400;
	test30->errMess = "Malformed Body : Invalid chunk size";
	test30->p_state = P_ERROR;
	test30->body = "";
	test30->hasBody = true;
	test30->isChunked = true;

	test30->add(compare_request_str,"Request String ");
	test30->add(compare_request_method, "Method ");
	test30->add(compare_request_version, "Version ");
	test30->add(compare_uri_str,"URI String ");
	test30->add(compare_uri_type,"URI Type ");
	test30->add(compare_uri_port_int,"URI Port int ");
	test30->add(compare_uri_auth,"URI auth ");
	test30->add(compare_uri_frag,"URI frag ");
	test30->add(compare_uri_host,"URI host ");
	test30->add(compare_uri_pass,"URI pass ");
	test30->add(compare_uri_path,"URI path ");
	test30->add(compare_uri_port,"URI port string ");
	test30->add(compare_uri_query,"URI query ");
	test30->add(compare_uri_scheme,"URI scheme ");
	test30->add(compare_uri_user,"URI user ");
	test30->add(compare_errNum,"errNum ");
	test30->add(compare_errMess,"errMess ");
	test30->add(compare_p_state,"p_state ");
	test30->add(compare_body,"body ");
	test30->add(compare_hasBody,"hasBody ");
	test30->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test30);


// 31. Invalid - headers after body start
/* 
"POST /oops HTTP/1.1\r\n"
"Host: z\r\n"
"Transfer-Encoding: chunked\r\n" // Conflict!
"\r\n"
"2\r\nhi\r\n"
"0\r\n\r\n"
 */
	testProfile* test31 = new testProfile;
	test31->request_str = "POST /oops HTTP/1.1";
	test31->method = "POST";
	test31->version = "HTTP/1.1";
	test31->uri_str = "/oops";
	test31->uri.uri_type = ORIGIN_FORM;
	test31->uri.port_int = 80;
	test31->uri.auth = "";
	test31->uri.frag = "";
	test31->uri.host = "";
	test31->uri.pass = "";
	test31->uri.path = "/oops";
	test31->uri.port = "";
	test31->uri.query = "";
	test31->uri.scheme = "";
	test31->uri.user = "";
	test31->errNum = 400;
	test31->errMess = "Malformed Body : Invalid chunk size";
	test31->p_state = P_ERROR;
	test31->body = "";
	test31->hasBody = true;
	test31->isChunked = true;

	test31->add(compare_request_str,"Request String ");
	test31->add(compare_request_method, "Method ");
	test31->add(compare_request_version, "Version ");
	test31->add(compare_uri_str,"URI String ");
	test31->add(compare_uri_type,"URI Type ");
	test31->add(compare_uri_port_int,"URI Port int ");
	test31->add(compare_uri_auth,"URI auth ");
	test31->add(compare_uri_frag,"URI frag ");
	test31->add(compare_uri_host,"URI host ");
	test31->add(compare_uri_pass,"URI pass ");
	test31->add(compare_uri_path,"URI path ");
	test31->add(compare_uri_port,"URI port string ");
	test31->add(compare_uri_query,"URI query ");
	test31->add(compare_uri_scheme,"URI scheme ");
	test31->add(compare_uri_user,"URI user ");
	test31->add(compare_errNum,"errNum ");
	test31->add(compare_errMess,"errMess ");
	test31->add(compare_p_state,"p_state ");
	test31->add(compare_body,"body ");
	test31->add(compare_hasBody,"hasBody ");
	test31->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test31);


// 32. Invalid - trailing headers without 0
/* 
"POST /trail HTTP/1.1\r\n"
"Host: site\r\n"
"Transfer-Encoding: chunked\r\n"
"\r\n"
"2\r\nok\r\n"
"X-Bad: oops\r\n"
*/

	testProfile* test32 = new testProfile;
	test32->request_str = "POST /trail HTTP/1.1";
	test32->method = "POST";
	test32->version = "HTTP/1.1";
	test32->uri_str = "/trail";
	test32->uri.uri_type = ORIGIN_FORM;
	test32->uri.port_int = 80;
	test32->uri.auth = "";
	test32->uri.frag = "";
	test32->uri.host = "";
	test32->uri.pass = "";
	test32->uri.path = "/trail";
	test32->uri.port = "";
	test32->uri.query = "";
	test32->uri.scheme = "";
	test32->uri.user = "";
	test32->errNum = 400;
	test32->errMess = "Malformed Body : Invalid chunk size";
	test32->p_state = P_ERROR;
	test32->body = "ok";
	test32->hasBody = true;
	test32->isChunked = true;

	test32->add(compare_request_str,"Request String ");
	test32->add(compare_request_method, "Method ");
	test32->add(compare_request_version, "Version ");
	test32->add(compare_uri_str,"URI String ");
	test32->add(compare_uri_type,"URI Type ");
	test32->add(compare_uri_port_int,"URI Port int ");
	test32->add(compare_uri_auth,"URI auth ");
	test32->add(compare_uri_frag,"URI frag ");
	test32->add(compare_uri_host,"URI host ");
	test32->add(compare_uri_pass,"URI pass ");
	test32->add(compare_uri_path,"URI path ");
	test32->add(compare_uri_port,"URI port string ");
	test32->add(compare_uri_query,"URI query ");
	test32->add(compare_uri_scheme,"URI scheme ");
	test32->add(compare_uri_user,"URI user ");
	test32->add(compare_errNum,"errNum ");
	test32->add(compare_errMess,"errMess ");
	test32->add(compare_p_state,"p_state ");
	test32->add(compare_body,"body ");
	test32->add(compare_hasBody,"hasBody ");
	test32->add(compare_isChunked,"isChunked ");	
	
	testProfiles.push_back(test32);	
}