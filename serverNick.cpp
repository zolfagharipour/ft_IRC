#include "ft_irc.h"
#include "server.hpp"


std::string	&Server::_nickLower( std::string& nick ){
	for (int i = 0; i < nick.size(); i++){
		if ((nick[i] >= 'A' && nick[i] <= 'Z') || nick[i] == '[' || nick[i] == ']' || nick[i] == '\\')
			nick[i] += 32;
		else if (nick[i] == '~')
			nick[i] = '^';
	}
	return (nick);
}

bool	Server::_nickValidity( std::string& nick ){
	std::string		specialChr = "{}|^";

	if (nick.size() > 9 || std::isdigit(nick[0]) || nick[0] == '-')
		return 0;
	for (int i = 0; i < nick.size(); i++){
		if (!std::isalpha(nick[i]) && specialChr.find(nick[i]) == std::string::npos
				&& nick[i] != '-')
			return 0;
	}
	return 1;
}

void	Server::_nickResp( std::vector<std::string> &cmds, int client ){
	std::string	respond, nick;

	if (cmds.size() < 2){
		_numericReply(client, "431", "");
		// if (!_clients[client].isRegistered())
		// 	_removeClient(_clients[client].getFd());
		return ;
	}
	if (!_nickValidity(cmds[1])){
		_numericReply(client, "432", "");
		// if (!_clients[client].isRegistered())
		// 	_removeClient(_clients[client].getFd());
		return ;
	}
	nick = _nickLower(cmds[1]);
	
	for (int i = 0; i < _clients.size(); i++){
		if (_clients[i].getNickName() == nick){
			_numericReply(client, "433", "");
			// if (!_clients[client].isRegistered())
			// 	_removeClient(_clients[client].getFd());
			return ;
		}
	}
	_clients[client].setNickName(nick);
}