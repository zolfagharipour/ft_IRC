#include "ft_irc.h"
#include "server.hpp"

void	Server::_sendMsg( std::vector<std::string> &cmds, int sender, int target ){
	std::string respond;

	respond =  ":" + _clients[sender]->getNickName() + " PRIVMSG " + _clients[target]->getNickName();
	for (int i = 2; i < cmds.size(); i++){
		respond += " " + cmds[i];
	}
	respond += "\r\n";
	send(_pollFd[target + 1].fd, respond.data(), respond.size(), 0);
	std::cout << "\n>> " << respond;
}

void	Server::_privMsgResp( std::vector<std::string> &cmds, int client ){
	std::map<std::string, Channel*>::iterator 	it;
	Channel		*channel;
	std::string	respond;

	if (cmds.size() < 3 ||  cmds[2][0] != ':'){
		numericReply(_clients[client], "411", "");
		return ;
	}
	if (cmds[1][0] == '#'){
		std::string	chName = cmds[1].substr(1);
		it = _channels.find(chName);
		if (it != _channels.end())
			channel = it->second;
		else{
			numericReply(_clients[client], "403", chName);
			return ;
		}
		respond =  " PRIVMSG #" + chName;
		for (int i = 2; i < cmds.size(); i++){
			respond += " " + cmds[i];
		}
		channel->_broadcast(respond, _clients[client]->getNickName(), false);
		return ;
	}
	std::string nick = _lowerCase(cmds[1]);
	for (int i = 0; i < _clients.size(); i++){
		if (_clients[i]->getNickName() == nick){
			_sendMsg(cmds, client, i);
			return ;
		}
	}
	numericReply(_clients[client], "401", "");
}
