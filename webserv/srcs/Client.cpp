#include "Client.hpp"

Client::Client(Config* config): _config(config){
	fd = -1;
	out_buff = "";
	_state = CONNECTED;
	time(&last_active);

	has_output = false;

	request = Request(config);
}

Client::Client(Config* config, int new_fd) : _config(config){
	fd = new_fd;
	out_buff = "";
	_state = CONNECTED;
	time(&last_active);

	has_output = false;
	request = Request(config);
}

Client::Client(const Client &other)
	:
	  has_output(other.has_output),
	  fd(other.fd),
	  out_buff(other.out_buff),
	  _state(other._state),
	  last_active(other.last_active)
{}

Client&	Client::operator=(const Client& other){

	if (this != &other)
	{
		fd = other.fd;
		out_buff = other.out_buff;
		_state = other._state;
		last_active = other.last_active;
		has_output = other.has_output;
	}
	return *this;
}

Client::~Client(){}

void change_state(enum cState state);

void Client::append_write(std::string str)
{
	out_buff.append(str);
	has_output = true;
}

std::string Client::get_msg()
{
	return(out_buff);
}

void Client::clear_out_buff()
{
	out_buff = "";
	has_output = false;
}

bool	Client::handleClientRead(const char *buffer, size_t len){

	if (!buffer)
		return false;

	// if (!_request_ptr)
	// 	_request_ptr = new Request;

	_p_state = request.parseRequest(buffer, len);
	// _p_state = _request_ptr->parseRequest(buffer, len);
	
	
	switch(_p_state){
		case P_PARSE	: 	{
								Log::instance().log(Log::INFO, Log::TYPE_REQUEST_PARSING, "Request Parsing : status = P_PARSE " + printstream(request));
								_state = READ;
								return false;
							}	
		case P_AWAIT	: 	{
								Log::instance().log(Log::INFO, Log::TYPE_REQUEST_PARSING, "Request Parsing : status = P_AWAIT " + printstream(request));
								_state = READ;
								return false; 
							}		
		case P_DONE		:	{	
								Log::instance().log(Log::INFO, Log::TYPE_REQUEST_PARSING, "Request Parsing : status = P_DONE " + printstream(request)); 
								_state = WRITE;
								//delete _request_ptr;
								break;
		}
		case P_ERROR	: 	{
								//delete _request_ptr;
								_state = ERROR;
								return true;
							}
		default	: break;
	}
	return true;
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
>>>>>>		PRINTING REQUEST	<<<<<<
>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<
*/

std::ostream&	operator<<(std::ostream& os, const fieldMap_t headers){
	for (fieldMap_t::const_iterator it = headers.begin(); it != headers.end(); it++) {
		std::string headerName = it->first;
		const std::vector<field_t*>& fieldList = it->second;

		for (size_t i = 0; i < fieldList.size(); i++) {
			field_t* field = fieldList[i];
			os << std::left << std::setw(20) << "Header " << std::setw(3) << " : " << headerName << "\n";
			os << std::left << std::setw(20) << "  Value " << std::setw(3) << " : " << field->val << "\n";

			for (size_t j = 0; j < field->params.size(); j++) {
				os << std::left << std::setw(20) << "    Param " << std::setw(3) << " : " << field->params[j].first
						  << " = " << field->params[j].second << "\n";
			}
		}
		os << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, Request& request){

	std::string status;
	
	switch(request.getParseStatus()){
		case 0	:{status = "P_PARSE"; break;}
		case 1	:{status = "P_AWAIT"; break;}
		case 2	:{status = "P DONE"; break;}
		case 3	:{status = "P ERROR"; break;}
		default	:{status = "UNKNOWN"; break;}
	}

	os << std::left << std::setw(20) << "Status"  << std::setw(3) << " : " << status << std::endl;
	os << std::left << std::setw(20) << "Method" << std::setw(3) << " : " << request._method << std::endl;
	os << std::left << std::setw(20) << "URI" << std::setw(3) << " : " << request._uri_str << std::endl;
	os << std::left << std::setw(20) << "Version" << std::setw(3) << " : " << request._version <<  std::endl;
	os << std::left << std::setw(20) << "Body" << std::setw(3) << " : " << request._body << std::endl;
	os << std::left << std::setw(20) << "ErrMess" << std::setw(3) << " : " << request._errMess << std::endl;
	os << std::left << std::setw(20) << "ErrNum" << std::setw(3) << " : " << request._errNum << std::endl << std::endl;
	os << request._headers;
	return os;
	
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
>>>>>>		PRINTING RESPONSE	<<<<<<
>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<
*/