#include <string>
#include "Request.hpp"
#include "Client.hpp"
#include "CgiResponse.hpp"
#include "ErrorResponse.hpp"
#include "StaticResponse.hpp"
#include "RediResponse.hpp"

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
>>>>>>		CONSTRUCTORS		<<<<<<
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/

Client::Client(listenInstance_s* listen_instance, int fd) :
	_listen_instance(listen_instance),
	_fd(fd),
	_matched_server(NULL),
	_matched_location(NULL),
	_response(NULL),
	_conn (CONN_KEEP_ALIVE),
	_state(CONNECTED),
	_has_output(false),
	_log_msg("fd = " + toString(fd) + ", request status = ")
{
	time(&_last_active);
	Log::instance().log(Log::INFO, Log::TYPE_REQUEST_PARSING,
			"Request : " + _log_msg + "instantiated" , false);
	_request = Request(_listen_instance);
}

Client::Client(const Client &other) :
	_listen_instance(other._listen_instance),
	_fd(other._fd),
	_matched_server(other._matched_server),
	_matched_location(other._matched_location),
	_request(other._request),
	_response(other._response),
	_conn(other._conn),
	_state(other._state),
	_has_output(other._has_output),
	_last_active(other._last_active),
	_log_msg(other._log_msg),
	_out_buff(other._out_buff)
{}

Client&	Client::operator=(const Client& other){

	if (this != &other)
	{
		_matched_server = other._matched_server;
		_matched_location = other._matched_location;
		_request = other._request;
		_response = other._response;
		_conn = other._conn;
		_state = other._state;
		_has_output = other._has_output;
		_last_active = other._last_active;
		_log_msg = other._log_msg;
		_out_buff = other._out_buff;
	}
	return *this;
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		DESTRUCTORS		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

Client::~Client(){}

void	Client::clear_out_buff(){
	_out_buff = "";
	_has_output = false;
}

/*
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		STATE CHANGE		<<<<<<
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/

void change_state(enum cState state);

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		GETTERS		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

eConn	Client::getConn(){
	return _conn;
}

int	Client::getFD(){
	return _fd;
}

std::string&	Client::get_msg(){
	return(_out_buff);
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		HANDLERS		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

bool	Client::handleClientRead(const char *buffer, size_t len){

	enum eParse	p_state;

	if (!buffer)
		return false;
	try {
		if (_request.getParseStatus() == P_ERROR)
			return false;
		p_state = _request.parseRequest(buffer, len);
		if (!_matched_server){
			_matched_server = _request.getMatchedServer();
			_matched_location = _request.getMatchedLocation();
		}
	}
	catch(const std::exception& e){
		std::string err = e.what();
		throw std::runtime_error("handleClientRead() : " + err);
	}

	switch(p_state){
		case P_PARSE	: 	{ _state = READ; generateLogMessage(); return false; }
		case P_AWAIT	: 	{ _state = READ; generateLogMessage(); return false;}
		case P_DONE		:	{ _state = WRITE; generateLogMessage();	break;}
		case P_ERROR	: 	{ _state = ERROR;_request._re.type = RESPOND_ERROR; _conn = CONN_CLOSE; generateLogMessage(); break;}
		default	: break;
	}
	if (_state == WRITE || _state == ERROR){
	 	handle_request();
		return true;
	}
	return false;
}

int Client::if_file_upload(Request &request)
{
	if (request._method == "POST")
	{
		std::vector<field_t* > header = request.findHeader("content-type");
		if (header.empty())
		{
			return (1);
		}
		else if (header[0]->val.find("multipart/form-data") != std::string::npos)
		{
			_response =  new RediResponse(FILE_UPLOAD_REDIREC, _request, true);
			return(_response->generate());
		}
		else if (header[0]->val.find("image/png") != std::string::npos)
		{
			_response =  new RediResponse(FILE_UPLOAD_REDIREC, _request, false);
			return(_response->generate());
		}
	}
	return (1);
}

void Client::handle_request()
{
	int			error_catch = 0;
	respond_t&	re = _request._re;

	switch(_request._re.type) {
		// case(RESPOND_AUTOIND):{ _response = new StaticResponse(_request); break;}
		case(RESPOND_CGI)    :{	_response = new CgiResponse(_request); break;}
		//case(RESPOND_DELETE) :{ _response = new DeleteResponse(_request); break;}
		case(RESPOND_ERROR)  :{ _response = new ErrorResponse(re.status, _request);	break;}
		case(RESPOND_INDEX)  :{ _response = new StaticResponse(_request); break;}
		case(RESPOND_NOT_SET):{ _response = new ErrorResponse(NOT_FOUND_ERROR, _request); break;}
		case(RESPOND_MP_UP)	 :{ _response = new RediResponse(FILE_UPLOAD_REDIREC, _request, true); break;}
		case(RESPOND_REDIR)  :{ _response = new RediResponse(_request, re.paths.redirect_url); break;}
		case(RESPOND_STATIC) :{ _response = new StaticResponse(_request); break;}
		case(RESPOND_UPLOAD) :{ _response = new RediResponse(FILE_UPLOAD_REDIREC, _request, false);	break;}
		default				 :{ _response = new ErrorResponse(NOT_FOUND_ERROR, _request); break;}
	}
	if (re.type != RESPOND_ERROR)
		error_catch = _response->generate();
	if (error_catch > 0){
		re.status = error_catch;
		delete _response;
		_response = new ErrorResponse(re.status, _request);
	}
	
	//else if (!_matched_location->getRedirect().url.empty())
	//{
	//	std::cout << "redirect found" << std::endl;
	//	_response = new RediResponse(_request, _matched_location->getRedirect().url); //TODO is this url validated??
	//	_response->generate();
	//}

	// else if ((error_catch = if_file_upload(_request)) != 1)
	// {
	// 	//std::cout << "file upload found" << std::endl;
	// 	if (error_catch >= 400)
	// 		_response = new ErrorResponse(error_catch, _request);
	// }

	// else if (path.find(CGI_PATH) != std::string::npos && path.find((CGI_PATH + std::string("assets"))) == std::string::npos)
	// {
	// 	//std::cout << "cgi found" << std::endl;
	// 	struct stat st;
	// 	if (stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
	// 	{
	// 		_response =  new CgiResponse(_request);
	// 		error_catch = _response->generate();
	// 		if (error_catch != 0)
	// 		{
	// 			delete _response;
	// 			_response = new ErrorResponse(error_catch, _request);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		_response = new ErrorResponse(NOT_FOUND_ERROR, _request); //again, could be 403 for directories etc but this is for security
	// 	}
	// }
	// else if (path.find(HTML_PATH) != std::string::npos || path.find(PHOTO_PATH) != std::string::npos)
	// {
	// 	//std::cout << "static page found" << std::endl;
	// 	_response = new StaticResponse(_request);
	// 	if ((error_catch = _response->generate()) != 0)
	// 	{
	// 		delete _response;
	// 		_response = new ErrorResponse(error_catch, _request);
	// 	}
	// }
	
	append_write(_response->serialize());
	delete _response;
	if (re.keep_alive == true && _request._p_state == P_DONE){
		_request.clear();
	}
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		WRITING		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

void Client::append_write(std::string str){

	_out_buff.append(str);
	_has_output = true;
}

void	Client::generateLogMessage(void){

	std::string status;

	switch(_request.getParseStatus()){
		case 0	:{status = "P_PARSE"; break;}
		case 1	:{status = "P_AWAIT"; break;}
		case 2	:{status = "P DONE"; break;}
		case 3	:{status = "P ERROR : " + _request.getErrMsg(); break;}
		default	:{status = "UNKNOWN"; break;}
	}
	std::cout << _request._re << std::endl;
	std::string message = _log_msg + "Parsing : " + status;
	Log::instance().log(Log::INFO, Log::TYPE_REQUEST_PARSING, message, false);
	Log::instance().log(Log::INFO, Log::TYPE_REQUEST_PARSING, printstream(_request), false);
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

std::ostream& operator<<(std::ostream& os, Request& _request){

	std::string status;

	switch(_request.getParseStatus()){
		case 0	:{status = "P_PARSE"; break;}
		case 1	:{status = "P_AWAIT"; break;}
		case 2	:{status = "P DONE"; break;}
		case 3	:{status = "P ERROR"; break;}
		default	:{status = "UNKNOWN"; break;}
	}

	os << std::left << std::setw(20) << "Status"  << std::setw(3) << " : " << status << std::endl;
	os << std::left << std::setw(20) << "Method" << std::setw(3) << " : " << _request._method << std::endl;
	os << std::left << std::setw(20) << "URI" << std::setw(3) << " : " << _request._uri_str << std::endl;
	os << std::left << std::setw(20) << "URI Scheme" << std::setw(3) << " : " << _request._uri.scheme << std::endl;
	os << std::left << std::setw(20) << "URI Auth" << std::setw(3) << " : " << _request._uri.auth << std::endl;
	os << std::left << std::setw(20) << "URI Port" << std::setw(3) << " : " << _request._uri.port << std::endl;
	os << std::left << std::setw(20) << "URI Path" << std::setw(3) << " : " << _request._uri.path << std::endl;
	os << std::left << std::setw(20) << "URI Frag" << std::setw(3) << " : " << _request._uri.frag << std::endl;
	//os << std::left << std::setw(20) << "URI Query" << std::setw(3) << " : " << _request._uri.queries << std::endl;
	os << std::left << std::setw(20) << "Version" << std::setw(3) << " : " << _request._version <<  std::endl;
	os << std::left << std::setw(20) << "Body" << std::setw(3) << " : " << _request._body << std::endl;
	os << std::left << std::setw(20) << "ErrMess" << std::setw(3) << " : " << _request._errMess << std::endl;
	os << std::left << std::setw(20) << "ErrNum" << std::setw(3) << " : " << _request._errNum << std::endl << std::endl;
	os << _request._headers;

	if (_request._matched_server){
		os << std::left << std::setw(20) << "Matched Server" << std::setw(3) << " : " << _request._matched_server << std::endl;
		os << std::left << std::setw(20) << "Matched Server Name" << std::setw(3) << " : " << _request._matched_server->getServerName() << std::endl;
	}
	if (_request._matched_location){
		os << std::left << std::setw(20) << "Matched Location" << std::setw(3) << " : " << &_request._matched_location << std::endl;
		os << std::left << std::setw(20) << "Matched Location URI" << std::setw(3) << " : " << _request._matched_location->getURI() << std::endl << std::endl;
		os << std::left << std::setw(20) << "Matched Location root" << std::setw(3) << " : " << _request._matched_location->getRootDir() << std::endl << std::endl;
	}
	return os;
}

/*
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
>>>>>>		PRINTING REQUEST->_RE	<<<<<<
>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<
*/

std::ostream& operator<<(std::ostream& os, const std::vector<std::pair <fileInfo_s, std::string> >& upload_paths){
	for (size_t i = 0; i < upload_paths.size(); i++){
		os << "\nupload_path[" << i << "]         : path         : " << upload_paths[i].first.path << std::endl; // to consider : do we allow overwriting files? What about delete? fileInfo will tell us if file already exists
		os << "upload_path[" << i << "]         : tmpfile path : " << upload_paths[i].second << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const reqPaths_s& paths){
	os << "error path             : " << paths.error_path.path << std::endl;
	os << "redirect url           : " << paths.redirect_url << std::endl;
	os << "resource path          : " << paths.resource_path.path << std::endl;
	os << "upload_base            : " << paths.upload_base << std::endl;
	os << paths.upload_paths;
	return os;
}

std::ostream& operator<<(std::ostream& os, const respond_t& re){
	
	os << "Response Type          : ";
	switch(re.type){
		case(RESPOND_NOT_SET)	:{std::cout << "RESPOND_NOT_SET" << std::endl; break;}
		case(RESPOND_CGI)		:{std::cout << "RESPOND_CGI" << std::endl; break;}
		case(RESPOND_ERROR)		:{std::cout << "RESPOND_ERROR" << std::endl; break;}
		case(RESPOND_FILE)		:{std::cout << "RESPOND_FILE" << std::endl; break;}
		case(RESPOND_INDEX)		:{std::cout << "RESPOND_INDEX" << std::endl; break;}
		case(RESPOND_REDIR)		:{std::cout << "RESPOND_REDIR" << std::endl; break;}
		case(RESPOND_UPLOAD)	:{std::cout << "RESPOND_UPLOAD" << std::endl; break;}
		default : break;
	}
	os << "Status                 : " << re.status << std::endl;
	os << "Message                : " << re.message << std::endl;
	os << re.paths;
	os << "uri_path               : " << re.uri_path << std::endl;
	os << "keep-alive             : " << re.keep_alive << std::endl;
	os << "Accept content-type(s) : " << re.accept_type << std::endl;
	return os;
}
