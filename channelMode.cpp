#include "channel.hpp"

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