#include "server.hpp"

void	Server::_removeClient( int client, std::string message ) {
	Channel*	channel;
	std::string	broadcastMsg = "QUIT :Client quit";
	if (message.size())
		broadcastMsg = message;
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end();){
		channel = it->second;
		channel->removeUser(_clients[client], broadcastMsg, false);
		if (channel->getUsers().empty()) {
			std::map<std::string, Channel *>::iterator toErase = it++;
        	_channels.erase(toErase);
			delete channel;
		}
		else
			++it;
	}

	for (size_t i = 0; i < _pollFd.size(); i++) {
		if (_pollFd[i].fd == _pollFd[client + 1].fd) {
			_pollFd.erase(_pollFd.begin() + i);
		}
	}
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i]->getFd() == _pollFd[client + 1].fd) {
			close (_clients[i]->getFd());
			delete _clients[i];
			_clients.erase(_clients.begin() + i);
		}
	}
}
