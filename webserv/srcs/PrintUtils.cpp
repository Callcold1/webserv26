#include "PrintUtils.hpp"

const int PrintUtils::LABEL_WIDTH = 30;
const int PrintUtils::INDENT_SIZE = 4;

std::string PrintUtils::indent(int level)
{
    return std::string(level * INDENT_SIZE, ' ');
}

void PrintUtils::printHeader(std::ostream& os, int indentLevel, const std::string& title){
    
    os  
    << "\n"
    << indent(indentLevel)
    << title << "\n"
    << indent(indentLevel)
    << std::string(title.length(), '=')
    << "\n";
}

void PrintUtils::printMap(std::ostream& os, int indentLevel, const std::string& label, const std::map<std::string, std::string>& m){
	
	os << indent(indentLevel) << label << " :\n";

	for (std::map<std::string, std::string>::const_iterator it = m.begin(); it != m.end(); it++){
		os << indent(indentLevel + 1)
		   << it->first << " => " << it->second << "\n";
	}
}
