#include "ft_irc.h"
#include "server.hpp"

void	Server::_sendMsg( std::vector<std::string> &cmds, int sender, int target ){
	std::string respond;

	respond =  ":" + _clients[sender].getNickName() + " PRIVMSG " + _clients[target].getNickName();
	for (int i = 2; i < cmds.size(); i++){
		respond += " " + cmds[i];
	}
	respond += "\r\n";
	send(_pollFd[target + 1].fd, respond.data(), respond.size(), 0);
	std::cout << "\n>> " << respond;
}



void	Server::_privMsgResp( std::vector<std::string> &cmds, int client ){
	std::string	respond = _clients[client].getNickName();

	if (cmds.size() < 3 ||  cmds[2][0] != ':'){
		_numericReply(client, "411", "");
		return ;
	}

	std::string nick = _nickLower(cmds[1]);
	for (int i = 0; i < _clients.size(); i++){
		if (_clients[i].getNickName() == nick){
			_sendMsg(cmds, client, i);
			return ;
		}
	}
	_numericReply(client, "401", "");
}