#include "channel.hpp"

void    Channel::removeUser( Client *client, std::string message, bool selfEcho ) {
	std::string	senderNick = client->getNickName();
	if (!isUserInChannel(senderNick))
		return ;
	if (message.size())
		_broadcast(message, senderNick, selfEcho);

	if (isOperator(client))
		removeOperator(client, "");
    _users.erase(senderNick);
}

void    Channel::removeUser( Client *toRemove, Client *kicker, std::string message, bool selfEcho ){

	std::string	senderNick = kicker->getNickName();
	if (!isUserInChannel(senderNick))
		return ;
	if (isOperator(toRemove))
		removeOperator(toRemove, "");
	if (message.size())
		_broadcast(message, senderNick, selfEcho);
    _users.erase(toRemove->getNickName());
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
    if (!getTopic().empty())
        _server->numericReply(client, "332", _name);

    return true ;
}