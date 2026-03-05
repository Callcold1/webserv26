#include "Server.hpp"
#include "Config.hpp"
#include "Log.hpp"

Server::Server(Config &config) : _config(&config), _running(false), _max_conn(config.getMaxConnections()), _port_count(config.getServerPortCount()){

	Log::instance().log(Log::INFO, Log::TYPE_CLASS, "Server starting....");
	setupListeningSockets(config);

	addListeningSocketsToPoll();
	Log::instance().log(Log::INFO, Log::TYPE_CLASS, "Server successfully started");
}

Server::~Server(void){

	_sockets.clear();
	_clients.clear();
	_listen_sockets.clear();

	for(int i = 0; i < _port_count + _max_conn; i++)
	{
		if (_sockets[i].fd != -1)
			close(_sockets[i].fd);
	}
}

void	Server::setupListeningSockets(Config& config){

	std::map<listen_s, std::vector<CServer*> > port_server_map = config.getPortServerMap();
	std::map<listen_s, std::vector<CServer*> >::iterator it;

	for (it = port_server_map.begin(); it != port_server_map.end(); it++) {

		uint32_t ip = it->first.ip;
		uint16_t port = it->first.port;
		std::vector<CServer*>& servers_on_port = it->second;

		int fd = createListeningSocket(it->first);

		CServer* default_srv = NULL;
		for (std::vector<CServer*>::iterator s_it = servers_on_port.begin(); s_it != servers_on_port.end(); s_it++) {

			CServer* s = *s_it;
			std::vector<listen_s> listens = s->getListen();
			std::vector<listen_s>::iterator ls;

			for (ls = listens.begin(); ls != listens.end(); ls++) {
				if (ls->ip == ip && ls->port == port && ls->is_default) {
					default_srv = s;
					break;
				}
			}
			if (default_srv)
				break;
		}
		if (!default_srv)
			default_srv = *servers_on_port.begin();
		listenInstance_s instance(default_srv, fd, it->first);

		instance.server_configs = servers_on_port;
		instance.default_server = default_srv;

		Log::instance().log(Log::INFO, Log::TYPE_ACCESS, "listening socket created : " + printstream(it->first));
		_listen_sockets.push_back(instance);
	}
}

int	Server::createListeningSocket(const listen_s& port_info)
{
	int	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sockfd < 0)
		throw std::runtime_error("socket() failed!");

	int	opt = 1; // Enable address reuse to avoid "Address already in use" error when restarting the server

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
	else
		std::cout << "bound to : " << port_info << std::endl;

	if (listen(sockfd, SOMAXCONN) < 0) // need to change SOMAXCONN to our max connections - does this value equal our total connections count, or just this socket?
		throw std::runtime_error("listen() failed!");

	return sockfd;
}

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

void	Server::loop(){

	Log::instance().log(Log::INFO, Log::TYPE_CLASS, "Server listening....");

	_running = true;

	while (_running)
	{
		//update_poll_events();

		int activity = poll(&_sockets[0], _port_count + _clients.size(), -1);

		if (activity < 0){
			Log::instance().log(Log::INFO, Log::TYPE_CLASS, "poll() failed: " + std::string(strerror(errno)));
			continue;
		}
		handleEvents();
	}
}

void	Server::handleEvents(void){

	for (int i = 0; i < (int)_sockets.size(); i++) {

		if (_sockets[i].revents == 0)
			continue;

		if (i < _port_count)
			handleAccept(i);

		else {

			int fd = _sockets[i].fd;

			if (_sockets[i].revents & POLLIN)
				handleClientRead(fd);

			if (_sockets[i].revents & POLLOUT)
				handleClientWrite(fd);

			if (_sockets[i].revents & (POLLERR | POLLHUP | POLLNVAL))
				disconnectClient(fd);
		}
		_sockets[i].revents = 0;
	}
}

void	Server::handleAccept(int index){

	while (true)
	{
		sockaddr_in addr;
		socklen_t len = sizeof(addr);

		int client_fd = accept(_sockets[index].fd, (sockaddr*)&addr, &len);

		if (client_fd < 0){
			Log::instance().log(Log::ERROR, Log::TYPE_ERROR, "accept() failed: " + std::string(strerror(errno)));
			break;
		}
		try{
			setNonBlocking(client_fd);
		}
		catch (const std::exception& e){
			Log::instance().log(Log::ERROR, Log::TYPE_ERROR, "Failed to set non-blocking mode for client fd=" + toString(client_fd) + " : " + e.what());
			close(client_fd);
			continue;
		}
		//addClient(client_fd);
		addClient(client_fd, index);

		Log::instance().log(Log::INFO, Log::TYPE_ACCESS, "New client connected: fd=" + toString(client_fd));
	}
}

void	Server::setNonBlocking(int fd){

	int flags = fcntl(fd, F_GETFL, 0);

	if (flags < 0)
		throw std::runtime_error("fcntl(F_GETFL) failed!");

	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl(F_SETFL) failed!");
}

void	Server::addClient(int client_fd, int listen_index){

	//Client	new_client(_config, client_fd);						//instantiate client with config
	Client	new_client(&_listen_sockets[listen_index], client_fd);  //instantiate client with listen_instance_s (limits config to servers listening on that ip:port)

	_clients.insert(std::make_pair(client_fd, new_client));

	pollfd pfd;
	pfd.fd = client_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	_sockets.push_back(pfd);
}

void	Server::handleClientRead(int fd){

    std::map<int, Client>::iterator it = _clients.find(fd);

	if (it == _clients.end()){
        Log::instance().log(Log::ERROR, Log::TYPE_ERROR, "handleClientRead: unknown fd=" + toString(fd));
        return;
    }

    Client&	client = it->second;

    char buffer[BUF_SIZE];

    ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);

    if (bytes < 0){
		Log::instance().log(Log::ERROR, Log::TYPE_ERROR, "handleClientRead() : Invalid read : client disconnected : fd " + toString(fd));
        disconnectClient(fd);
        return;
    }
    else if (bytes == 0) {
		Log::instance().log(Log::ERROR, Log::TYPE_ACCESS, "handleClientRead() : Read size of zero : fd " + toString(fd));
        disconnectClient(fd);
        return;
    }
	else if (client.handleClientRead(buffer, bytes))		// expect true if parsing complete (client will hold info on what is to be in response)
		_sockets[getSocketsPosition(fd)].events |= POLLOUT;
}

void	Server::handleClientWrite(int fd)
{
    std::string response = "We can write this later";  // Need actual response logic here....

    ssize_t sent = send(fd, response.c_str(), response.size(), 0);

    if (sent <= 0){
        disconnectClient(fd);
        return;
    }

    response.erase(0, sent);

    if (response.empty())
        _sockets[getSocketsPosition(fd)].events &= ~POLLOUT;
}

void	Server::disconnectClient(int fd){

    Log::instance().log(Log::INFO, Log::TYPE_ACCESS, "Client disconnected: fd=" + toString(fd));
    close(fd);
    removeClientFromPoll(fd);
    _clients.erase(fd);
}

void	Server::removeClientFromPoll(int fd){

	for (size_t i = 0; i < _sockets.size(); i++){

		if (_sockets[i].fd == fd){
			_sockets[i].fd = -1;
			_sockets[i].events = 0;
			_sockets[i].revents = 0;
			break;
		}
	}
}

int	Server::getSocketsPosition(int fd)
{
	for (size_t i = 0; i < _sockets.size(); i++){

		if (_sockets[i].fd == fd)
			return i;
	}
	return -1;
}
