#include "ft_irc.h"
#include "server.hpp"


std::string	&Server::_lowerCase( std::string& nick ){
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
		numericReply(_clients[client], "431", "", "", "");
		return ;
	}
	if (!_nickValidity(cmds[1])){
		numericReply(_clients[client], "432", "", "", cmds[1]);
		return ;
	}
	nick = _lowerCase(cmds[1]);
	
	for (int i = 0; i < _clients.size(); i++){
		if (_clients[i]->getNickName() == nick){
			numericReply(_clients[client], "433", "", "", nick);
			return ;
		}
	}
	_clients[client]->setNickName(nick);
}