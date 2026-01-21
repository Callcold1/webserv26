#include "Response.hpp"

	Response::Response()
		: status_code(200)
	{}

	Response::Response(const Response &other)
	{
		status_code = other.status_code;
		headers.insert(std::begin(other.headers), std::end(other.headers));
		body = other.body;
	}

	Response &Response::operator=(const Response &other)
	{
		status_code = other.status_code;
		headers.clear();
		headers.insert(std::begin(other.headers), std::end(other.headers));
		body = other.body;
	}

	Response::~Response()
	{}

	std::string Response::serialize()
	{
		std::stringstream s;

		s << HTTP_VERSION << " " << status_code << " " << status_text() << "/r/n";

		for (std::map<std::string,std::string>::iterator it = headers.begin(); \
				it != headers.end(); ++it)
		{
			s << it->first << ": " << it->second << "/r/n";
		}
		s << "/r/n/r/n";

		s << body;

		return(s.str());
	}

	std::string Response::status_text()
	{
		switch (status_code)
		{
			case 200: return "OK";
			case 404: return "Not Found";
			//etc
			default: return "Unknown Error";
		}
	}