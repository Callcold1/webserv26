#include "tests.hpp"

#define TEST_BUFFER_SIZE 30

void writeStringToFile(const std::string& name, const std::string& filename) {
	std::ofstream outfile(filename.c_str(), std::ios::binary);

	if (!outfile.is_open()) {
		std::cerr << "Failed to open file for writing\n";
		return;
	}
	outfile.write(name.data(), name.size());
	outfile.close();
}

void	printEscaped(const std::string& data) {
	for (size_t i = 0; i < data.size(); i++) {
		unsigned char c = data[i];
    	switch (c) {
    	    case '\r': std::cout <<  "\\r"; break;
    	    case '\n': std::cout <<  "\\n"; break;
    	    case '\'': std::cout << "\'"; break;
    	    case '\\': std::cout << "\\"; break;
    	    default: {
    	        if (std::isprint(static_cast<unsigned char>(c))) {
    	           std::cout << c;
    	        } else {
    	            std::ostringstream oss;
    	            oss << "'\\x" << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(static_cast<unsigned char>(c))) << "'";
    	            std::cout << oss.str();
    	        }
				break;
    		}
		}
	}
	std::cout << std::endl;
}

void	printHeaders(fieldMap_t headers){
	for (fieldMap_t::iterator it = headers.begin(); it != headers.end(); it++) {
		std::string headerName = it->first;
		std::vector<field_t*>& fieldList = it->second;

		for (size_t i = 0; i < fieldList.size(); i++) {
			field_t* field = fieldList[i];
			std::cout << std::left << std::setw(20) << "Header " << std::setw(3) << " : " << headerName << "\n";
			std::cout << std::left << std::setw(20) << "  Value " << std::setw(3) << " : " << field->val << "\n";

			for (size_t j = 0; j < field->params.size(); j++) {
				std::cout << std::left << std::setw(20) << "    Param " << std::setw(3) << " : " << field->params[j].first
						  << " = " << field->params[j].second << "\n";
			}
		}
		std::cout << std::endl;
	}
}

void	printRequest(Request* request){

	std::string status;
	switch(request->getParseStatus()){
		case 0	:{status = "P_PARSE"; break;}
		case 1	:{status = "P_AWAIT"; break;}
		case 2	:{status = "P DONE"; break;}
		case 3	:{status = "P ERROR"; break;}
		default	:{status = "UNKNOWN"; break;}
	}
	std::cout << std::left << std::setw(20) << "Status"  << std::setw(3) << " : " << status << std::endl;
	std::cout << std::left << std::setw(20) << "Method" << std::setw(3) << " : " << request->_method << std::endl;
	std::cout << std::left << std::setw(20) << "URI string" << std::setw(3) << " : " << request->_uri_str << std::endl;
	std::cout << std::left << std::setw(20) << "Version" << std::setw(3) << " : " << request->_version <<  std::endl;
	printHeaders(request->_headers);
	std::cout << std::left << std::setw(20) << "Body" << std::setw(3) << " : ";
	printEscaped(request->_body);
	std::cout << std::left << std::setw(20) << "\nErrMess" << std::setw(3) << "  : " << request->_errMess << std::endl;
	std::cout << std::left << std::setw(20) << "ErrNum" << std::setw(3) << " : " << request->_errNum << std::endl << std::endl;
	std::cout << std::left << std::setw(20) << "Unparsed Length" << std::setw(3) << " : " << request->_unparsed_length << std::endl;
	std::cout << std::left << std::setw(20) << "Unparsed Data" << std::setw(3) << " : ";
	printEscaped(request->getUnparsedStr());
	std::cout << std::endl;
	return;
}

void	printMessage(int fd, std::vector<std::string>& strings, Request* request){
	printErrorString(fd - 3);
	std::cout << "\nTest String  : ";
	printEscaped(strings[fd - 3]);
	std::cout << std::endl;
	printRequest(request);
}

void	populateMap(std::map<int, Request* >& map, Config* config, size_t size){
	for (size_t i = 3; i < size + 3; i++){
		std::string filename = "./testRequestFiles/str" + iToStr(i);
		int fd = open(filename.c_str(), O_RDONLY);
		map[fd] = new Request(config);
	}
}

void	server(Config* config, std::vector<std::string>& strings) {
	
	size_t complete = 0;
	std::map<int, Request* > map;
	std::map<int, Request* >::iterator it;
	populateMap(map, config, strings.size());

	while (map.size() > complete) {
		for (it = map.begin(); it != map.end(); it++){
			if (complete > 0 && (!(it->second->getParseStatus() == P_AWAIT || it->second->getParseStatus() == P_PARSE))){
				continue;
			}
			char	buffer[TEST_BUFFER_SIZE]; //Random buffer size chosen so that multiple read
			ssize_t	read_len;
			
			memset(buffer, 0, TEST_BUFFER_SIZE);
			read_len = read(it->first, buffer, TEST_BUFFER_SIZE);
			
			if (read_len < 0 || it->second->getParseStatus() == P_DONE ) {
				it->second->setState(P_ERROR, "Read Size of Zero : Incomplete message", 400);
				complete++;
				continue;
			}
			else if (read_len == 0) {
				it->second->setState(P_ERROR, "Read Size of Zero : Incomplete message", 400);
				complete++;
				continue;
			}
			else {
				it->second->parseRequest(buffer, read_len);
				switch(it->second->getParseStatus()){
					case P_AWAIT			: { continue; }
					case P_DONE				: { complete++; continue; }
					case P_ERROR			: { complete++; continue;  }
					default					: continue;
				}
			}
		}
	}
	std::vector<testProfile *> profiles;
	testProfileCreation(profiles);
	for (it = map.begin(); it != map.end(); it++){
		std::cout <<  "\nfd  = " << it->first << "\n========" << std::endl;
		printMessage(it->first, strings, it->second);
		DEBUG2(profiles[it->first - 3]->run(it->second));
	}
}

int main(int argc, char** argv) {

	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}
	ConfigParser cp(argv[1]);
	Config* config = cp.parseServerblocks();

	std::vector<std::string> strings;
	populateTestStringsVector(strings);
	server(config, strings);

	return 0;
}