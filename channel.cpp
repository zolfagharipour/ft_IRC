#include "channel.hpp"

Channel::Channel(std::string &name) : {
    _name = name;
    _inviteOnly = false;
    _userLimit = -1;
}

/*default ocnstructor missing*/
/*destructor missing*/

//getters
const std::string Channel::getName() {
    return _name;
}

const std::string Channel::getTopic() {
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

std::map<std::string, Client*> _users; //key = nickname, value: client pointer
        std::set<Client *>      _operators;
        bool                    _inviteOnly;
        int                     _userLimit;
        std::string             _key;


        //void  setTopic(const std::string &topic);

        //user management
        bool    addUser(Client *client);
        void    removeUser(Client *client);
        bool    isUserInChannel(std::string &nickname);

        //OP MANAGEMENT
        void    addOperator(Client *client);
        void    removeOperator(Client *client);
        bool    isOperator(Client *client);

        //Mode
        void    setInviteOnly(bool inviteOnly);
        void    setUserLimit(int max);