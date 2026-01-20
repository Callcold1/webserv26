#include "tests.hpp"

void	populateTestStringsVector(std::vector<std::string>& strings){


	const char raw_str3[] = {
		'G','E','T',' ','/','i','n','d','e','x','.','h','t','m','l',' ','H','T','T','P','/','1','.','1','\r','\n',

		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};

	const char raw_str4[] = {
		'P','O','S','T',' ','/','s','u','b','m','i','t','-','f','o','r','m',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'C','o','n','t','e','n','t','-','T','y','p','e',':',' ','a','p','p','l','i','c','a','t','i','o','n','/','x','-','w','w','w','-','f','o','r','m','-','u','r','l','e','n','c','o','d','e','d','\r','\n',
		'C','o','n','t','e','n','t','-','L','e','n','g','t','h',':',' ','2','7','\r','\n',
		'\r','\n',
		'n','a','m','e','=','J','o','h','n','+','D','o','e','&','a','g','e','=','3','0'
	};
	
	
	const char raw_str5[] = {
	'D','E','L','E','T','E',' ','/','r','e','s','o','u','r','c','e','/','1','2','3',' ','H','T','T','P','/','1','.','1','\r','\n',
	'H','o','s','t',':',' ','a','p','i','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
	'\r','\n'
	};
	
	const char raw_str6[] = {
		'G','E','T',' ','/','h','o','m','e','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str7[] = {
		'P','U','T',' ','/','u','p','d','a','t','e',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str8[] = {
		'G','E','T',' ','/','i','n','d','e','x','.','h','t','m','l',' ','H','T','T','P','/','1','.','1',' ','E','X','T','R','A','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str9[] = {
		'G','E','T',' ','/','s','e','a','r','c','h','?','q','=','c','h','a','t','g','p','t','&','l','a','n','g','=','e','n',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str10[] = {
		'P','O','S','T',' ','/','s','u','b','m','i','t',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'C','o','n','t','e','n','t','-','T','y','p','e',':',' ','a','p','p','l','i','c','a','t','i','o','n','/','j','s','o','n','\r','\n',
		'\r','\n',
		'{','\"','n','a','m','e','\"',':','\"','J','o','h','n','\"','}'
	};
	
	const char raw_str11[] = {
		'P','O','S','T',' ','/','a','p','i','/','u','s','e','r','s',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','a','p','i','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'C','o','n','t','e','n','t','-','T','y','p','e',':',' ','a','p','p','l','i','c','a','t','i','o','n','/','j','s','o','n','\r','\n',
		'C','o','n','t','e','n','t','-','L','e','n','g','t','h',':',' ','2','0','\r','\n',
		'\r','\n',
		'{','\"','u','s','e','r','n','a','m','e','\"',':','\"','a','l','i','c','e','\"','}'
	};
	
	const char raw_str12[] = {
		'G','E','T',' ','/','a','b','o','u','t',' ','H','T','T','P','/','1','.','1','\r','\n',
		'\r','\n'
	};
	
	const char raw_str13[] = {
		'D','E','L','E','T','E',' ','/','i','t','e','m','s','?','i','d','=','4','2',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','a','p','i','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str14[] = {
		'G','E','T',' ','/','h','o','m','e',' ','H','T','T','P','/','2','.','0','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str15[] = {
		'G','E','T',' ','/',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str16[] = {
		'g','e','t',' ','/','p','a','g','e',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str17[] = {
		'P','O','S','T',' ','/','l','o','g','i','n',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'C','o','n','t','e','n','t','-','T','y','p','e',':',' ','a','p','p','l','i','c','a','t','i','o','n','/','x','-','w','w','w','-','f','o','r','m','-','u','r','l','e','n','c','o','d','e','d','\r','\n',
		'C','o','n','t','e','n','t','-','L','e','n','g','t','h',':',' ','2','9','\r','\n',
		'\r','\n',
		'u','s','e','r','n','a','m','e','=','b','o','b','&','p','a','s','s','w','o','r','d','=','s','e','c','r','e','t'
	};
	
	const char raw_str18[] = {
		'G','E','T',' ',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str19[] = {
		'D','E','L','E','T','E',' ','/','u','s','e','r','s','/','7','8','9',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','a','p','i','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str20[] = {
		'P','O','S','T',' ','/','s','u','b','m','i','t',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'C','o','n','t','e','n','t','-','T','y','p','e',':',' ','t','e','x','t','/','p','l','a','i','n','\r','\n',
		'C','o','n','t','e','n','t','-','L','e','n','g','t','h',':',' ','1','0','\r','\n',
		'\r','\n',
		'H','e','l','l','o','H','e','l','l','o'
	};
	
	const char raw_str21[] = {
		'G','E','T',' ','/','d','a','s','h','b','o','a','r','d',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'X','-','C','u','s','t','o','m','-','H','e','a','d','e','r',':',' ','1','2','3','4','5','\r','\n',
		'\r','\n'
	};
	
	const char raw_str22[] = {
		'G','E','T','/','i','n','d','e','x','.','h','t','m','l',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str23[] = {
		'P','O','S','T',' ','/','u','p','l','o','a','d',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'C','o','n','t','e','n','t','-','T','y','p','e',':',' ','m','u','l','t','i','p','a','r','t','/','f','o','r','m','-','d','a','t','a',';',' ','b','o','u','n','d','a','r','y','=','-','-','-','-','W','e','b','K','i','t','F','o','r','m','B','o','u','n','d','a','r','y','\r','\n',
		'C','o','n','t','e','n','t','-','L','e','n','g','t','h',':',' ','1','3','8','\r','\n',
		'\r','\n',
		'-','-','-','-','-','-','W','e','b','K','i','t','F','o','r','m','B','o','u','n','d','a','r','y','\r','\n',
		'C','o','n','t','e','n','t','-','D','i','s','p','o','s','i','t','i','o','n',':',' ','f','o','r','m','-','d','a','t','a',';',' ','n','a','m','e','=','\"','f','i','l','e','\"',';',' ','f','i','l','e','n','a','m','e','=','\"','t','e','s','t','.','t','x','t','\"','\r','\n',
		'C','o','n','t','e','n','t','-','T','y','p','e',':',' ','t','e','x','t','/','p','l','a','i','n','\r','\n',
		'\r','\n',
		'H','e','l','l','o',' ','w','o','r','l','d','\r','\n',
		'-','-','-','-','-','-','W','e','b','K','i','t','F','o','r','m','B','o','u','n','d','a','r','y','-','-','\r','\n'
	};
	
	const char raw_str24[] = {
		'D','E','L','E','T','E',' ','/','r','e','s','o','u','r','c','e','/','4','5','6',' ','H','T','T','P','/','1','.','1',' ','e','x','t','r','a','\r','\n',
		'H','o','s','t',':',' ','a','p','i','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str25[] = {
		'G','E','T',' ','/','p','a','g','e','.','h','t','m','l','#','s','e','c','t','i','o','n','1',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str26[] = {
		'P','O','S','T',' ','/','s','u','b','m','i','t',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'C','o','n','t','e','n','t','-','T','y','p','e',':',' ','a','p','p','l','i','c','a','t','i','o','n','/','j','s','o','n','\r','\n',
		'C','o','n','t','e','n','t','-','L','e','n','g','t','h',':',' ','1','8','\r','\n',
		'{','\"','n','a','m','e','\"',':','\"','a','l','i','c','e','\"','}'
	};
	
	const char raw_str27[] = {
		'G','E','T',' ','/','i','n','f','o',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'U','s','e','r','-','A','g','e','n','t',':',' ','C','u','s','t','o','m','C','l','i','e','n','t','/','1','.','0','\r','\n',
		'A','c','c','e','p','t',':',' ','t','e','x','t','/','h','t','m','l','\r','\n',
		'\r','\n'
	};
	
	const char raw_str28[] = {
		'G','E','T',' ','/','f','i','l','e',' ','n','a','m','e','.','t','x','t',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str29[] = {
		'P','O','S','T',' ','/','p','i','n','g',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','w','w','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'C','o','n','t','e','n','t','-','L','e','n','g','t','h',':',' ','0','\r','\n',
		'\r','\n'
	};
	
	const char raw_str30[] = {
		'D','E','L','E','T','E',' ','/','i','t','e','m','/','1','2','3',' ','h','t','t','p','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','a','p','i','.','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	
	const char raw_str31[] = {
		'G','E','T',' ','/','s','t','a','t','u','s',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','[','2','0','0','1',':','d','b','8',':',':','1',']','\r','\n',
		'\r','\n'
	};
	
	const char raw_str32[] = {
		'P','O','S','T',' ','/','d','a','t','a',' ','H','T','T','P','/','1','.','1','\r','\n',
		'C','o','n','t','e','n','t','-','T','y','p','e',':',' ','a','p','p','l','i','c','a','t','i','o','n','/','j','s','o','n','\r','\n',
		'\r','\n',
		'{','\"','d','a','t','a','\"',':','\"','t','e','s','t','\"','}'
	};

	const std::string str3(raw_str3, sizeof(raw_str3));
	const std::string str4(raw_str4, sizeof(raw_str4));
	const std::string str5(raw_str5, sizeof(raw_str5));
	const std::string str6(raw_str6, sizeof(raw_str6));
	const std::string str7(raw_str7, sizeof(raw_str7));
	const std::string str8(raw_str8, sizeof(raw_str8));
	const std::string str9(raw_str9, sizeof(raw_str9));
	const std::string str10(raw_str10, sizeof(raw_str10));
	const std::string str11(raw_str11, sizeof(raw_str11));
	const std::string str12(raw_str12, sizeof(raw_str12));
	const std::string str13(raw_str13, sizeof(raw_str13));
	const std::string str14(raw_str14, sizeof(raw_str14));
	const std::string str15(raw_str15, sizeof(raw_str15));
	const std::string str16(raw_str16, sizeof(raw_str16));
	const std::string str17(raw_str17, sizeof(raw_str17));
	const std::string str18(raw_str18, sizeof(raw_str18));
	const std::string str19(raw_str19, sizeof(raw_str19));
	const std::string str20(raw_str20, sizeof(raw_str20));
	const std::string str21(raw_str21, sizeof(raw_str21));
	const std::string str22(raw_str22, sizeof(raw_str22));
	const std::string str23(raw_str23, sizeof(raw_str23));
	const std::string str24(raw_str24, sizeof(raw_str24));
	const std::string str25(raw_str25, sizeof(raw_str25));
	const std::string str26(raw_str26, sizeof(raw_str26));
	const std::string str27(raw_str27, sizeof(raw_str27));
	const std::string str28(raw_str28, sizeof(raw_str28));
	const std::string str29(raw_str29, sizeof(raw_str29));
	const std::string str30(raw_str30, sizeof(raw_str30));
	const std::string str31(raw_str31, sizeof(raw_str31));
	const std::string str32(raw_str32, sizeof(raw_str32));
	strings.push_back(str3);
	strings.push_back(str4);
	strings.push_back(str5);
	strings.push_back(str6);
	strings.push_back(str7);
	strings.push_back(str8);
	strings.push_back(str9);
	strings.push_back(str10);
	strings.push_back(str11);
	strings.push_back(str12);
	strings.push_back(str13);
	strings.push_back(str14);
	strings.push_back(str15);
	strings.push_back(str16);
	strings.push_back(str17);
	strings.push_back(str18);
	strings.push_back(str19);
	strings.push_back(str20);
	strings.push_back(str21);
	strings.push_back(str22);
	strings.push_back(str23);
	strings.push_back(str24);
	strings.push_back(str25);
	strings.push_back(str26);
	strings.push_back(str27);
	strings.push_back(str28);
	strings.push_back(str29);
	strings.push_back(str30);
	strings.push_back(str31);
	strings.push_back(str32);
	

	for (size_t i = 3; i < strings.size() + 3; i++)
	{	
		std::string filename = "./testRequestFiles/str" + iToStr(i);
		writeStringToFile(strings[i - 3], filename);
	}
}

void	printErrorString(int i){

	const char *error[] = {
		"illegal pipe character",
		"missing authority",
		"port out of range",
		"missing `//`",
		"quote in path",
		"space in host",
		"caret in path",
		"empty fragment",
		"misplaced query/fragment",
		"missing host",
		"pipe again",
		"multiple ports",
		"empty port",
		"illegal in FTP too",
		"missing host",
		"missing scheme",
		"invalid hex in percent",
		"invalid double query",
		"brackets in hostname",
		"curly braces in query",
		"unescaped space",
		"illegal angle brackets",
		"illegal curly braces",
		"illegal path characters",
		"incomplete percent-encoding",
		"invalid percent-encoding",
		"malformed IPv6 address",
		"illegal pipe in path",
		"invalid port (non-numeric)",
		"backslash instead of slash"
	};
	std::cout << "Problem : " << error[i] << "\n" << std::endl;
}
