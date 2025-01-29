#include "channel.hpp"
#include "server.hpp"

Channel::Channel( const std::string &name, Server* server ) :
		_server(server), _botActivated(false), _name(name), _topicRestricted(false), _inviteOnly(false), _userLimit(-1) {
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

Channel::~Channel( ){
	_users.clear();
	_operators.clear();
	_guestList.clear();
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

bool    Channel::isBotActivated( ){
    return _botActivated;
}


const std::string Channel::getKey( ) {
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

void    Channel::setBotActivation( bool status ){
    _botActivated = status;
}

std::string    Channel::censor( std::vector<std::string> &cmds ){
    return _bot.censor(cmds);
}

bool            Channel::shallKick( std::string message ){
    return _bot.shallKick(message);
}

const std::string &Channel::getTopic( ) {
    return _topic;
}
