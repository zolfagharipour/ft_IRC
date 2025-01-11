#include "server.hpp"

int Server::_signal = false;

Server::Server() : _port(6667), _serverName("irc.fzserver"), _password("00"){ }

Server::Server( int port ) : _port(port) { }

Server::~Server(){
	for (size_t i = 0; i < _pollFd.size(); ++i)
		close (_pollFd[i].fd);
	_pollFd.clear();
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end();) {
		delete it->second;
		it = _channels.erase(it);
	}
	for (size_t i = 0; i < _clients.size(); i++) {
		delete _clients[i];
	}
}

const std::string	&Server::getName(){
	return _serverName;
}

Client *Server::getClient( std::string clientName ){
	std::vector<Client*>::iterator it = _clients.begin();
	for (; it != _clients.end(); ++it){
		if ((*it)->getNickName() == clientName)
			return (*it);
	}
	return nullptr;
}

Channel *Server::getChannel( std::string channelName) {
	auto it = _channels.find(channelName);
	if (it != _channels.end())
		return it->second;
	else
		return nullptr;
}

void	Server::joinChannel( Client *client, const std::string &channelName, std::string key ) {
	Channel *channel;
	
	if (channelName.empty())
		numericReply(client, "403", channelName);

	//does the channel exist?
	auto it = this->_channels.find(channelName);
	if (it != this->_channels.end())
		channel = it->second;
	else {
		channel = new Channel(channelName, this);
		_channels.insert(std::make_pair(channelName, channel));
	}

	//is client in channel?
	if (channel->isUserInChannel(client->getNickName())) {
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

	channel->addUser(client);
	if (channel->getUsers().size() == 1)
		channel->addOperator(client, "");
}



void	Server::addChannel(Channel *channel) {
	_channels[channel->getName()] = channel;
	//added on server
}

void	Server::addChannel(std::string name) {
	Channel channel(name, this);
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

bool	Server::_channelNameValidity( std::string& channelName ){
    if (channelName.empty() || channelName[0] == ':') {
        return false;
    }

	for (size_t i = 1; i < channelName.size(); ++i) {
        char c = channelName[i];
        if (c == ' ' || c == ',' || c == '\0' || c == ':') {
            return false;
        }
    }

    return true;
}
