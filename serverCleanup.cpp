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

	_pollFd.erase(_pollFd.begin() + client + 1);

	close (_clients[client]->getFd());
	delete _clients[client];
	_clients.erase(_clients.begin() + client);
}
