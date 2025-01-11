#include "channel.hpp"

void    Channel::setInviteOnly( Client *client, bool inviteOnly ) {
    std::string clientName = client->getNickName();
    std::string channelName = _name;

    if (!hasPersmission(client)){
        this->_server->numericReply(client, "482", this->_name);
        return ;
    }
    _inviteOnly = inviteOnly;
    if (inviteOnly)
        _broadcast("MODE #" + channelName + " +i", clientName, true);
    else
        _broadcast("MODE #" + channelName + " -i", clientName, true);
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

void    Channel::setUserLimitRestriction( Client *client, bool status ) {
    if (!hasPersmission(client)){
        std::cerr << client->getNickName() << ": no permission to change userlimit restriction" << std::endl;
        return ;
    }
    _userLimitRestricted = status;
    std::cout << "The userlimit restriction for channel " << this->getName() << " has been changed by OP" << std::endl;
}

void    Channel::setTopic( Client *client, const std::string &topic ) {
    if (_topicRestricted && !hasPersmission(client)) {
        this->_server->numericReply(client, "482", this->_name);
        return ;
    }
    _topic = topic;
    _broadcast("TOPIC #" + _name + " :" + topic, client->getNickName(), true);
}

void    Channel::setKey( Client *client, std::string key ) {
    std::string clientName = client->getNickName();
    std::string channelName = _name;

    if (!hasPersmission(client)) {
        this->_server->numericReply(client, "482", this->_name);
        return ;
    }
    _key = key;
    _broadcast("MODE #" + channelName + " +k", client->getNickName(), true);
}

void    Channel::removeKey( Client *client ) {
    std::string clientName = client->getNickName();
    std::string channelName = _name;

    if (!hasPersmission(client)) {
        this->_server->numericReply(client, "482", this->_name);
        return ;
    }
    _key.clear();
    _broadcast("MODE #" + channelName + " -k", client->getNickName(), true);
}

void    Channel::setTopicRestriction( Client *client, bool status ) {
    std::string clientName = client->getNickName();
    std::string channelName = _name;
    
    if (!hasPersmission(client)) {
        this->_server->numericReply(client, "482", this->_name);
        return ;
    }
    _topicRestricted = status;
    if (status)
        _broadcast("MODE #" + channelName + " +t", clientName, true);
    else
        _broadcast("MODE #" + channelName + " -t", clientName, true);
    
}

const std::string &Channel::getTopic() {
    return _topic;
}

void    Channel::changeOperatorPrivilege( Client *sourceClient, bool give, std::vector<std::string> &cmds ) {

    std::map<std::string, Client *>::iterator it;
    std::map<std::string, Client *>::iterator it2;


    if (!hasPersmission(sourceClient)) {
        this->_server->numericReply(sourceClient, "482", this->_name);
        return ;
    }

    for (size_t i = 3; i < cmds.size(); ++i) {
        std::string nickName = cmds[i];

        if (!_server->getClient(nickName))
            _server->numericReply(sourceClient, "401", "");
        
        it = _users.find(nickName);
        if (it == _users.end()) {
            _server->numericReply(sourceClient, "441", this->_name);
            continue ;
        }
        Client *client = it->second;
        std::string clientName = client->getNickName();
        std::string channelName = _name;

        if (give)
            addOperator(client, sourceClient->getNickName());
        else if (!give)
            removeOperator(client, sourceClient->getNickName());
    }
}

/*not mode, change file*/
void    Channel::kickUser( Client *sourceClient, Client *targetClient, std::string message ) {
    std::string	respond = "KICK #" + _name + " " + targetClient->getNickName();

	if (!hasPersmission(sourceClient)) {
        _server->numericReply(sourceClient, "482", _name);
		return ;
    }
    if (!this->isUserInChannel(targetClient->getNickName())) {
        _server->numericReply(sourceClient, "441", _name);
        return ;
    }
	if (message.size())
		respond += message;
	// _broadcast(respond, sourceClient->getNickName(), true)
    removeUser(targetClient, sourceClient, respond, true);
}