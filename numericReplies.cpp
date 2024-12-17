#include "server.hpp"
#include "ft_irc.h"

void	Server::_numericReply( int client, std::string numeric, std::string channel ){
	std::string nick = _clients[client].getNickName();
	std::string	respond = ":" + _serverName + " " + numeric + " " + nick + " ";
	
	if (channel.size() < 0)
		nick = "#" + channel;




	// FUNCTIONPOINTER AND FOR LOOP
	if (numeric == "001")
		respond += " :Welcome to the MyIrc\r\n";
	else if (numeric == "409")
		respond += " :No origin specified\r\n";
	else if (numeric == "401")
		respond += " :No such nick/channel\r\n";
		// real nickname has to be added
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
	else if (numeric == "451")
		respond += " :You have not registered\r\n";
	else if (numeric == "461")
		respond += " PRIVMSG :Not enough parameters\r\n";
	else if (numeric == "462")
		respond += " :Unauthorized command (already registered)\r\n";
	else if (numeric == "464")
		respond += " :Password incorrect\r\n";



	send(_pollFd[client + 1].fd, respond.data(), respond.size(), 0);
	
	std::cout << "\n>> " << respond;

}