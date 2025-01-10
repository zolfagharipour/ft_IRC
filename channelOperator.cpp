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

void    Channel::setKey( Client *client, const std::string &key ) {
    if ( !this->isUserInChannel(client->getNickName( )) ) {
        std::cerr << client->getNickName() << ": is not in channel: " << this->getName() << std::endl;
        return ;
    }
    
    if ( !hasPersmission( client ) ) {
        std::cerr << client->getNickName() << ": no permission to set/remove key in channel: " << this->getName() << std::endl;
        return ;
    }
    this->_key = key;
    if ( key.empty() )
        std::cout << "The channel key for: " << this->getName() << " has been removed by " << client->getNickName() << std::endl;
    else
        std::cout << "The channel key for: " << this->getName() << " has been set by " << client->getNickName() << std::endl;
}