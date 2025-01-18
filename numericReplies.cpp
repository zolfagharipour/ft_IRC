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













// OLD VERSION


	// // FUNCTIONPOINTER AND FOR-LOOP
	// if (numeric == "001")
	// 	respond += ":Welcome to the MyIrc\r\n";
	// else if (numeric == "331")
	// 	respond += ":No topic is set\r\n";
	// else if (numeric == "332")
	// 	respond += ":" + channelPtr->getTopic() + "\r\n";
	// else if (numeric == "341")
	// 	respond += nickName + "\r\n";
	// 	//
	// else if (numeric == "401")
	// 	respond += nickName + " :No such nick/channel\r\n";
	// 	//
	// else if (numeric == "403")
	// 	respond += ":No such channel\r\n";
	// else if (numeric == "404")
	// 	respond += ":Cannot send to channel\r\n"; 
	// else if (numeric == "409")
	// 	respond += ":No origin specified\r\n";
	// else if (numeric == "411")
	// 	respond += ":No recipient given " + command + "\r\n";
	// 	// 
	// else if (numeric == "431")
	// 	respond += ":No nickname given\r\n";
	// else if (numeric == "432")
	// 	respond += nickName + " :Erroneous nickname\r\n";
	// 	//
	// else if (numeric == "433")
	// 	respond += nickName + " :Nickname is already in use\r\n";
	// 	//
	// else if (numeric == "441")
	// 	respond += ":They aren't on that channel\r\n";
	// 	// 
	// else if (numeric == "442")
	// 	respond += ":Your're not on that channel\r\n";
	// else if (numeric == "443")
	// 	respond += ":is already on channel\r\n";
	// 	// 
	// else if (numeric == "451")
	// 	respond += ":You have not registered\r\n";
	// else if (numeric == "461")
	// 	respond += command + " :Not enough parameters\r\n";
	// //
	// else if (numeric == "462")
	// 	respond += ":Unauthorized command (already registered)\r\n";
	// else if (numeric == "464")
	// 	respond += ":Password incorrect\r\n";
	// else if (numeric == "471")
	// 	respond += ":Cannot join channel (+l)\r\n";
	// else if (numeric == "473")
	// 	respond += ":Cannot join channel (+i)\r\n";
	// else if (numeric == "475")
	// 	respond += ":Cannot join channel (+k)\r\n";
	// else if (numeric == "479")
	// 	respond += ":Erroneous channel name\r\n";
	// else if (numeric == "482")
	// 	respond += ":You're not a channel operator\r\n";