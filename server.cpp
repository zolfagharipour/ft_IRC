#include "server.hpp"

int Server::_signal = false;

Server::Server() : _port(6667)
{ }

Server::~Server()
{
	closeFds();
}

void	Server::handler(int signum) {
	(void)signum;
	_signal = true;
	std::cout << "Signal received: Server stopped" << std::endl;
}

void Server::signalMonitor () {
	int signals[] = {SIGINT, SIGQUIT, SIGTERM, SIGPIPE};
	for (size_t i = 0; i < sizeof(signals) / sizeof(signals[0]); i++)
		signal(signals[i], &handler);
}

void	Server::SocketInit() {
	
	struct sockaddr_in	sockAdd;
	struct pollfd       newPoll;

	sockAdd.sin_addr.s_addr = INADDR_ANY;
	sockAdd.sin_family = AF_INET;
	sockAdd.sin_port = htons(_port);
	
	_serFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serFd < 0) 
		throw (std::runtime_error("socket creation failed"));

	int	ov = 1;
	if (setsockopt(_serFd, SOL_SOCKET, SO_REUSEADDR, &ov, sizeof(ov)) < 0) {
		throw (std::runtime_error("socket option failed to set on reusable"));
	}

	if (fcntl(_serFd, F_SETFL, O_NONBLOCK) < 0){
		throw (std::runtime_error("socket option failed to set on non-block"));
	}

	if (bind(_serFd, (sockaddr *)&sockAdd, sizeof(sockAdd)) < 0){
		throw (std::runtime_error("socket option failed to set on non-block"));
	}

	if (listen(_serFd, SOMAXCONN) < 0) {
		throw (std::runtime_error("server could not listen"));
	}

	newPoll.fd = _serFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	_pollFd.push_back(newPoll);

	std::cout << "Server socket created; waiting for a connection..." << std::endl;
}

void Server::ServerInit() {
	SocketInit();

	while (!_signal) {
		if (poll(&_pollFd[0], _pollFd.size(), -1) < 0 && !_signal) {
			throw (std::runtime_error("poll() failed"));
        }
		for (size_t i = 0; i < _pollFd.size(); i++) {
			if (_pollFd[i].revents & POLLIN) {
				if (_pollFd[i].fd == _serFd) {
				    clientRegister();
				}
				else {

					clientCommunicate(i);
				}
				// std::cout << "yes: " << i << std::endl;
			}
		}
	}
}

void	Server::clientRegister() {
	struct pollfd       newPoll;
	Client				newClient;
	struct sockaddr_in	cliSock;
	int					cliFd;
	socklen_t			sockLen = sizeof(cliSock);


	cliSock.sin_addr.s_addr = INADDR_ANY;
	cliSock.sin_family = AF_INET;
	cliSock.sin_port = htons(_port);

	cliFd = accept(_serFd, (struct sockaddr *)&cliSock, &sockLen);
	if (cliFd < 0)
		throw (std::runtime_error("server could not listen"));
	int ov = 1;
	if (setsockopt(cliFd, SOL_SOCKET, SO_REUSEADDR, &ov, sizeof(ov)) < 0) {
		throw (std::runtime_error("socket option failed to set on reusable"));
	}

	if (fcntl(cliFd, F_SETFL, O_NONBLOCK) < 0){
		throw (std::runtime_error("socket option failed to set on non-block"));
	}


	newClient.setFd(cliFd);
	_clients.push_back(newClient);

	newPoll.fd = cliFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	_pollFd.push_back(newPoll);
	std::cout << "Client is registered" << std::endl;
}

void	Server::clientCommunicate(size_t i) {
	char	buff[512];
	int		bytesRead;

	bytesRead = recv(_pollFd[i].fd, buff, sizeof(buff) - 1, 0);
	if (bytesRead <= 0) {
		removeClient(_pollFd[i].fd);
		close (_pollFd[i].fd);
		std::cerr << "client dissconnected" << std::endl;
	}
	else {
		buff[bytesRead] = '\0';
		std::cout << "Client: " << buff;
	}

}

void	Server::closeFds() {
	for (size_t i = 0; i < _clients.size(); i++){
		close (_clients[i].getFd());
	}
	close (_serFd);
}

void	Server::removeClient(int fd) {
	for (size_t i = 0; i < _pollFd.size(); i++) {
		if (_pollFd[i].fd == fd) {
			_pollFd.erase(_pollFd.begin() + i);
		}
	}
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getFd() == fd) {
			_clients.erase(_clients.begin() + i);
		std::cerr << "erased client" << std::endl;
		}
	}
}

