#include "server.hpp"

int Server::_signal = false;

Server::Server() : _port(6667), _serverName("irc.fzserver"), _password("00"){ }

Server::Server( int port ) : _port(port) { }

Server::~Server()
{
	_closeFds();
	// removeClient()
}

void	Server::joinChannel( Client *client, const std::string &channelName, std::string key ) {
	Channel *channel;
	
	if (channelName.empty()) {
		std::cout << "Is the following supposed to be printed?" << std::endl;
		numericReply(client, "403", channelName);
	}
	else if (channelName[0] != '#')
		std::cout << "here" << std::endl;

	//does the channel exist?
	auto it = this->_channels.find(channelName);
	if (it != this->_channels.end())
		channel = it->second;
	else {
		channel = new Channel(channelName, _serverName);
		_channels.insert(std::make_pair(channelName, channel));
	}

	//is client in channel?
	if (channel->userExists(client->getNickName())) {
		/*proper numeric reply error*/
		numericReply(client, "443", channelName);
		//is already in channel
		return ;
	}
	
	//can client join channel?
	if (channel->getInviteOnly()) {
		numericReply(client, "473", channelName);
		//is invite only
		return ;
	}

	if (!channel->getKey().empty() && key != channel->getKey()) {
        numericReply(client, "475", channelName );
		return ;
    }
	std::cout << "sth else: " << client->getFd() << std::endl;

	channel->addUser(client);
	if (channel->getUsers().size() == 1)
		channel->addOperator(client);
}

void	Server::leaveChannel(Client *client, const std::string &channelName) {
	auto it = _channels.find(channelName);
	if (it == _channels.end()) {
		numericReply(client, "403", channelName);
		return ;
	}

	Channel *channel = it->second;

	if (!channel->userExists(client->getNickName())) {
		numericReply(client, "442", channelName);
		return ;
	}
	
	channel->removeUser(client);
	std::cout << client->getNickName() << " left channel " << channel->getName() << std::endl;
	
	if (channel->isOperator(client))
		channel->removeOperator(client);
	
	/*check if new operator needed*/
	if (!channel->getOperator() && !channel->getUsers().empty()) {
		Client *nextClient = channel->getUsers().begin()->second;
		channel->addOperator(nextClient);
	}

	if (channel->getUsers().empty()) {
		delete channel;
		_channels.erase(it);
		std::cout << "Channel " << channelName << "has been deleted because it's now empty" << std::endl;
	}
}

Channel *Server::getChannel( std::string channelName) {
	auto it = _channels.find(channelName);
	if (it != _channels.end())
		return it->second;
	else
		return nullptr;
}

void	Server::addChannel(Channel *channel) {
	_channels[channel->getName()] = channel;
	//added on server
}

void	Server::addChannel(std::string name) {
	Channel channel(name, _serverName);
	_channels[channel.getName()] = &channel;
	std::cout << "Channel " << channel.getName() << " added on server" << std::endl;
}

void	Server::printChannels() {
    std::cout << "Channels on the server:\t\t";
    for (std::map<std::string, Channel *>::const_iterator it = _channels.begin(); it != _channels.end(); ++it) {
        std::cout << it->first << " ";// Print the nickname
    }
    std::cout << std::endl;
}