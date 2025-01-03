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

void	Server::_broadcast( std::vector<std::string> &cmds, std::string chName, int client ){
	std::map<std::string, Channel*>::iterator it = _channels.find(chName);
	Channel			*channel;
	std::string		respond;
	std::string		clieName;
	std::string		senderName = _clients[client].getNickName();
	
	if (it != _channels.end())
		channel = it->second;
	else
		return ;
	// check if channel name doesnt exist
	for (int i = 0; i < _clients.size(); i++){
		clieName = _clients[i].getNickName();
		if (client != i && channel->isUserInChannel(clieName)){
			respond = ":" + senderName + "!" + _clients[i].getUserName() + 
					" PRIVMSG #" + chName + " " + cmds[2];
			for (int i = 3; i < cmds.size(); i++){
				respond += " " + cmds[i];
			}
			respond += "\r\n";
			send(_pollFd[i + 1].fd, respond.data(), respond.size(), 0);
			std::cout << "\n>> " << respond;
		}
	}
}



void	Server::_privMsgResp( std::vector<std::string> &cmds, int client ){
	std::string	respond = _clients[client].getNickName();

	if (cmds.size() < 3 ||  cmds[2][0] != ':'){
		_numericReply(&_clients[client], "411", "");
		return ;
	}
	if (cmds[1][0] == '#'){
		std::string	chName = cmds[1].substr(1);
		_broadcast(cmds, chName, client);
		return ;
	}
	std::string nick = _nickLower(cmds[1]);
	for (int i = 0; i < _clients.size(); i++){
		if (_clients[i].getNickName() == nick){
			_sendMsg(cmds, client, i);
			return ;
		}
	}
	_numericReply(&_clients[client], "401", "");
}