#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "ft_irc.h"
#include "client.hpp"
#include "server.hpp"

class Client;

class Channel {
    private:
        std::string                     _name;
        std::string                     _topic;
        bool                            _topicRestricted;
        std::map<std::string, Client*>  _users; //key = nickname, value: client pointer
        std::set<Client *>              _operators;
        bool                            _inviteOnly;
        int                             _userLimit;
        bool                            _userLimitRestricted;
        std::string                     _key;
        std::string                     _serverName;

    public:
        Channel() {};
        //what initiated to? more arguments?
        Channel(const std::string &name, std::string serverName );

        //getters
        const std::string                       &getName();
        const std::string                       &getTopic();
        bool                                    getInviteOnly();
        int                                     getUserLimit();
        Client                                  *getOperator();
        const std::map<std::string, Client*>    &getUsers();
        const std::string                       getKey( );



        //user management
        bool    userExists(const std::string &nickName);
        bool    addUser( Client *client);
        void    removeUser( Client *client );
        bool    isUserInChannel( std::string &nickname );

        //OP MANAGEMENT
        void    addOperator( Client *client );
        void    kickUser( Client *sourceClient, Client *targetClient );
        void    removeOperator( Client *client );
        bool    isOperator( Client *client );
        bool    hasPersmission( Client *client );
        void    inviteUser( Client *sourceClient, Client *targetClient );
        void    addUserToChannel( Client *client, Client *targetClient );
        void    setKey( Client *client, const std::string &key );

        //Mode
        void    setInviteOnly( Client *client, bool inviteOnly );
        void    setUserLimit( Client *client, int max );
        void    setUserLimitRestriction(Client *client, bool status);
        void    setTopic( Client *client, const std::string &topic );
        void    setTopicRestriction( Client *client, bool status );
        void    changeOperatorPrivilege( Client *sourceClient, Client *targetClient, bool give );

        
        void    kickUser();
        void    inviteUser();

        //debug
        void    printUsers();


		// messaging
		void	_broadcast( std::string message, std::string senderName );
	    void	numericReply( Client *client, std::string numeric, std::string channel );

    };

#endif