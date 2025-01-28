#include "ft_irc.h"
#include "server.hpp"


void	Server::_capResp( std::vector<std::string> &cmds, int client ){
	if (cmds.size() > 1 && cmds[1] == "LS"){
		std::string respond = "CAP * LS :\r\n";
		send(_pollFd[client + 1].fd, respond.data(), respond.size(), 0);
		std::cout << "\n>> " << respond;
	}
}

void	Server::_passResp( std::vector<std::string> &cmds, int client ){
	if (cmds.size() < 2)
		return ;
	if (cmds[1] == _password){
		_clients[client]->authenticate();
		return ;
	}
	numericReply(_clients[client], "464", "", "", "");
}

void	Server::_userResp( std::vector<std::string> &cmds, int client ){
	std::string	realName;

	if (cmds.size() < 5 || _clients[client]->getNickName() == "*"
			|| cmds[4][0] != ':'){
		numericReply(_clients[client], "461", "", "USER", "");
		return ;
	}
	else if (_clients[client]->isRegistered()){
		numericReply(_clients[client], "462", "", "", "");
		return ;
	}

	realName = cmds[4].substr(1);
	for (int i = 5; i < cmds.size(); i++){
		realName += " " + cmds[i];
	}
	_clients[client]->setUserName(cmds[1]);
	_clients[client]->setRealName(realName);
	_clients[client]->registered();
	numericReply(_clients[client], "001", "", "", "");
}

void	Server::_pingResp( std::vector<std::string> &cmds, int client ){
		std::string respond = "PONG ";

		if (cmds.size() < 2){
			numericReply(_clients[client], "409", "", "", "");
			return ;
		}
		respond += cmds[1] + "\r\n";
		send(_pollFd[client + 1].fd, respond.data(), respond.size(), 0);
		std::cout << "\n>> " << respond;
}

void	Server::_joinResp( std::vector<std::string> &cmds, int client ) {
	if ( !_clients[client]->isRegistered() )
		return ;
	if (cmds.size() < 2 || (cmds.size() < 3 && cmds[1].size() < 1
		&& cmds[1][0] == ':' )) {
		numericReply(_clients[client], "461", "", "JOIN", "");
		return ;
	}

	std::vector<std::string> channelList;
	std::vector<std::string> keyList;
	std::stringstream ss(cmds[1]);
	std::string channelName;

	while (std::getline(ss, channelName, ',')) {
		channelName = _lowerCase(channelName);
		if (!channelName.empty() && channelName[0] == '#')
			channelList.push_back(channelName.substr(1));
	}

	if (cmds.size() > 2) {
		std::stringstream keyStream(cmds[2]);
		std::string key;
		while(std::getline(keyStream, key, ','))
			keyList.push_back(key);
	}

	for (size_t i = 0; i < channelList.size(); ++i) {
		std::string currentChannelname = channelList[i];
		std::string key = (i < keyList.size()) ? keyList[i] : "";
		if (!_channelNameValidity(currentChannelname))
			numericReply(_clients[client], "479", currentChannelname, "", "");
		else
			joinChannel(_clients[client], currentChannelname, key);
	}
}

void	Server::_partResp( std::vector<std::string> &cmds, int client ) {
	std::string nick = _clients[client]->getNickName();
	std::vector<std::string> channels;
	
	if ( !_clients[client]->isRegistered() )
		return ;
	if (cmds.size() < 2) {
		numericReply(_clients[client], "461", "", "PART", "");
		return ;
	}

	std::string channelNames = cmds[1];
	if (!channelNames.empty()) {
		std::stringstream ss(channelNames);
		std::string channel;

		while (std::getline(ss, channel, ',')) {
			if (!channel.empty() && channel[0] == '#')
				channels.push_back(channel.substr(1));
		}
	}

	for (size_t i = 0; i < channels.size(); ++i) {
		std::string currentChannelName = channels[i];

		std::map<std::string, Channel*>::iterator it = _channels.find(currentChannelName);
		if (it == _channels.end()) {
			numericReply(_clients[client], "403", currentChannelName, "", "");
        	continue ;
		}

		Channel *channel = it->second;
		if (!channel->isUserInChannel(_clients[client]->getNickName()))
			numericReply(_clients[client], "442", currentChannelName, "", "");

		std::string partMessage = "PART #" + currentChannelName;
		if (cmds.size() > 2) {
			for (size_t i = 2; i < cmds.size(); i++)
				partMessage += " " + cmds[i];
		}

		channel->removeUser(_clients[client], partMessage, true);
		
		if (channel->getUsers().empty()) {
			_channels.erase(currentChannelName);
			delete channel;
		}
	}
}

void	Server::_kickResp( std::vector<std::string> &cmds, int client ){
	if (cmds.size() < 3){
		numericReply(_clients[client], "461", "", "KICK", "");
		return ;
	}
	if (cmds[1][0] != '#')
		return ;
	Client*	kickedClient = getClient(cmds[2]);
	if (!kickedClient){
		numericReply(_clients[client], "401", "", "", cmds[2]);
		return ;
	}

	std::string	channelName = cmds[1].substr(1);
	Channel*	channel = getChannel(channelName);
	if (!channel){
		numericReply(_clients[client], "403", channelName, "", "");
		return ;
	}
	std::string reason;
	for (size_t i = 3; i < cmds.size(); i++){
		reason += " " + cmds[i];
	}
	channel->kickUser(_clients[client], kickedClient, reason);
}

void	Server::_modeResp( std::vector<std::string> &cmds, int client ) {
	Client *clientPtr = _clients[client];
	std::string channelName = cmds[1];

	if (channelName[0] != '#' || cmds.size() < 3)
		return ;

	if (!channelName.empty() && channelName[0] == '#')
			channelName = channelName.substr(1);

	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	if (it == _channels.end()) {
		numericReply(_clients[client], "403", channelName, "", "");
		return ;
	}

	Channel *channel = it->second;

	if (cmds[2] == "+t\0")
		channel->setTopicRestriction(clientPtr, true);
	else if (cmds[2] == "-t\0")
		channel->setTopicRestriction(clientPtr, false);
	else if (cmds[2] == "+i\0")
		channel->setInviteOnly(clientPtr, true);
	else if (cmds[2] == "-i\0")
		channel->setInviteOnly(clientPtr, true);
	else if (cmds.size() > 3  && cmds[2] == "+k\0")
		channel->setKey(clientPtr, cmds[3]);
	else if (cmds[2] == "-k\0")
		channel->removeKey(clientPtr);
	else if (cmds[2] == "+o\0")
		channel->changeOperatorPrivilege(clientPtr, true, cmds);
	else if (cmds[2] == "-o\0")
		channel->changeOperatorPrivilege(clientPtr, false, cmds);
	else if (cmds[2] == "+l\0")
		channel->setUserLimit(clientPtr, cmds);
	else if (cmds[2] == "-l\0")
		channel->removeUserLimit(clientPtr);
}

void	Server::_inviteResp( std::vector<std::string> &cmds, int client ) {
	Client *sourceClient = _clients[client];
	std::string channelName = cmds[2];
	std::string nickName = cmds[1];

	if (!channelName.empty() && channelName[0] == '#')
		channelName = channelName.substr(1);

	std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
	if (it == _channels.end())  {
        numericReply(sourceClient, "403", channelName, "", "");
        return ;
    }
	
	Channel *channel = it->second;
	Client *targetClient = getClient(nickName);
	
	if (!targetClient) {
        numericReply(sourceClient, "401", "", "", nickName);
        return ;
    }

	channel->inviteUser(sourceClient, targetClient);
}

void	Server::_topicResp( std::vector<std::string> &cmds, int client ) {
	Client *clientPtr = _clients[client];
	std::string channelName = cmds[1];

	if (!channelName.empty() && channelName[0] == '#')
			channelName = channelName.substr(1);

	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	if (it == _channels.end()) {
		numericReply(_clients[client], "403", channelName, "", "");
		return ;
	}
	Channel *channel = it->second;
	if (cmds.size() == 2) {
		if (channel->getTopic().empty())
			numericReply(clientPtr, "331", channelName, "", "");
		else
			numericReply(clientPtr, "332", channelName, "", "");
	}
	else {
		if (!channel->isUserInChannel(clientPtr->getNickName())) {
			numericReply(_clients[client], "442", channelName, "", "");
			return ;
		}
		std::string newTopic;
		for (size_t i = 2; i < cmds.size(); ++i) {
			if (i > 2)
				newTopic += " ";
			newTopic += cmds[i];
		}
		if (!newTopic.empty() && newTopic[0] == ':')
			newTopic = newTopic.substr(1);
		channel->setTopic(clientPtr, newTopic);
    }
}

void	Server::_quitResp( std::vector<std::string> &cmds, int client ){
	std::string	respond = "QUIT";

	for (size_t i = 1; i < cmds.size(); i++)
		respond += " " + cmds[i];
	if (cmds.size() == 1)
		respond = "";
	_removeClient(client, respond);
}

bool	Server::_parser( std::vector<std::string> &cmds, int client ){
	if (!_clients[client]->isAuthenticated()
			&& cmds[0] != "CAP" && cmds[0] != "PASS"){
		return true ;
	}
	std::string command = cmds[0];
	std::map<std::string, CommandHandler>::iterator it = _commandFn.find(cmds[0]);
	if (it != _commandFn.end())
		(this->*(it->second))(cmds, client);
	
	if (command == "QUIT")
		return false;
	return true;
}

void	Server::_serverRespond( int client ){
	std::vector<std::string>	cmds;
	
	cmds = _clients[client]->getCommand();

	while (cmds.size()){
		if (!_parser(cmds, client))
			return;
		_clients[client]->clearBuff();
		cmds = _clients[client]->getCommand();
	}
}

