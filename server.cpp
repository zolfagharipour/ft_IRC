#include "server.hpp"

int Server::_signal = false;

Server::Server() : _port(6667), _serverName("irc.fzserver"), _password("00"){ }

Server::Server( int port ) : _port(port) { }

Server::~Server()
{
	_closeFds();
	// removeClient()
}

void	Server::joinChannel(Client *client, const std::string &channelName, std::string key) {
	Channel *channel;
	
	//does the channel exist?
	auto it = this->_channels.find(channelName);
	if (it != this->_channels.end())
		channel = it->second;
	else {
		channel = new Channel(channelName, _serverName);
		this->_channels[channelName] = channel;
		/*BROADCAST!*/
		std::string broadcast = ":" + client->getNickName() + "!" + client->getRealName() + "@" + _serverName + " JOIN " + channelName;
		std::cout << broadcast << std::endl;
		std::cout << "sideinfo: Channel: " << channelName << " created by " << client->getNickName() << std::endl;
	}

	//is client in channel?
	if (channel->userExists(client->getNickName())) {
		
		std::cerr << "ERROR: client " << client->getNickName() << " is already in channel" << std::endl;
		return ;
	}
	
	//can client join channel?
	if (channel->getInviteOnly()) {
		_numericReply(client, "473", channelName);
		// std::cerr << "ERROR: channel " << channel->getName() << " is invite only" << std::endl;
		return ;
	}

	if (!channel->getKey().empty() && key != channel->getKey()) {
        std::cerr << client->getNickName() << " provided incorrect key for channel: " << channel->getName() << std::endl;
        _numericReply(client, "475", channelName );
		return ;
    }

	if (channel->addUser(client)) {
		if (channel->getUsers().size() == 1)
			channel->addOperator(client);
	}
}

void	Server::leaveChannel(Client &client, const std::string &channelName) {
	auto it = _channels.find(channelName);
	if (it != _channels.end()) {
		Channel *channel = it->second;
		if (channel->userExists(client.getNickName())) {
			channel->removeUser(&client);
			std::cout << client.getNickName() << " left channel " << channel->getName() << std::endl;
			if (channel->isOperator(&client))
				channel->removeOperator(&client);
		}
		Client *op = channel->getOperator();
		if (!op) {
			if (!channel->getUsers().empty()) {
				Client *nextClient = channel->getUsers().begin()->second;
				channel->addOperator(nextClient);
			}
		}
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
	// std::cout << "Channel " << channel->getName() << " added on server" << std::endl;
}

void	Server::addChannel(std::string name) {
	Channel channel(name, _serverName);
	_channels[channel.getName()] = &channel;
	std::cout << "Channel " << channel.getName() << " added on server" << std::endl;
}

void	Server::printChannels() {
    std::cout << "Channels on the server:\t\t";
    for (std::map<std::string, Channel *>::const_iterator it = _channels.begin(); it != _channels.end(); ++it) {
        std::cout << it->first << " "; // Print the nickname
    }
    std::cout << std::endl;
}