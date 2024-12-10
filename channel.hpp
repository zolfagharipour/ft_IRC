#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "ft_irc.hpp"
#include "client.hpp"

class Channel {
    private:
        std::string             _name;
        std::string             _topic;
        std::map<std::string, Client*> _users; //key = nickname, value: client pointer
        std::set<Client *>      _operators;
        bool                    _inviteOnly;
        int                     _userLimit;
        std::string             _key;

    public:
        Channel() {};
        //what initiated to? more arguments?
        Channel(const std::string &name);

        //getters
        const std::string             &getName();
        const std::string             &getTopic();
        bool                    getInviteOnly();
        int                     getUserLimit();
        const std::map<std::string, Client*> &getUsers();

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


};

#endif