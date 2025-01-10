#include "server.hpp"


void	Server::_closeFds() {
	for (size_t i = 0; i < _pollFd.size(); ++i)
		close (_pollFd[i].fd);
	_pollFd.clear();
}

void	Server::_removeClient( int client ) {
	Channel*	channel;
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it){
		channel = it->second;
		if (channel->isUserInChannel(_clients[client]->getNickName())){
			channel->removeOperator(_clients[client]);
			channel->removeUser(_clients[client]);
		}
	}
	for (size_t i = 0; i < _pollFd.size(); i++) {
		if (_pollFd[i].fd == _pollFd[client + 1].fd) {
			_pollFd.erase(_pollFd.begin() + i);
		}
	}
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i]->getFd() == _pollFd[client + 1].fd) {
			delete _clients[i];
			_clients.erase(_clients.begin() + i);
		}
	}
}
