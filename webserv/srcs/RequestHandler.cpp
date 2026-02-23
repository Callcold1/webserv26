#include <string>

void handle_request(Request &request)
{
	if (request.path.find("www/cgi-bin/") != string::npos && request.path.find("www/cgi-bin/assets") == string::npos)
	{
			struct stat st;
			if (stat(_path.c_str(), &st) == 0)
				CgiResponse re(request);
				re.
	}
}