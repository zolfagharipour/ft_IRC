#include "channel.hpp"
#include "server.hpp"

Channel::Channel( const std::string &name, Server* server ) :
		_name(name), _inviteOnly(false), _userLimit(-1), _botActivated(false), _topicRestricted(false), _server(server) {
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

bool    Channel::isBotActivated(){
    return _botActivated;
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
    if (_inviteOnly && !hasPersmission(sourceClient)) {
        _server->numericReply(sourceClient, "482", _name, "", "");
        return ;
    }

    if (!isUserInChannel(sourceClient->getNickName())) {
        _server->numericReply(sourceClient, "442", _name, "", "");
        return ;
    }

    if (isUserInChannel(targetClient->getNickName())) {
        _server->numericReply(sourceClient, "443", _name, "", targetClient->getNickName());
        return ;
    }

    //
    if (_userLimitRestricted && _users.size() >= _userLimit) {
        _server->numericReply(sourceClient, "471", this->_name, "", "");
        return ;
    }
	// _users.insert(std::make_pair(targetClient->getNickName(), targetClient));
	
    std::string		respond;
    respond = ":" + sourceClient->getNickName() + "!" + sourceClient->getUserName();
    respond += " INVITE " + targetClient->getNickName() + " :#" + getName();
	respond += "\r\n";
    send(targetClient->getFd(), respond.data(), respond.size(), 0);
    std::cout << "\n>> " << respond;

    _server->numericReply(sourceClient, "341", _name, "", targetClient->getNickName());
	_guestList.insert(targetClient->getNickName());
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
