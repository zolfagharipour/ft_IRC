#include "channel.hpp"

Channel::Channel( const std::string &name ) :
		_name(name), _inviteOnly(false), _userLimit(-1), _topicRestricted(false) {
        _userLimitRestricted = false;
        std::cout << name << ": " << "Channel created!" << std::endl;
}

/*default ocnstructor missing*/
/*destructor missing*/

//getters
const std::string&  Channel::getName( ){
    return _name;
}

bool Channel::getInviteOnly( ) {
    return _inviteOnly;
}

int Channel::getUserLimit( ) {
    return _userLimit;
}

const std::string Channel::getKey() {
    return _key;
}

const std::map<std::string, Client*> &Channel::getUsers( ) {
    return _users;
}

Client *Channel::getOperator( ) {
    if (!_operators.empty())
        return *_operators.begin();
    return NULL;
}

//user management
bool    Channel::addUser(Client *client) {
    if (_userLimit > 0 && _users.size() >= (size_t)_userLimit)
        return false;
        /*how is this supposed to be saved*/
    std::cout << client->getNickName() << ": added to channel: " << this->getName() << std::endl;
    _users[client->getNickName()] = client;
    return true ;
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
        std::cout << client->getNickName() << " is now OP of " << this->getName() << std::endl;
    }
    else
        std::cout << "ERROR: cannot add " << client->getNickName() << " as OP" << std::endl;
}

void    Channel::removeOperator(Client *client) {
    if (_operators.erase(client) > 0)
        std::cout << client->getNickName() << " removed as OP" << std::endl;
    else
        std::cout << "ERROR: cannot remove " << client->getNickName() << " as OP" << std::endl;
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

//Mode
void    Channel::setInviteOnly( Client *client, bool inviteOnly ) {
    if (!hasPersmission(client)){
        std::cerr << client->getNickName() << ": no permission to change invite mode" << std::endl;
        return ;
    }
    _inviteOnly = inviteOnly;
    std::cout << "MODE: Invite only mode in channel " << this->getName() << " has been changed by OP" << std::endl;

}

void    Channel::setUserLimit( Client *client, int max) {
    if (_userLimitRestricted && !hasPersmission(client)) {
        std::cerr << client->getNickName() << " does not have permission to adjust user limit" << std::endl;
        return ;
    }
    if (max >= 0 && max < _users.size())
        std::cerr << "ERROR: user limit cannot be lower than current amount of users" << std::endl;
    else {
        std::cout << "User limit in channel " << this->getName() << " set to " << max << std::endl;
        _userLimit = max;
    }
}

void    Channel::setUserLimitRestriction(Client *client, bool status) {
    if (!hasPersmission(client)){
        std::cerr << client->getNickName() << ": no permission to change userlimit restriction" << std::endl;
        return ;
    }
    _userLimitRestricted = status;
    std::cout << "The userlimit restriction for channel " << this->getName() << " has been changed by OP" << std::endl;
}

void    Channel::setTopic(Client *client, const std::string &topic) {
    if (_topicRestricted && !hasPersmission(client)) {
        std::cerr << client->getNickName() << ": no permission to change topic" << std::endl;
        return ;
    }
    _topic = topic;
    std::cout << "The topic for channel " << this->getName() << " has been changed by " << client->getNickName() << std::endl;
}

void    Channel::setTopicRestriction(Client *client, bool status) {
    if (!hasPersmission(client)){
        std::cerr << client->getNickName() << " does not have permission to change topic restriction" << std::endl;
        return ;
    }
    _topicRestricted = status;
    std::cout << "The topic restriction for channel " << this->getName() << " has been changed by OP" << std::endl;
}


const std::string &Channel::getTopic() {
    static const std::string tp = "no topic set";
    if (_topic.empty())
        return tp;
    return _topic;
}

void    Channel::changeOperatorPrivilege(Client *sourceClient, Client *targetClient, bool give) {
    if (!hasPersmission(sourceClient)) {
        std::cerr << sourceClient->getNickName() << ": no permission to moddify operator priviledges in channel: " << this->getName() << std::endl;
        return ;
    }
    if (!this->isUserInChannel(targetClient->getNickName())) {
        std::cerr << targetClient->getNickName() << " is not in channel: " << this->getName() << std::endl;
        return ;
    }
    if (give) {
        if (_operators.insert(targetClient).second)
            std::cout << targetClient->getNickName() << " has been promoted to operator in channel: " << this->getName() << std::endl;
        else
            std::cout << targetClient->getNickName() << " is already operator in channel: " << this->getName() << std::endl;
    }
    else if (!give) {
        if (this->isOperator(targetClient)) {
            this->removeOperator(targetClient);
            std::cout << targetClient->getNickName() << " has been demoted from operator in channel: " << this->getName() << std::endl;
        }
        else
            std::cout << targetClient->getNickName() << " is no operator and cannot be demoted in channel: " << this->getName() << std::endl;
    }
}

void    Channel::kickUser( Client *sourceClient, Client *targetClient ) {
    if (!hasPersmission(sourceClient)) {
        std::cerr << sourceClient->getNickName() << ": no permission to kick users from channel: " << this->getName() << std::endl;
        return ;
    }
    if (!this->isUserInChannel(targetClient->getNickName())) {
        std::cerr << targetClient->getNickName() << ": is not in channel: " << this->getName() << std::endl;
        return ;
    }
    if (this->isOperator(targetClient))
        this->removeOperator(targetClient);
    this->removeUser(targetClient);
    std::cout << targetClient->getNickName() << ": has been kicked from channel: " << this->getName() << std::endl;

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

void    Channel::inviteUser( Client *sourceClient, Client *targetClient ) {
    if (!hasPersmission(sourceClient)) {
        std::cerr << sourceClient->getNickName() << ": no permission to invite users to channel: " << this->getName() << std::endl;
        return ;
    }

    if (this->isUserInChannel(targetClient->getNickName())) {
        std::cerr << targetClient->getNickName() << ": is already in channel: " << this->getName() << std::endl;
        return ;
    }
    this->addUser(targetClient);
}

bool    Channel::userExists( const std::string &nickName ) {
    if (_users.find(nickName)!= _users.end())
        return true;
    else
        return false;
}

void    Channel::printUsers() {
    std::cout << "Clients in channel " << this->_name << ":\t";
    for (std::map<std::string, Client*>::const_iterator it = _users.begin(); it != _users.end(); ++it) {
        std::cout << it->first;
        if (this->isOperator(it->second))
            std::cout << "-(OP)";
        std::cout << " ";
    }
    std::cout << std::endl;
}
