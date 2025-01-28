#include "server.hpp"

int Server::_signal = false;

Server::Server( ) : _port(6667), _serverName("irc.fzserver"), _password("00")
					, _numericResponse({
		{"001", ":Welcome to the MyIrc"},
		{"331", ":No topic is set"},
		{"332", ":"},
		{"341", ""},
		{"401", " :No such nick/channel"},
		{"403", ":No such channel"},
		{"404", ":Cannot send to channel"},
		{"409", ":No origin specified"},
		{"411", ":No recipient given "},
		{"431", ":No nickname given"},
		{"432", " :Erroneous nickname"},
		{"433", " :Nickname is already in use"},
		{"441", ":They aren't on that channel"},
		{"442", ":Your're not on that channel"},
		{"443", ":is already on channel"},
		{"451", ":You have not registered"},
		{"461", " :Not enough parameters"},
		{"462", ":Unauthorized command (already registered)"},
		{"464", ":Password incorrect"},
		{"471", ":Cannot join channel (+l)"},
		{"473", ":Cannot join channel (+i)"},
		{"475", ":Cannot join channel (+k)"},
		{"479", ":Erroneous channel name"},
		{"482", ":You're not a channel operator"},
	}){}

Server::~Server( ){
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

const std::string	&Server::getName( ){
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
	std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
	if (it != _channels.end())
		return it->second;
	else
		return nullptr;
}

void	Server::joinChannel( Client *client, const std::string &channelName, std::string key ) {
	Channel *channel;
	
	if (channelName.empty())
		numericReply(client, "403", channelName, "", "");

	std::map<std::string, Channel *>::iterator it = this->_channels.find(channelName);
	if (it != this->_channels.end())
		channel = it->second;
	else {
		channel = new Channel(channelName, this);
		_channels.insert(std::make_pair(channelName, channel));
	}

	if (channel->isUserInChannel(client->getNickName())) {
		numericReply(client, "443", channelName, "", client->getNickName());
		return ;
	}
	
	if (channel->getInviteOnly() && !channel->isGuestList(client->getNickName())) {
		numericReply(client, "473", channelName, "", "");
		return ;
	}

	if (!channel->getKey().empty() && key != channel->getKey() && !channel->isGuestList(client->getNickName())) {
        numericReply(client, "475", channelName, "", "");
		return ;
    }

	if (!channel->addUser(client))
		return ;

	if (channel->isGuestList(client->getNickName()))
		channel->removeFromGuestList(client->getNickName());
	if (channel->getUsers().size() == 1)
		channel->addOperator(client, "");
}



void	Server::addChannel( Channel *channel ) {
	_channels[channel->getName()] = channel;
	//added on server
}

void	Server::addChannel( std::string name ) {
	Channel channel(name, this);
	_channels[channel.getName()] = &channel;
	std::cout << "Channel " << channel.getName() << " added on server" << std::endl;
}

void	Server::printChannels( ) {
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
