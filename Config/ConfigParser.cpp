#include "./includes/ConfigParser.hpp"

ConfigParser::ConfigParser( const std::string& filename ) : _filename(filename){}

ConfigParser::~ConfigParser( void ){}

void ConfigParser::parseLocation(Config& config, std::ifstream& file, std::string& oldLine)
{
	std::string line;
	Route* currentRoute = new Route();

	currentRoute->setPath(parseLocationPath(oldLine));
	while (std::getline(file, line))
	{
		line = trim(line);
		if (line.find("allow_methods") == 0)
		{
			std::vector<std::string> methods = parseMethods(line);
			if (currentRoute)
				currentRoute->setAllowedMethods(methods);
			else
				currentRoute->setAllowedMethods(methods);  // Access via pointer
		}
		else if (line.find("autoindex") == 0)
			currentRoute->setAutoindex(parseValue<std::string>(line));
		else if (line.find("return") == 0) {
			std::istringstream iss(line);
			std::string key;
			int status;
			std::string url;
			iss >> key >> status >> url;
			currentRoute->setRedirectStatus(status);
			currentRoute->setRedirectUrl(url);
		}
		else if (line.find("root") == 0 && currentRoute)
			currentRoute->setRootDirRoute(parseValue<std::string>(line));
		else if (line.find("index") == 0 && currentRoute)
			currentRoute->setIndexFile(parseValue<std::string>(line));
		else if (line == "}")
			break;
	}
	if (currentRoute){
		currentRoute->setRouteTarget();
		config.addRoute(currentRoute);
	}
}

Config* ConfigParser::parseServerblocks()
{
	std::ifstream file(_filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Could not open configuration file.");
	Config* config = new Config[3];  // Use a pointer to Config
	std::string line;
	Route *currentRoute = NULL;
		
	for (int idx = 0; idx < 3; ++idx) {  // Loop over the first two config objects
		while (std::getline(file, line)) {
			line = trim(line);
			if (line.empty() || line[0] == '#')
				continue;
			if (line == "server {")
				continue;
			else if (line == "}")
				break;
			else if (line.find("listen") == 0)
				config[idx].setPort(parseValue<int>(line));  // Access via pointer
			else if (line.find("server_name") == 0)
				config[idx].setName(parseValue<std::string>(line));  // Access via pointer
			else if (line.find("root") == 0 && !currentRoute)
				config[idx].setRootDirConfig(parseValue<std::string>(line));  // Access via pointer
			else if (line.find("client_max_body_size") == 0)
				config[idx].setMaxBodySize(parseValue<int>(line));  // Access via pointer
			else if (line.find("index") == 0 && !currentRoute)
				config[idx].setDefaultFile(parseValue<std::string>(line));  // Access via pointer
			else if (line.find("error_page") == 0)
			{
				std::istringstream iss(line);
				std::string key;
				int error_code;
				std::string error_page;
				iss >> key >> error_code >> error_page;
				config[idx].setErrorPage(error_code, error_page);  // Access via pointer
			}
			else if (line.find("allow_methods") == 0)
			{
				std::vector<std::string> methods = parseMethods(line);
				if (currentRoute)
					currentRoute->setAllowedMethods(methods);
				else
					config[idx].setAllowedMethods(methods);  // Access via pointer
			} else if (line.find("location") == 0)
				parseLocation(config[idx], file, line);  // Pass pointer to parseLocation
		}
		if (currentRoute)
			config[idx].addRoute(currentRoute);  // Access via pointer
	}	
	file.close();
	return config;  // Dereference pointer to return the Config object
}

std::string ConfigParser::trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t");
	if (first == std::string::npos) return "";
		size_t last = str.find_last_not_of(" \t;");
	return str.substr(first, last - first + 1);
}

template<typename T>
T ConfigParser::parseValue(const std::string &line)
{
	std::istringstream iss(line);
	std::string key;
	T value;
	iss >> key >> value;
	return value;
}

std::vector<std::string> ConfigParser::parseMethods(const std::string &line)
{
	std::istringstream iss(line);
	std::string key, method;
	std::vector<std::string> methods;

	iss >> key;
	while (iss >> method)
		methods.push_back(method);
	return methods;
}

std::string ConfigParser::parseLocationPath(const std::string &line)
{
	size_t start = line.find("location") + 8;
	size_t end = line.find("{");
	return trim(line.substr(start, end - start));
}
