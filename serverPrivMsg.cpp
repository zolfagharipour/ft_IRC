#include "ft_irc.h"
#include "server.hpp"

void	Server::_sendMsg( std::vector<std::string> &cmds, int sender, int target ){
	std::string respond;

	respond =  ":" + _clients[sender]->getNickName() + " PRIVMSG " + _clients[target]->getNickName();
	for (size_t i = 2; i < cmds.size(); i++){
		respond += " " + cmds[i];
	}
	respond += "\r\n";
	send(_pollFd[target + 1].fd, respond.data(), respond.size(), 0);
	std::cout << "\n>> " << respond;
}

bool	Server::_botActivation( int client, Channel& channel, std::string message ){
	std::string	respond;

	if (message.find("bot") != std::string::npos){
		if (!channel.isOperator(_clients[client])){
			numericReply(_clients[client], "482", channel.getName(), "", "");
			return true;
		}
		if (message.find("bot off") != std::string::npos){
			channel.setBotActivation(false);
			respond = " PRIVMSG #" + channel.getName() + " :BigBrother stop watching this channel";
		}
		else if (message.find("bot on") != std::string::npos){
			channel.setBotActivation(true);
			respond = " PRIVMSG #" + channel.getName() + " :BigBrother is watching this channel";
		}
		channel._broadcast(respond, "", true);
		return true;
	}
	return false;
}

void	Server::_privMsgResp( std::vector<std::string> &cmds, int client ){
	std::map<std::string, Channel*>::iterator 	it;
	Channel		*channel;
	std::string	respond;

	if (cmds.size() < 3 ||  cmds[2][0] != ':'){
		numericReply(_clients[client], "411", "", "PRIVMSG", "");
		return ;
	}
	if (cmds[1][0] == '#'){
		std::string	chName = cmds[1].substr(1);
		it = _channels.find(chName);
		if (it != _channels.end())
			channel = it->second;
		else{
			numericReply(_clients[client], "403", chName, "", "");
			return ;
		}
		respond = " PRIVMSG #" + chName;
		if (!channel->isBotActivated()){
			for (size_t i = 2; i < cmds.size(); i++){
				respond += " " + cmds[i];
			}
		}
		else{
			std::string afterCensor = channel->censor(cmds);
			if (channel->shallKick(afterCensor)){
				std::string	message = "PART #" + channel->getName() + " :Comrade has left the channel on his own will";
				channel->removeUser(_clients[client], message, true);
				return ;
			}
			respond += afterCensor;
		}
		if (!_botActivation(client, *channel, respond))
			channel->_broadcast(respond, _clients[client]->getNickName(), false);
		return ;
	}
	std::string nick = _lowerCase(cmds[1]);
	for (size_t i = 0; i < _clients.size(); i++){
		if (_clients[i]->getNickName() == nick){
			_sendMsg(cmds, client, i);
			return ;
		}
	}
	numericReply(_clients[client], "401", "", "", nick);
}
