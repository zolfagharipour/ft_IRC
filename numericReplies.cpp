#include "server.hpp"
#include "ft_irc.h"

void	Server::numericReply( Client *client, std::string numeric, std::string channel ) {
	std::string nick = client->getNickName();

	std::string	respond = ":" + _serverName + " " + numeric + " " + nick;
	
	if (channel.size() > 0) {
		respond += " #" + channel;
	}

	Channel *channelPtr;

	auto it = _channels.find(channel);
	if (it != _channels.end())
		channelPtr = it->second;

	// FUNCTIONPOINTER AND FOR-LOOP
	if (numeric == "001")
		respond += " :Welcome to the MyIrc\r\n";
	else if (numeric == "331")
		respond = respond + " :No topic is set\r\n";
	else if (numeric == "332")
		respond = respond + " :" + channelPtr->getTopic() + "\r\n";
	else if (numeric == "401")
		respond += " :No such nick/channel\r\n";
		// real nickname has to be added
	else if (numeric == "403")
		respond += " :No such channel\r\n";
	else if (numeric == "404")
		respond += " :Cannot send to channel\r\n"; 
	else if (numeric == "409")
		respond += " :No origin specified\r\n";
	else if (numeric == "411")
		respond += " :No recipient given (PRIVMSG)\r\n";
	else if (numeric == "431")
		respond += " :No nickname given\r\n";
	else if (numeric == "432")
		respond += " :Erroneous nickname\r\n";
		// real nickname has to be added
	else if (numeric == "433")
		respond += " :Nickname is already in use\r\n";
		// real nickname has to be added
	else if (numeric == "441")
		respond += " :They aren't on that channel\r\n";
		// real nickname has to be added
	else if (numeric == "442")
		respond += " :Your're not on that channel\r\n";
	else if (numeric == "443")
		respond += " :is already on channel\r\n";
	else if (numeric == "451")
		respond += " :You have not registered\r\n";
	//2 versions of 461 part && pricmsg
	else if (numeric == "461")
		respond += "PRIVMSG :Not enough parameters\r\n";
	else if (numeric == "462")
		respond += " :Unauthorized command (already registered)\r\n";
	else if (numeric == "464")
		respond += " :Password incorrect\r\n";
	else if (numeric == "473")
		respond += " :Cannot join channel (+i)\r\n";
	else if (numeric == "475")
		respond += " :Cannot join channel (+k)\r\n";
	else if (numeric == "479")
		respond += " :Erroneous channel name\r\n";
	else if (numeric == "482")
		respond += " :You're not a channel operator\r\n";

	/*might be wrong*/
	send(client->getFd(), respond.data(), respond.size(), 0);
	
	std::cout << "\n>> " << respond;
}