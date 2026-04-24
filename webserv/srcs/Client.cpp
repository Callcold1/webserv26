#include <string>
#include "Request.hpp"
#include "Client.hpp"
#include "CgiResponse.hpp"
#include "ErrorResponse.hpp"
#include "SuccessResponse.hpp"
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
	Log::instance().log(Log::INFO, Log::TYPE_PARSING,
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

Client::~Client(){
	_request.clear();
}

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

int	Client::getServerTimeout()
{
	if (_matched_server && _matched_server->isTimeoutKASet())
	{
		return(_matched_server->getTimeoutKA());
	}
	return (-1);
}

time_t	Client::getLastActivity()
{
	return(_last_active);
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		HANDLERS		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

bool	Client::handleClientRead(const char *buffer, size_t len){

	enum eParse	p_state;

	if (!buffer && _request._sizes.bytes_unread == 0)
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
		case P_PARSE	: 	{ _state = READ; generateLogMessage(); return false;}
		case P_AWAIT	: 	{ _state = READ; if (_request._r_state > V_HEADERS) { generateLogMessage();} return false;}
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

void	Client::handleZeroRead(){
	
	eReqState request_state  = _request.getReqStatus();
	eParse parse_state = _request.getParseStatus();
	eBodyType body_type = _request._body_type;

	if (parse_state == P_AWAIT) {		// KM - Check if this could lead to segfault (e.g. no matched server, or no matched location, or other problems)
		if (request_state < P_BODY){
			_request.setState(P_ERROR, "Malformed Request : Client closed connection before full request received", 400);
			handle_request();
			return;
		}
		else if (request_state == P_BODY) {
			switch (body_type){
				case (BODY_EOF)			: { 
											if (_request._version == "HTTP/1.0")
											_request.setState(P_DONE, "Parsing Complete", _request._errNum);
											handle_request();
											return;
										}
				case (BODY_CHUNKED)		: {
											_request.setState(P_ERROR, "Malformed Request : Client closed connection before full request received", 400);
											handle_request();
											return;
										}
				case (BODY_CONTENT_LEN)	: {
											_request.setState(P_ERROR, "Malformed Request : Client closed connection before full request received", 400);
											handle_request();
											return;
										}
				default	: return;
			}
			return;
		}
	}
	else if (parse_state == P_DONE)
		return;
	else if (parse_state == P_PARSE){
	
		if (_request._sizes.bytes_unread > 0){ // Still unparsed data in _request._buffer
			if (_request._re.keep_alive)	  // Connection is set to keep alive
				handleClientRead(NULL, 0); 	  // Handle new request
			else if (_request._version == "HTTP/1.0")
				return;
		}
	}
	else if (parse_state == P_ERROR)
		return;
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
	Log::instance().log(Log::INFO, Log::TYPE_RESPONSE,	"Client [" + toString(_fd) + "] : response instantiated" , false);
	Log::instance().log(Log::INFO, Log::TYPE_RESPONSE,	"Client [" + toString(_fd) + "] : " + getStatusString(), false);

	int error_catch = 0;
	
	std::string path = _request._re.paths.resource_path.path;

	if (_request._re.type == RESPOND_ERROR)
		_response = new ErrorResponse(_request._errNum, _request);
	
	else if (!_request._re.paths.redirect_url.empty())
	{
		_response = new RediResponse(_request, _request._re.paths.redirect_url);
		_response->generate();
	}

	else if ((error_catch = if_file_upload(_request)) != 1)
	{
		if (error_catch >= 400)
			_response = new ErrorResponse(error_catch, _request);
	}

	else if (_request._isCGI)
	{
		fileInfo_s info;
		getFileInfo(path, info);
		if (info.is_exec || info.is_file)
		{
			if (ends_with(path, ".py"))
				_response =  new CgiResponse(_request, true);
			else if (ends_with(path, ".php"))
				_response =  new CgiResponse(_request, false);
			else
				_response = new ErrorResponse(403, _request);
			error_catch = _response->generate();
			if (error_catch != 0)
			{
				delete _response;
				_response = new ErrorResponse(error_catch, _request);
			}
		}
		else if (info.is_dir || info.is_file || info.inaccessible)
			_response = new ErrorResponse(403, _request);				//permission denied
		else
			_response = new ErrorResponse(NOT_FOUND_ERROR, _request);
	}
	else if (_request._re.type == RESPOND_STATIC) 
	{
		_response = new SuccessResponse(_request);
		if ((error_catch = _response->generate()) != 0)
		{
			delete _response;
			_response = new ErrorResponse(error_catch, _request);
		}
	}
	else
	{
		_response =  new ErrorResponse(NOT_FOUND_ERROR, _request);
	}
	Log::instance().log(Log::INFO, Log::TYPE_RESPONSE,_response->serialize() , false);
	append_write(_response->serialize());
	delete _response;
	if (_request._re.keep_alive == true && _request._p_state == P_DONE){
		_request.clear();
	}
}

/*
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
>>>>>>		WRITING		<<<<<<
>>>>>>>>>>>>>>><<<<<<<<<<<<<<<
*/

std::string	Client::getStatusString(void){
	
	std::string str;

	str += "Parsing State : ";
	
	switch(_request.getParseStatus()){
		case 0	:{str += "P_PARSE"; break;}
		case 1	:{str += "P_AWAIT"; break;}
		case 2	:{str += "P DONE"; break;}
		case 3	:{str += "P ERROR : " + _request.getErrMsg(); break;}
		default	:{str += "UNKNOWN"; break;}
	}
	str += " Request State : ";

	switch(_request.getReqStatus()){
		case 0	:{str += "P_REQLINE"; break;}
		case 1	:{str += "P_REQLINE_PARAMS"; break;}
		case 2	:{str += "V_REQLINE"; break;}
		case 3	:{str += "P_HEADERS"; break;}
		case 4	:{str += "V_HEADERS"; break;}
		case 5	:{str += "P_BODY"; break;}
		case 6	:{str += "V_BODY"; break;}
		case 7	:{str += "P_TRAILERS"; break;}
		case 8	:{str += "V_TRAILERS"; break;}
		case 9	:{str += "P_MULTI"; break;}
		case 10	:{str += "V_MPHEADERS"; break;}
		case 11	:{str += "V_MPBODY"; break;}
		case 12	:{str += "REQ_DONE"; break;}
		default	:{str += "UNKNOWN"; break;}
	}
	
	str += "Initial Response Type : ";

	switch(_request._re.type){
			case(RESPOND_NOT_SET)	:{str += "RESPOND_NOT_SET\n"; break;}
			case(RESPOND_CGI)		:{str += "RESPOND_CGI\n"; break;}
			case(RESPOND_ERROR)		:{str += "RESPOND_ERROR\n"; break;}
			case(RESPOND_FILE)		:{str += "RESPOND_FILE\n"; break;}
			case(RESPOND_INDEX)		:{str += "RESPOND_INDEX\n"; break;}
			case(RESPOND_REDIR)		:{str += "RESPOND_REDIR\n"; break;}
			case(RESPOND_STATIC)	:{str += "RESPOND_STATIC\n"; break;}
			case(RESPOND_UPLOAD)	:{str += "RESPOND_UPLOAD\n"; break;}
			default : break;
	}
	return str;
}

void Client::append_write(std::string str){

	_out_buff.append(str);
	_has_output = true;
}

void	Client::generateLogMessage(void){

	std::string message = _log_msg + getStatusString();
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
		case(RESPOND_STATIC)	:{std::cout << "RESPOND_STATIC" << std::endl; break;}
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

std::string	printReqStatus(Request& _request, int fd){

	std::string p_status;
	std::string r_status;
	std::ostringstream os;

	switch(_request.getParseStatus()){
		case 0	:{p_status = "P_PARSE"; break;}
		case 1	:{p_status = "P_AWAIT"; break;}
		case 2	:{p_status = "P DONE"; break;}
		case 3	:{p_status = "P ERROR"; break;}
		default	:{p_status = "UNKNOWN"; break;}
	}
	switch(_request.getReqStatus()){
		case 0	:{r_status = "P_REQLINE"; break;}
		case 1	:{r_status = "P_REQLINE_PARAMS"; break;}
		case 2	:{r_status = "V_REQLINE"; break;}
		case 3	:{r_status = "P_HEADERS"; break;}
		case 4	:{r_status = "V_HEADERS"; break;}
		case 5	:{r_status = "P_BODY"; break;}
		case 6	:{r_status = "V_BODY"; break;}
		case 7	:{r_status = "P_TRAILERS"; break;}
		case 8	:{r_status = "V_TRAILERS"; break;}
		case 9	:{r_status = "P_MULTI"; break;}
		case 10	:{r_status = "V_MPHEADERS"; break;}
		case 11	:{r_status = "V_MPBODY"; break;}
		case 12	:{r_status = "REQ_DONE"; break;}
		default	:{r_status = "UNKNOWN"; break;}
	}
	os << std::left << std::setw(21) << "\nFile Descriptor" << std::setw(3) << " : " << "[" << fd << "]" << std::endl;
	os << std::left << std::setw(20) << "Parse Status" << std::setw(3) << " : " << "[" << p_status << "]" << std::endl;
	os << std::left << std::setw(20) << "Request Status"  << std::setw(3)<< " : " << "[" << r_status << "]" << std::endl;
	os << std::left << std::setw(20) << "Method" << std::setw(3) << " : " << "[" << _request._method << "]" << std::endl;
	os << std::left << std::setw(20) << "ErrNum" << std::setw(3) << " : " << "[" << _request._errNum << "]" << std::endl;
	os << std::left << std::setw(20) << "ErrMess" << std::setw(3) << " : " << "[" << _request._errMess << "]" << std::endl;
	
	os << std::left << std::setw(20) << "URI" << std::setw(3) << " : " << "[" << _request._uri_str << "]" << std::endl;
	os << std::left << std::setw(20) << "Matched Location URI" << std::setw(3) << " : " << "[" << _request._matched_location->getURI() << "]" << std::endl;
	os << std::left << std::setw(20) << "URI remainder" << std::setw(3) << " : " << "[" << _request._leftover << "]" << std::endl;
	os << std::left << std::setw(20) << "Matched Location Methods" << std::setw(3) << " : " << "[" << _request._matched_location->getMethod() << "]" << std::endl;
	if ( p_status == "P ERROR")
		os << std::left << std::setw(20) << "Path" << std::setw(3) << " : " << "[" << _request._re.paths.error_path.path << "]" << std::endl << std::endl;
	else if (_request._re.paths.upload_paths.size()){
		for (size_t i = 0; i < _request._re.paths.upload_paths.size(); i++)
			os << std::left << std::setw(20) << "Upload Path [" + toString(i) + "]" << std::setw(3) << " : " << "[" << _request._re.paths.upload_paths[i].first.path << "]" << std::endl << std::endl;
	}
	else {
		os << std::left << std::setw(20) << "Path" <<  std::setw(3) << " : " << "[" << _request._re.paths.resource_path.path << "]" << std::endl << std::endl;
	}
	std::string result = os.str();
	return result;
}

//setters

void				Client::setLastActivity()
{
	_last_active = std::time(0);
}