#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
	if (vec.size() == 0){
		os << "[ empty ]";
		return os;
	}
	os << "[ ";
	for (size_t i = 0; i < vec.size(); i++)
	{
		os << vec[i];
		if (i + 1 < vec.size())
			os << ", ";
	}
	os << " ]";
	return os;
}

#include <map>

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m)
{
	if (m.empty()) {
		os << "[ empty ]";
		return os;
	}

	os << "{ ";
	
	typename std::map<K, V>::const_iterator it = m.begin();
	
	while (it != m.end()){

		os << it->first << " : " << it->second;
		it++;
		if (it != m.end())
			os << ", ";
	}	
	os << " }";

	return os;
}

class PrintUtils
{
	public:

		static const int LABEL_WIDTH;
		static const int INDENT_SIZE;

		static std::string indent(int level);

		static void printHeader(std::ostream& os, int indentLevel, const std::string& title);
		static void printMap(std::ostream& os, int indentLevel, const std::string& label, const std::map<std::string, std::string>& m);

		template<typename T>
		static void printField(std::ostream& os, int indentLevel, const std::string& label, const T& value, bool is_set){
			os
			<< indent(indentLevel) << std::left << std::setw(LABEL_WIDTH)
			<< label << "(set = " << is_set << ")\t" << " : " << value
			<< "\n";
		}
};