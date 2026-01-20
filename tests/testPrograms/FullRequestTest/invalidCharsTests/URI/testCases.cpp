#include "tests.hpp"
#include <cassert>
#include "RequestTypes.hpp"

void	populateTestStringsVector(std::vector<std::string>& strings){

	const char raw_str3[] = {
		'P','O','S','T',' ','h','t','t','p',':','/','/','u','s','e','r','@',':','8','0',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','a','p','i','.','t','e','s','t','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str3(raw_str3, sizeof(raw_str3));
	strings.push_back(str3);

	const char raw_str4[] = {
		'P','O','S','T',' ',':','/','/','e','x','a','m','p','l','e','.','c','o','m',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','j','s','.','s','e','r','v','e','r','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str4(raw_str4, sizeof(raw_str4));
	strings.push_back(str4);

	const char raw_str5[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m',':','9','9','9','9','9','9',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','h','o','s','t','n','a','m','e','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str5(raw_str5, sizeof(raw_str5));
	strings.push_back(str5);

	const char raw_str6[] = {
	    'P','O','S','T',' ','h','t','t','p',':','e','x','a','m','p','l','e','.','c','o','m',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','c','f','g','.','s','v','c','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str6(raw_str6, sizeof(raw_str6));
	strings.push_back(str6);

	const char raw_str7[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','t','e','\\','\"','s','t',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','d','a','t','a','.','o','r','g','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str7(raw_str7, sizeof(raw_str7));
	strings.push_back(str7);

	const char raw_str8[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a',' ','m','p','l','e','.','c','o','m',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','b','a','d','.','c','o','m','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str8(raw_str8, sizeof(raw_str8));
	strings.push_back(str8);

	const char raw_str9[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','p','a','^','t','h',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','t','e','s','t','.','n','e','t','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str9(raw_str9, sizeof(raw_str9));
	strings.push_back(str9);

	const char raw_str10[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','t','e','s','t','#',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','t','e','s','t','.','o','r','g','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str10(raw_str10, sizeof(raw_str10));
	strings.push_back(str10);

	const char raw_str11[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','?','#',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','a','.','b','.','c','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str11(raw_str11, sizeof(raw_str11));
	strings.push_back(str11);

	const char raw_str12[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','?','q','u','e','r','y','=','b','a','d',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','x','.','y','.','z','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str12(raw_str12, sizeof(raw_str12));
	strings.push_back(str12);

	const char raw_str13[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','p','a','|','t','h',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','m','y','.','s','i','t','e','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str13(raw_str13, sizeof(raw_str13));
	strings.push_back(str13);

	const char raw_str14[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m',':','8','0',':','9','0',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','a','b','c','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str14(raw_str14, sizeof(raw_str14));
	strings.push_back(str14);

	const char raw_str15[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m',':','/','p','a','t','h',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','t','e','s','t','.','o','r','g','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str15(raw_str15, sizeof(raw_str15));
	strings.push_back(str15);

	const char raw_str16[] = {
		'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','%','G','1',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','e','x','a','m','p','l','e','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str16(raw_str16, sizeof(raw_str16));
	strings.push_back(str16);

	const char raw_str17[] = {
		'P','O','S','T',' ','f','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','<','>',
		' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','b','l','a','h','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str17(raw_str17, sizeof(raw_str17));
	strings.push_back(str17);
		
	const char raw_str18[] = {
		'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','/','?','?',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','d','o','c','.','l','o','c','a','l','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str18(raw_str18, sizeof(raw_str18));
	strings.push_back(str18);

	const char raw_str19[] = {
		'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','p','a','t','h','?','q','u','e','r','y','=','t','e','{','}','s','t',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','s','e','r','v','e','r','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str19(raw_str19, sizeof(raw_str19));
	strings.push_back(str19);

	const char raw_str20[] = {
		'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','|','m','p','l','e','.','c','o','m',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','a','p','i','.','s','e','r','v','e','r','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str20(raw_str20, sizeof(raw_str20));
	strings.push_back(str20);

	const char raw_str21[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','[','e','x','a','m','p','l','e','.','c','o','m',']',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','w','e','b','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str21(raw_str21, sizeof(raw_str21));
	strings.push_back(str21);

	const char raw_str22[] = {
		'P','O','S','T',' ','h','t','t','p',':','/','/','/','a','b','c',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','s','i','t','e','.','e','x','a','m','p','l','e','\r','\n',
		'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
		'\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str22(raw_str22, sizeof(raw_str22));
	strings.push_back(str22);

	const char raw_str23[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','i','n','v','a','l','i','d',' ','s','p','a','c','e',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','s','i','t','e','.','c','o','m','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str23(raw_str23, sizeof(raw_str23));
	strings.push_back(str23);

	const char raw_str24[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','<','>',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','h','o','s','t','.','c','o','m','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str24(raw_str24, sizeof(raw_str24));
	strings.push_back(str24);

	const char raw_str25[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','{','}','t','e','s','t',' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','m','y','s','i','t','e','.','o','r','g','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str25(raw_str25, sizeof(raw_str25));
	strings.push_back(str25);

	const char raw_str26[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','t','e','s','t','<','p','a','t','h','>',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','q','u','e','r','y','.','t','e','s','t','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str26(raw_str26, sizeof(raw_str26));
	strings.push_back(str26);

	const char raw_str27[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','%',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str27(raw_str27, sizeof(raw_str27));
	strings.push_back(str27);

	const char raw_str28[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','%','Z','Z',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','w','w','w','.','t','e','s','t','.','n','e','t','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str28(raw_str28, sizeof(raw_str28));
	strings.push_back(str28);

	const char raw_str29[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','[',':',':','1',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','a','b','c','.','c','o','m','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str29(raw_str29, sizeof(raw_str29));
	strings.push_back(str29);

	const char raw_str30[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','t','e','s','t','|','f','i','l','e',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','d','e','m','o','.','s','i','t','e','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str30(raw_str30, sizeof(raw_str30));
	strings.push_back(str30);

	const char raw_str31[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m','/','t','e','s','t','\\','\\','p','a','t','h',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','f','i','l','e','.','s','e','r','v','e','r','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
	};
	const std::string str31(raw_str31, sizeof(raw_str31));
	strings.push_back(str31);

	const char raw_str32[] = {
	    'P','O','S','T',' ','h','t','t','p',':','/','/','e','x','a','m','p','l','e','.','c','o','m',':','a','b','c',
		' ','H','T','T','P','/','1','.','1','\r','\n',
	    'H','o','s','t',':',' ','s','e','a','r','c','h','.','t','o','o','l','\r','\n',
	    'T','r','a','n','s','f','e','r','-','E','n','c','o','d','i','n','g',':',' ','c','h','u','n','k','e','d','\r','\n',
	    '\r','\n','0','\r','\n','\r','\n'
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
		"missing host",
		"missing scheme",
		"port out of range",
		"missing `//`",
		"quote in path",
		"space in host",
		"caret in path",
		"empty fragment",
		"misplaced query/fragment",
		"missing authority",
		"illegal pipe in path",
		"multiple ports",
		"empty port",
		"invalid percent-encoding",
		"illegal in FTP too",
		"invalid double query",
		"curly braces in query",
		"pipe again",
		"brackets in hostname",
		"missing host",
		"unescaped space",
		"illegal angle brackets",
		"illegal curly braces",
		"illegal path characters",
		"incomplete percent-encoding",
		"invalid hex in percent",
		"malformed IPv6 address",
		"illegal pipe character",
		"backslash instead of slash",
		"invalid port (non-numeric)"
	};
	std::cout << "Test Outcome : " << error[i] << std::endl;
}

