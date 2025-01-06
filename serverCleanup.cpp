#include "server.hpp"


void	Server::_closeFds() {
	for (size_t i = 0; i < _pollFd.size(); ++i)
		close (_pollFd[i].fd);
	_pollFd.clear();
}

void	Server::_removeClient(int fd) {
	for (size_t i = 0; i < _pollFd.size(); i++) {
		if (_pollFd[i].fd == fd) {
			// close (_pollFd[i].fd);
			_pollFd.erase(_pollFd.begin() + i);
		}
	}
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i]->getFd() == fd) {
			// delete _clients[i];
			_clients.erase(_clients.begin() + i);
		}
	}
}
