#include "channel.hpp"

void    Channel::removeUser(Client *client) {
    _users.erase(client->getNickName());
}

bool    Channel::isUserInChannel(std::string &nickname) {
    if (_users.find(nickname) != _users.end())
        return true;
    else
        return false;
}


bool    Channel::addUser( Client *client ) {
    if (_userLimit > 0 && _users.size() >= _userLimit) {
        _server->numericReply(client, "471", this->_name);
        // std::cerr << "ERROR: user limit reached in channel cannot add: " << client->getNickName() << std::endl;
        return false ;
    }
	_users.insert(std::make_pair(client->getNickName(), client));

	std::string	respond = "JOIN #" + getName();	
	
	_broadcast(respond, client->getNickName(), true);
    // _users[client->getNickName()] = client;
    return true ;
}