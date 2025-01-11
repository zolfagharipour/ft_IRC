#include "channel.hpp"
#include "server.hpp"

Channel::Channel( const std::string &name, Server* server ) :
		_name(name), _inviteOnly(false), _userLimit(-1), _topicRestricted(false), _server(server) {
        _userLimitRestricted = false;
}

Channel::Channel( const Channel& other ){
	*this = other;
}

Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		_server = other._server;
		_name = other._name;
		_topic = other._topic;
		_topicRestricted = other._topicRestricted;
		_inviteOnly = other._inviteOnly;
		_userLimit = other._userLimit;
		_userLimitRestricted = other._userLimitRestricted;
		_key = other._key;

		_users.clear();
		_operators.clear();

		for ( std::map<std::string, Client*>::const_iterator it = other._users.begin(); it != other._users.end(); ++it) {
			_users[it->first] = it->second;
		}
		for (std::set<Client*>::iterator it = other._operators.begin(); it != other._operators.end(); ++it) {
    		_operators.insert(*it);
		}
	}
	return *this;
}

Channel::~Channel(){
	_users.clear();
	_operators.clear();
}

const std::string&  Channel::getName( ){
    return _name;
}

bool Channel::getInviteOnly( ) {
    return _inviteOnly;
}

int Channel::getUserLimit( ) {
    return _userLimit;
}

bool Channel::getUserLimitStatus( ) {
    return _userLimitRestricted;
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
