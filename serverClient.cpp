#include "server.hpp"

void	Server::_clientRegister() {
	Client				newClient;
	struct pollfd       newPoll;
	struct sockaddr_in	cliSock;
	socklen_t			sockLen = sizeof(cliSock);
	int					cliFd;


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
	send (cliFd, ":myserver 001 mzolfagh :Welcome to the IRCcom Network, mzolfagh", 58, 0);
	send (cliFd, ":myserver 002 mzolfagh :Your host is myserver, running version 1.0", 58, 0);
	send (cliFd, ":myserver 003 mzolfagh :This server was created Tue Nov 30 2011 at 11:11:25 CET", 58, 0);
	// send (cliFd, ":myserver 004 mzolfagh <servername> <version> <available umodes> <available cmodes> [<cmodes with param>]", 58, 0);
	
}

void	Server::_clientCommunicate(size_t i) {
	char	buff[512];
	int		bytesRead;

	bytesRead = recv(_pollFd[i].fd, buff, sizeof(buff) - 1, 0);
	if (bytesRead <= 0) {
		_removeClient(_pollFd[i].fd);
		close (_pollFd[i].fd);
		std::cerr << "client dissconnected" << std::endl;
	}
	else {
		buff[bytesRead] = '\0';
		std::cout << "$ " << buff;
	}

}

