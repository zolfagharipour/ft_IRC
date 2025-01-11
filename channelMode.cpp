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

void    Channel::setUserLimit( Client *client, std::vector<std::string> &cmds ) {
    std::string clientName = client->getNickName();
    std::string channelName = _name;

    if (!hasPersmission(client)) {
        _server->numericReply(client, "482", _name);
        return ;
    }

    if (cmds.size() < 4) {
        _server->numericReply(client, "461", _name);
        return ;
    }

    int limit;
    try {
        limit = std::stoi(cmds[3]);
        if (limit <= 0)
            throw std::out_of_range("too small user limit");
    } catch (const std::exception &e) {
        return ;
    }
 
    _userLimit = limit;
    _userLimitRestricted = true;
    _broadcast("MODE #" + channelName + " +l " + std::to_string(limit), clientName, true);
}

void    Channel::removeUserLimit( Client *client ) {
    std::string clientName = client->getNickName();
    std::string channelName = _name;

    if (!hasPersmission(client)){
        this->_server->numericReply(client, "482", this->_name);
        return ;
    }

    _userLimitRestricted = false;
    _broadcast("MODE #" + channelName + " -l", clientName, true);
}

void    Channel::setTopic( Client *client, const std::string &topic ) {
    if (_topicRestricted && !hasPersmission(client)) {
        this->_server->numericReply(client, "482", this->_name);
        return ;
    }
    if (_topic == topic)
        return ;

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
void    Channel::kickUser( Client *sourceClient, Client *targetClient ) {
    if (!hasPersmission(sourceClient)) {
        std::cerr << sourceClient->getNickName() << ": no permission to kick users from channel: " << this->getName() << std::endl;
        return ;
    }
    if (!this->isUserInChannel(targetClient->getNickName())) {
        std::cerr << targetClient->getNickName() << ": is not in channel: " << this->getName() << std::endl;
        return ;
    }
    this->removeUser(targetClient, "KICK", true);
    std::cout << targetClient->getNickName() << ": has been kicked from channel: " << this->getName() << std::endl;
}