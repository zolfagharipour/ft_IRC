#include "server.hpp"
#include "ft_irc.h"

void	Server::numericReply( Client *client, std::string numeric,
			 std::string channel, std::string command, std::string nickName ) {
	Channel *channelPtr;
	std::string nick = client->getNickName();
	std::string	respond = ":" + _serverName + " " + numeric + " " + nick + " ";
	std::map<std::string, Channel *>::iterator it = _channels.find(channel);

	if (it != _channels.end()){
		if (numeric == "441" || numeric == "443")
			respond += nickName + " ";
		respond += "#" + channel + " ";
		channelPtr = it->second;
	}
	if (numeric == "341" || numeric == "401" || numeric == "432" || numeric == "433")
		respond += nickName;
	else if (numeric == "461")
		respond += command;
	
	std::map<std::string, std::string>::const_iterator itReply = _numericResponse.find(numeric);
	if (itReply == _numericResponse.end())
		return ;
	respond += itReply->second;

	if (numeric == "332")
		respond += channelPtr->getTopic();
	else if (numeric == "411")
		respond += command;
	respond += "\r\n";

	send(client->getFd(), respond.data(), respond.size(), 0);
	std::cout << "\n>> " << respond;
}
