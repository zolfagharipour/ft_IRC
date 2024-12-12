#include "channel.hpp"

Channel::Channel( const std::string &name ) :
		_name(name), _inviteOnly(false), _userLimit(-1) {
        std::cout << name << ": " << "Channel created!" << std::endl;
}

/*default ocnstructor missing*/
/*destructor missing*/

//getters
const std::string&  Channel::getName(){
    return _name;
}

const std::string&  Channel::getTopic() {
    return _topic;
}

bool Channel::getInviteOnly() {
    return _inviteOnly;
}

int Channel::getUserLimit() {
    return _userLimit;
}

const std::map<std::string, Client*> &Channel::getUsers() {
    return _users;
}


//user management
bool    Channel::addUser(Client *client) {
    if (_userLimit > 0 && _users.size() >= _userLimit)
        return false;
        /*how is this supposed to be saved*/
    std::cout << client->getNickName() << ": added to channel: " << this->getName() << std::endl;
    _users[client->getNickName()] = client;
    return true;
}

void    Channel::removeUser(Client *client) {
    _users.erase(client->getNickName());
}

bool    Channel::isUserInChannel(std::string &nickname) {
    if (_users.find(nickname) != _users.end())
        return true;
    else
        return false;
}

//OP MANAGEMENT
void    Channel::addOperator(Client *client) {
    if (_users.find(client->getNickName()) != _users.end()) {
        _operators.insert(client);
        client->setOPStatus(this, true);
    }
}

void    Channel::removeOperator(Client *client) {
    /*dont't i need to set the operator bool in client to false at the same time?*/
        if (_users.find(client->getNickName()) != _users.end()) {
        _operators.erase(client);
        client->setOPStatus(this, false);
    }
}

bool    Channel::isOperator(Client *client) {
    if (_operators.find(client) != _operators.end())
        return true;
    else
        return false;
}

//Mode
void    Channel::setInviteOnly(bool inviteOnly) {
    _inviteOnly = inviteOnly;
}

void    Channel::setUserLimit(int max) {
    if (max >= 0 && max < _users.size())
        std::cerr << "ERROR: user limit cannot be lower than current amount of users" << std::endl;
    else
        _userLimit = max;
}