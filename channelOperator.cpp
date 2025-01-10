#include "channel.hpp"

void    Channel::addOperator(Client *client) {
    if (_users.find(client->getNickName()) != _users.end()){
		std::string	respond = "MODE #" + _name + " +o " + client->getNickName();
        _operators.insert(client);
		_broadcast(respond, "", true);
	}
    
}

void    Channel::removeOperator(Client *client) {
	std::string	leaveRespond = "MODE #" + _name + " -o " + client->getNickName();
	_broadcast(leaveRespond, "", true);
	_operators.erase(client);

	if (!getOperator()){
		std::map<std::string, Client*>::const_iterator	users = getUsers().begin();
		for (; users != getUsers().end(); ++users){
			if (users->second != client){
				addOperator(users->second);
				return ;
			}
		}
	}
}

bool    Channel::isOperator(Client *client) {
    if (_operators.find(client) != _operators.end())
        return true;
    else
        return false;
}

bool    Channel::hasPersmission(Client *client) {
    if (_operators.find(client) != _operators.end())
        return true;
    else
        return false;
}