#include "Server.hpp"
#include "Config.hpp"
#include "Log.hpp"

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<
>>>>>>		CONSTRUCTORS		<<<<<<
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/

Server::Server(Config &config) : _config(&config), _running(false), _max_conn(config.getMaxConnections()), _port_count(config.getServerPortCount()){

	Log::instance().log(Log::INFO, Log::TYPE_CLASS, "Server starting....", true);
	setupListeningSockets(config);

	addListeningSocketsToPoll();
	Log::instance().log(Log::INFO, Log::TYPE_CLASS, "Server successfully started", true);
}

/* >>>>>>	DESTRUCTOR	<<<<<<*/

Server::~Server(void){

	_sockets.clear();
	_clients.clear();
	_listen_sockets.clear();

	for(int i = 0; i < _port_count + _max_conn; i++){
		if (_sockets[i].fd != -1)
			close(_sockets[i].fd);
	}
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>		SET UP LISTENING SOCKETS		<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<
*/

/* >>>>>>	ITERATE OVER PORT:SERVER MAP AND CREATE LISTENING SOCKETS	<<<<<<*/

void	Server::setupListeningSockets(Config& config){

	std::map<listen_s, std::vector<CServer*> > port_server_map = config.getPortServerMap();
	std::map<listen_s, std::vector<CServer*> >::iterator it;

	try {
		for (it = port_server_map.begin(); it != port_server_map.end(); it++) {
			int fd = createListeningSocket(it->first);
			listenInstanceCreate(fd, it->first, it->second);
		}
	}
	catch (const std::exception& e){
		std::string err = e.what();
		throw std::runtime_error(err + " : server stopped");
	}
}

/* >>>>>>	CREATE LISTENING INSTANCE (HOLDS SOCKET FD, MAP OF SERVER_NAME TO SERVER, DEFAULT SERVER FOR SOCKET)	<<<<<<*/

void	Server::listenInstanceCreate(int fd, listen_s listen, std::vector<CServer*>& servers){

	listenInstance_s instance(fd, listen, _config, servers);

	for (size_t i = 0; i < servers.size(); i++){

		listenInstanceNameServerMap(instance.name_to_srv, servers[i], (servers[i])->getServerName());
		
		if (listenInstanceDefaultServer(servers[i]->getListen(), listen)){
			if (instance.def_srv)
				throw std::runtime_error("Multiple default_server for same port");
			instance.def_srv = servers[i];
		}
	}
	if (!instance.def_srv)
		instance.def_srv = servers[0];
	if (!instance.def_srv->getServerName().empty())
		instance.def_srv_name = createArgsString(instance.def_srv->getServerName());
	else
		instance.def_srv_name = "--NO NAME--";
	Log::instance().log(Log::INFO, Log::TYPE_CONF, "listening socket created : " + printstream(instance), false);
	
	_listen_sockets.push_back(instance);
}

/* >>>>>>	CREATE MAP OF SERVER NAME TO SERVER (TO ASSIST WITH HOSTNAME TO SERVER NAME MATCHING)	<<<<<<*/

void	Server::listenInstanceNameServerMap(std::map<std::string, CServer* >& map, CServer* server, std::vector<std::string> names){

	for (size_t i = 0; i < names.size(); i++){

		std::string name = names[i];
		std::map<std::string, CServer* >::iterator it = map.find(name);

		if (it != map.end())
			throw std::runtime_error("Listen Instance : duplicate server name");
		map[name] = server;
	}
}

/* >>>>>>	SET DEFAULT SERVER FOR A LISTENING SOCKET	<<<<<<*/

bool	Server::listenInstanceDefaultServer(std::vector<listen_s> listens, listen_s target){

	for (size_t i = 0; i < listens.size(); i++){
		if (target.ip == listens[i].ip && target.port == listens[i].port && listens[i].is_default)
			return true;
	}
	return false;
}

/* >>>>>>	CREATE LISTENING SOCKET	<<<<<<*/

int	Server::createListeningSocket(const listen_s& port_info)
{
	int	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sockfd < 0)
		throw std::runtime_error("socket() failed!");

	int	opt = 1;

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	// SO_REUSEADDR allows us to reuse the port immediately after the server is closed, without waiting for the OS to release it (which can take a few minutes)

	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_info.port);
  	addr.sin_addr.s_addr = port_info.ip;

	if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0){
		std::cout << "bind() failed for" << port_info << std::endl;
		throw std::runtime_error("bind() failed!");
	}

	if (listen(sockfd, SOMAXCONN) < 0)
		throw std::runtime_error("listen() failed!");

	return sockfd;
}

/* >>>>>>	ADD LISTENING SOCKET TO POLL	<<<<<<*/

void	Server::addListeningSocketsToPoll(void){
	size_t size = _listen_sockets.size();

	for (size_t i = 0; i < size; i++){

		pollfd	pfd;

		pfd.fd = _listen_sockets[i].fd;
		pfd.events = POLLIN;
		pfd.revents = 0;

		int flags = fcntl(pfd.fd, F_GETFL, 0);

		if (flags < 0)
			throw std::runtime_error("fcntl(F_GETFL) failed!");

		if (fcntl(pfd.fd, F_SETFL, flags | O_NONBLOCK) == -1)
			throw std::runtime_error("fcntl(F_SETFL) failed!");

		_sockets.push_back(pfd);
	}
}

/*
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
>>>>>>		SERVER LOOP		<<<<<<
>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<
*/

void	Server::loop(){

	Log::instance().log(Log::INFO, Log::TYPE_CLASS, "Server listening....", false);

	_running = true;

	while (_running) {
		
		int activity = poll(&_sockets[0], _port_count + _clients.size(), -1);
		
		if (activity < 0){
			Log::instance().log(Log::INFO, Log::TYPE_CLASS, "poll() failed", true);
			continue;
		}
		handleEvents();
	}
}

/*
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		EVENT HANDLING		<<<<<<
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/

void	Server::handleEvents(void){

	for (int i = 0; i < (int)_sockets.size(); ) {

		if (_sockets[i].revents == 0)
		{
			i++;
			continue;
		}

		if (i < _port_count)
			handleAccept(i);

		else {

			int fd = _sockets[i].fd;

			if (_sockets[i].revents & POLLOUT)
				handleClientWrite(fd);
				
			if (_sockets[i].revents & POLLIN)
				handleClientRead(fd);

			if (_sockets[i].revents & (POLLERR | POLLHUP | POLLNVAL))
			{
				disconnectClient(fd, "handleEvents: POLLERR | POLLHUP | POLLNVAL");
				continue;
			}
		}
		_sockets[i].revents = 0;
		i++;
	}
}

/*
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
>>>>>>		CLIENT HANDLING		<<<<<<
>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<
*/

/* >>>>>>	ACCEPT CLIENT CONNECTION	<<<<<<*/

void	Server::handleAccept(int index){

	while (true)
	{
		sockaddr_in addr;
		socklen_t len = sizeof(addr);

		int client_fd = accept(_sockets[index].fd, (sockaddr*)&addr, &len);

		if (client_fd < 0){
			Log::instance().log(Log::ERROR, Log::TYPE_ERROR, "accept() failed: " + std::string(strerror(errno)), true);
			break;
		}
		try{
			setNonBlocking(client_fd);
		}
		catch (const std::exception& e){
			Log::instance().log(Log::ERROR, Log::TYPE_ERROR, "Failed to set non-blocking mode for client fd [" + toString(client_fd) + "] : " + " : " + e.what(), true);
			close(client_fd);
			continue;
		}
		addClient(client_fd, index);
		Log::instance().log(Log::INFO, Log::TYPE_ACCESS, "New client connected: fd [" + toString(client_fd) + "]", true);
		break;
	}
}

/* >>>>>>	ADD CLIENT TO POLL	<<<<<<*/

void	Server::addClient(int client_fd, int listen_index){

	Client	new_client(&_listen_sockets[listen_index], client_fd);

	_clients.insert(std::make_pair(client_fd, new_client));

	pollfd pfd;
	pfd.fd = client_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	_sockets.push_back(pfd);
}

/* >>>>>>	READ FROM CLIENT	<<<<<<*/

void	Server::handleClientRead(int fd){

	std::map<int, Client>::iterator it = _clients.find(fd);

	if (it == _clients.end()){
		Log::instance().log(Log::ERROR, Log::TYPE_ERROR, "handleClientRead: unknown fd [" + toString(fd) + "] : ", true);
		return;
	}
	Client&	client = it->second;

	char buffer[BUF_SIZE];

	ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);

	if (bytes < 0)
		disconnectClient(fd, "Invalid read");
	else if (bytes == 0)
		disconnectClient(fd, "Read size of zero");
	else if (client.handleClientRead(buffer, bytes)){
		_sockets[getSocketsPosition(fd)].events |= POLLOUT;
	}
}

/* >>>>>>	WRITE TO CLIENT	<<<<<<*/

void    Server::handleClientWrite(int fd)
{
    std::map<int, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end()){
		Log::instance().log(Log::ERROR, Log::TYPE_ERROR, "handleClientWrite: unknown fd [" + toString(fd) + "]", true);
		return;
	}

    Client& client = it->second;
    std::string &response = client.get_msg();

    ssize_t sent = send(fd, response.c_str(), response.size(), 0);

    if (sent < 0) {
        disconnectClient(fd, "send error");
        return;
    }
	response.erase(0, sent);

    if (response.empty()) {
        _sockets[getSocketsPosition(fd)].events &= ~POLLOUT;

		if (client.getConn() == CONN_CLOSE) {
            disconnectClient(fd, "Finished sending: Connection set to type 'close'");
        }
    }
}

/*
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
>>>>>>		CLIENT DISCONNECT		<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

/* >>>>>>	DISCONNECT CLIENT	<<<<<<*/

void	Server::disconnectClient(int fd, const std::string& reason){

	Log::instance().log(Log::INFO, Log::TYPE_ACCESS, "Client disconnected: fd [" + toString(fd) + "] : " + reason, true);
	close(fd);
	removeClientFromPoll(fd);
	_clients.erase(fd);
}

/* >>>>>>	REMOVE CLIENT FROM POLL	<<<<<<*/

void Server::removeClientFromPoll(int fd) {

	close(fd);

	for (size_t i = 0; i < _sockets.size(); i++){
		if (_sockets[i].fd == fd){
			_sockets.erase(_sockets.begin() + i);
			break;
		}
	}
}

/* >>>>>>	SET SOCKET TO NON-BLOCKING	<<<<<<*/

void	Server::setNonBlocking(int fd){

	int flags = fcntl(fd, F_GETFL, 0);

	if (flags < 0)
		throw std::runtime_error("fcntl(F_GETFL) failed!");

	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl(F_SETFL) failed!");
}

/* >>>>>>	GET POSITION OF FD IN POLLING 	<<<<<<*/

int	Server::getSocketsPosition(int fd)
{
	for (size_t i = 0; i < _sockets.size(); i++){

		if (_sockets[i].fd == fd)
			return i;
	}
	return -1;
}

/*
>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<
>>>>>>		PRINT SERVER INFO 		<<<<<<
>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<
*/

// Listening Instance
std::ostream&	operator<<(std::ostream& os, const listenInstance_s& li){
	os
	<< "\n\n\tListening Socket fd: " << li.fd << ", IP : "
	<< ( li.ip & 0xFF) << "."
	<< ((li.ip >> 8)  & 0xFF) << "."
	<< ((li.ip >> 16) & 0xFF) << "."
	<< ((li.ip >> 24) & 0xFF)
	<< ", port : " << li.port
	<< "\n\n\tDefault server (name) : " << li.def_srv_name
	<< li.name_to_srv;
	return os;
}

// Listen Instance Map (Name to server)
std::ostream&	operator<<(std::ostream& os, const std::map<std::string, CServer*>& map){

	int i = 0;

	if (map.size() == 0)
		return os << "\n\n\tNo server names in config : hostname cannot be mapped to server name";
	os << "\n\n\tListening Socket Name to Server Map";

	for (std::map<std::string, CServer*>::const_iterator it = map.begin(); it != map.end(); it++, i++)
		os << "\n\tServer[" << i << "] name(s) : " << it->first  << "\t\tServer config address: " << it->second;
	return os;
}