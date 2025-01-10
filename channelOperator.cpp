#include "channel.hpp"

void    Channel::addOperator(Client *client) {
    if (_users.find(client->getNickName()) != _users.end()) {
        _operators.insert(client);
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