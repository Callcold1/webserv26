#include "tests.hpp"

void	populateTestStringsVector(std::vector<std::string>& strings){

	const char raw_str3[] = {
		'G','E','T',' ','/','h','e','l','l','o','%','2','0','w','o','r','l','d',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
	};

	const std::string str3(raw_str3, sizeof(raw_str3));
	strings.push_back(str3);

	const char raw_str4[] = {
		'G','E','T',' ','/','f','o','l','d','e','r','%','2','F','f','i','l','e',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str4(raw_str4, sizeof(raw_str4));
	strings.push_back(str4);

	const char raw_str5[] = {
		'G','E','T',' ','/','s','e','a','r','c','h','?','q','=','o','p','e','n','%','2','0','a','i',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','e','n','g','i','n','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str5(raw_str5, sizeof(raw_str5));
	strings.push_back(str5);

	const char raw_str6[] = {
		'G','E','T',' ','/','p','e','r','c','e','n','t','?','v','a','l','u','e','=','1','0','0','%','2','5',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','p','e','r','c','e','n','t','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str6(raw_str6, sizeof(raw_str6));
	strings.push_back(str6);

	const char raw_str7[] = {
		'G','E','T',' ','/','%','E','2','%','9','C','%','9','3',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','u','n','i','c','o','d','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str7(raw_str7, sizeof(raw_str7));
	strings.push_back(str7);

	const char raw_str8[] = {
		'G','E','T',' ','/','f','o','o','%','3','A','b','a','r',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str8(raw_str8, sizeof(raw_str8));
	strings.push_back(str8);

	const char raw_str9[] = {
		'G','E','T',' ','/','f','i','l','e','%','3','B','v','e','r','s','i','o','n','=','1',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str9(raw_str9, sizeof(raw_str9));
	strings.push_back(str9);

	const char raw_str10[] = {
		'G','E','T',' ','/','c','o','n','f','i','g','?','s','e','t','%','3','D','v','a','l','u','e',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str10(raw_str10, sizeof(raw_str10));
	strings.push_back(str10);

	const char raw_str11[] = {
		'G','E','T',' ','/','h','o','m','e',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'U','s','e','r','-','A','g','e','n','t',':',' ','M','y','A','g','e','n','t','%','2','F','1','.','0','\r','\n',
		'\r','\n'
	};
	const std::string str11(raw_str11, sizeof(raw_str11));
	strings.push_back(str11);

	const char raw_str12[] = {
		'G','E','T',' ','/','a','p','i','?','k','e','y','=','1','2','3','%','2','6','m','o','d','e','=','e','d','i','t',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','a','p','i','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str12(raw_str12, sizeof(raw_str12));
	strings.push_back(str12);

	const char raw_str13[] = {
		'G','E','T',' ','/','a','b','c','%','2','f','D','e','f',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','s','i','t','e','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str13(raw_str13, sizeof(raw_str13));
	strings.push_back(str13);

	const char raw_str14[] = {
		'G','E','T',' ','/','u','s','e','r','%','4','0','m','a','i','l','.','c','o','m',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','m','a','i','l','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str14(raw_str14, sizeof(raw_str14));
	strings.push_back(str14);

	const char raw_str15[] = {
		'G','E','T',' ','/','a','p','i','%','3','F','f','a','k','e','=','1',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','a','p','i','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str15(raw_str15, sizeof(raw_str15));
	strings.push_back(str15);

	const char raw_str16[] = {
		'G','E','T',' ','/','%','F','0','%','9','F','%','9','8','%','8','1',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','e','m','o','j','i','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str16(raw_str16, sizeof(raw_str16));
	strings.push_back(str16);


	const char raw_str17[] = {
		'G','E','T',' ','/',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','s','i','t','e','.','c','o','m','\r','\n',
		'A','c','c','e','p','t','-','L','a','n','g','u','a','g','e',':',' ','e','n','%','2','C','u','s','\r','\n',
		'\r','\n'
	};
	const std::string str17(raw_str17, sizeof(raw_str17));
	strings.push_back(str17);

	const char raw_str18[] = {
		'G','E','T',' ','/','i','n','v','a','l','i','d','%','G','1',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str18(raw_str18, sizeof(raw_str18));
	strings.push_back(str18);

	const char raw_str19[] = {
		'G','E','T',' ','/','t','e','s','t','%',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str19(raw_str19, sizeof(raw_str19));
	strings.push_back(str19);

	const char raw_str20[] = {
		'G','E','T',' ','/','p','a','t','h','%','2',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str20(raw_str20, sizeof(raw_str20));
	strings.push_back(str20);

	const char raw_str21[] = {
		'G','E','T',' ','/',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','%','7','3','t',':',' ','b','a','d','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str21(raw_str21, sizeof(raw_str21));
	strings.push_back(str21);

	const char raw_str22[] = {
		'G','E','T',' ','/','%','E','2','%','2','8','%','A','1',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','b','r','o','k','e','n','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str22(raw_str22, sizeof(raw_str22));
	strings.push_back(str22);

	const char raw_str23[] = {
		'G','E','T',' ','/','m','i','x','%','2','0','e','d','%','2','Z','d','a','t','a',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','m','i','x','e','d','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str23(raw_str23, sizeof(raw_str23));
	strings.push_back(str23);

	const char raw_str24[] = {
		'G','E','T',' ','/',' ','H','T','T','P','/','1','.','1','\r','\n',
		'U','s','e','r','-','A','g','e','n','t',':',' ','T','e','s','t','%','z','z','\r','\n',
		'H','o','s','t',':',' ','t','e','s','t','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str24(raw_str24, sizeof(raw_str24));
	strings.push_back(str24);

	const char raw_str25[] = {
		'G','E','T',' ','/','b','a','d','%','0','\r','\n',
		'%','0','A',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','s','p','l','i','t','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str25(raw_str25, sizeof(raw_str25));
	strings.push_back(str25);

	const char raw_str26[] = {
		'G','E','T',' ','/','w','e','i','r','d','%','%','2','0','p','a','t','h',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','w','e','i','r','d','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str26(raw_str26, sizeof(raw_str26));
	strings.push_back(str26);

	const char raw_str27[] = {
		'G','E','T',' ','/',' ','H','T','T','P','/','1','.','1','\r','\n',
		'X','-','H','e','a','d','e','r',':',' ','t','e','s','t','%','0','D','%','0','A','M','a','l','i','c','i','o','u','s',':',' ','i','n','j','e','c','t','e','d','\r','\n',
		'H','o','s','t',':',' ','e','v','i','l','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str27(raw_str27, sizeof(raw_str27));
	strings.push_back(str27);

	const char raw_str28[] = {
		'G','E','T',' ','/','s','o','m','e','%','2','0','t','h','i','n','g','%',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','e','r','r','o','r','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str28(raw_str28, sizeof(raw_str28));
	strings.push_back(str28);

	const char raw_str29[] = {
		'G','E','T',' ','/','a','p','i','?','%','b','a','d','=','v','a','l','u','e',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','a','p','i','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str29(raw_str29, sizeof(raw_str29));
		strings.push_back(str29);

	const char raw_str30[] = {
		'G','E','T',' ','/','%','F','8','%','8','8','%','8','0','%','8','0',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','u','t','f','8','f','a','i','l','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str30(raw_str30, sizeof(raw_str30));
		strings.push_back(str30);

	const char raw_str31[] = {
		'G','E','T',' ','/','f','i','l','e','%','0','0','n','a','m','e',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','n','u','l','l','.','c','o','m','\r','\n',
		'\r','\n'
	};
	const std::string str31(raw_str31, sizeof(raw_str31));
		strings.push_back(str31);

	const char raw_str32[] = {
		'G','E','T',' ','/','p','a','t','h','?','q','u','e','r','y','=','%','3','G',' ','H','T','T','P','/','1','.','1','\r','\n',
		'H','o','s','t',':',' ','e','x','a','m','p','l','e','.','c','o','m','\r','\n',
		'\r','\n'
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

	};
	std::cout << "Expected Result : " << error[i] << "\n" << std::endl;
}