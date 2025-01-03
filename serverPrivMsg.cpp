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
	std::string		senderNick = _clients[client].getNickName();
	
	if (it != _channels.end())
		channel = it->second;
	else{
		_numericReply(&_clients[client], "403", chName);
		return ;
	}
	if (!channel->isUserInChannel(senderNick)){
		_numericReply(&_clients[client], "404", chName);
		return ;
	}
	for (int i = 0; i < _clients.size(); i++){
		clieName = _clients[i].getNickName();
		if (client != i && channel->isUserInChannel(clieName)){
			respond = ":" + senderNick + "!" + _clients[i].getUserName() + 
					" PRIVMSG #" + chName;
			for (int i = 2; i < cmds.size(); i++){
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

// void	Channel::_broadcast( std::string message, std::string senderNick ){
// 	std::map<std::string, Client*>::iterator it = _users.find(senderNick);
// 	std::string		respond = ":" + senderNick + "!";
// 	Client			*sender;
	
// 	if (it != _users.end()){
// 		sender = it->second;
// 		respond += sender->getUserName() + " " + message + "\r\n";
// 	}
// 	if (!isUserInChannel(senderNick)){
// 		_numericReply(sender, "404", getName());
// 		return ;
// 	}
// 	for (std::map<std::string, Client*>::iterator it = _users.begin(); it != _users.end(); ++it) {
// 		if (it->first != senderNick)
// 			send(it->second->getFd(), respond.data(), respond.size(), 0);
// 	}
// }