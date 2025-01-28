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

bool    Channel::isGuestList(std::string &nickname) {
    if (_guestList.find(nickname) != _guestList.end())
        return true;
    else
        return false;
}

void    Channel::removeFromGuestList(std::string nick) {
	_guestList.erase(nick);
}

bool    Channel::addUser( Client *client ) {
    if (_userLimitRestricted && _users.size() >= _userLimit) {
        _server->numericReply(client, "471", this->_name, "", "");
        return false ;
    }
	_users.insert(std::make_pair(client->getNickName(), client));

	std::string	respond = "JOIN #" + getName();	

	_broadcast(respond, client->getNickName(), true);
    if (!getTopic().empty())
        _server->numericReply(client, "332", _name, "", "");

    return true ;
}

void    Channel::kickUser( Client *sourceClient, Client *targetClient, std::string message ) {
    std::string	respond = "KICK #" + _name + " " + targetClient->getNickName();

	if (!hasPersmission(sourceClient)) {
        _server->numericReply(sourceClient, "482", _name, "", "");
		return ;
    }
    if (!this->isUserInChannel(targetClient->getNickName())) {
        _server->numericReply(sourceClient, "441", _name, "", targetClient->getNickName());
        return ;
    }
	if (message.size())
		respond += message;
	// _broadcast(respond, sourceClient->getNickName(), true)
    removeUser(targetClient, sourceClient, respond, true);
}