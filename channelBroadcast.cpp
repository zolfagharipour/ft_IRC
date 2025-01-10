#include "channel.hpp"

void	Channel::_broadcast( std::string message, std::string senderNick, bool selfEcho ){
	std::map<std::string, Client*>::iterator it = _users.find(senderNick);
	std::string		respond;
	Client			*sender;
	Server			server;
	
	if (it != _users.end()){
		sender = it->second;
		respond = ":" + senderNick + "!" + sender->getUserName();
	}
	else if (senderNick == ""){
		respond = ":" + _server->getName();
	}
	else if (!isUserInChannel(senderNick)){
		server.numericReply(_server->getClient(senderNick), "404", getName());
		return ;
	}

	if (message.size())
		respond += " " + message;
	respond += "\r\n";
	
	if (selfEcho){
		for (std::map<std::string, Client*>::iterator itt = _users.begin(); itt != _users.end(); ++itt) {
			Client*	receiver = itt->second;
			send(receiver->getFd(), respond.data(), respond.size(), 0);
            std::cout << "\n>> " << respond;
		}
	}
	else{
		for (std::map<std::string, Client*>::iterator itt = _users.begin(); itt != _users.end(); ++itt) {
			Client*	receiver = itt->second;
			if (receiver->getNickName() != senderNick){
				send(receiver->getFd(), respond.data(), respond.size(), 0);
	            std::cout << "\n>> " << respond;
			}
		}
	}
}