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
		_clients[client].authenticate();
		return ;
	}
	numericReply(&_clients[client], "464", "");
	// _removeClient(_clients[client].getFd());
}

void	Server::_userResp( std::vector<std::string> &cmds, int client ){
	std::string	realName;

	if (cmds.size() < 5 || _clients[client].getNickName() == "*"
			|| cmds[4][0] != ':'){
		numericReply(&_clients[client], "461", "");
		return ;
	}
	else if (_clients[client].isRegistered()){
		numericReply(&_clients[client], "462", "");
		return ;
	}

	realName = cmds[4].substr(1);
	for (int i = 5; i < cmds.size(); i++){
		realName += " " + cmds[i];
	}
	_clients[client].setUserName(cmds[1]);
	_clients[client].setRealName(realName);
	_clients[client].registered();
	numericReply(&_clients[client], "001", "");
}

void	Server::_pingResp( std::vector<std::string> &cmds, int client ){
		std::string respond = "PONG ";

		if (cmds.size() < 2){
			numericReply(&_clients[client], "409", "");
			return ;
		}
		respond += cmds[1] + "\r\n";
		send(_pollFd[client + 1].fd, respond.data(), respond.size(), 0);
		std::cout << "\n>> " << respond;
}

void	Server::_joinResp( std::vector<std::string> &cmds, int client ) {
	if ( !_clients[client].isRegistered() )
		return ;
	if (cmds.size() < 2 || (cmds.size() <  3 && cmds[1].size() < 1
		&& cmds[1][0] == ':' )) {
		numericReply(&_clients[client], "461", "");
		return ;
	}
	std::string ChannelName, key;
	ChannelName =cmds[1].substr(1);
	joinChannel(&_clients[client], ChannelName, cmds[1]);
	/*error code here?*/
}

void	Server::_partResp( std::vector<std::string> &cmds, int client ) {
	std::string	respond;
	
	if ( !_clients[client].isRegistered() )
		return ;

	// std::cout << "check1" << std::endl;
	if (cmds.size() < 2) {
		numericReply(&_clients[client], "461", "");
		return ;
	}
	// std::cout << "check2" << std::endl;

	std::string channelName = cmds[1];
	if (!channelName.empty() && channelName[0] == '#')
		channelName = channelName.substr(1);
	// std::cout << "check3" << std::endl;

	// }

	auto it = _channels.find(channelName);
	if (it == _channels.end()) {
		if (_channels.empty()) {
        // std::cout << "No channels available." << std::endl;
		numericReply(&_clients[client], "403", channelName);
        return;
    	}
	}

    // std::cout << "List of all channels:" << std::endl;
    // for (const auto &entry : _channels) {
    //     std::cout << "- " << entry.first << std::endl; // Print the key (channel name)
    // }
	// std::cout << "check4" << std::endl;

	Channel *channel = it->second;
	if (!channel->userExists(_clients[client].getNickName()))
		numericReply(&_clients[client], "442", channelName);

	channel->removeUser(&_clients[client]);
	respond = "PART #" + channel->getName();
	for (int i = 2; i < cmds.size(); i++){
		respond += " " + cmds[i];
	}
    std::cout << _clients[client].getNickName() << std::endl;
	channel->_broadcast(respond, _clients[client].getNickName());
    /*********************** */
    // Broadcast PART message to other channel members
    /*********************** */
    // If the channel is now empty, delete it
    if (channel->getUsers().empty()) {
        _channels.erase(channelName);
        delete channel;
    }
}

void	Server::_parser( std::vector<std::string> &cmds, int client ){
	if (!_clients[client].isAuthenticated()
			&& cmds[0] != "CAP" && cmds[0] != "PASS" && cmds[0] != "JOIN"){
		numericReply(&_clients[client], "464", "");
		// _removeClient(_clients[client].getFd());
		return ;
	}

	if (cmds[0] == "CAP")
		_capResp(cmds, client);
	else if(cmds[0] == "PASS")
		_passResp(cmds, client);
	else if (cmds[0] == "NICK")
		_nickResp(cmds, client);
	else if (cmds[0] == "USER")
		_userResp(cmds, client);
	else if (cmds[0] == "PING")
		_pingResp(cmds, client);
	else if (cmds[0] == "PRIVMSG")
		_privMsgResp(cmds, client);
	else if (cmds[0] == "JOIN")
		_joinResp(cmds, client);
	else if (cmds[0] == "PART")
		_partResp(cmds, client);
}



void	Server::_serverRespond( int client ){
	std::vector<std::string>	cmds;
	
	cmds = _clients[client].getCommand();

	while (cmds.size()){
		_parser(cmds, client);
		_clients[client].clearBuff();
		cmds = _clients[client].getCommand();
	}
	std::cout << std::endl;
}

