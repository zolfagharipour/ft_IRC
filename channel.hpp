#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "ft_irc.h"
#include "client.hpp"
#include "server.hpp"

class Client;
class Server;

class Channel {
    private:
		Server*							_server;
        std::string                     _name;
        std::string                     _topic;
        bool                            _topicRestricted;
        std::map<std::string, Client*>  _users; //key = nickname, value: client pointer
        std::set<Client *>              _operators;
        bool                            _inviteOnly;
        int                             _userLimit;
        bool                            _userLimitRestricted;
        std::string                     _key;

    public:
        Channel() {};
        Channel( const Channel& other );
        //what initiated to? more arguments?
        Channel(const std::string &name, Server* server );
        Channel& operator=( const Channel& other );
        ~Channel();

        //getters
        const std::string                       &getName();
        const std::string                       &getTopic();
        bool                                    getInviteOnly();
        int                                     getUserLimit();
        Client                                  *getOperator();
        const std::map<std::string, Client*>    &getUsers();
        const std::string                       getKey( );



        //user management
        bool    addUser( Client *client);
        void    removeUser( Client *client, std::string message, bool selfEcho );
        bool    isUserInChannel( std::string &nickname );

        //OP MANAGEMENT
        void    addOperator( Client *client, std::string sourceName );
        void    kickUser( Client *sourceClient, Client *targetClient );
        void    removeOperator( Client *client, std::string sourceName );
        bool    isOperator( Client *client );
        bool    hasPersmission( Client *client );
        void    inviteUser( Client *sourceClient, Client *targetClient );
        void    addUserToChannel( Client *client, Client *targetClient );

        //Mode
        void    setInviteOnly( Client *client, bool inviteOnly );
        void    setUserLimit( Client *client, int max );
        void    setUserLimitRestriction(Client *client, bool status);
        void    setTopic( Client *client, const std::string &topic );
        void    setTopicRestriction( Client *client, bool status );
        void    changeOperatorPrivilege( Client *sourceClient, bool give, std::vector<std::string> &cmds );
        void    setKey( Client *client, std::string key );
        void    removeKey( Client *client );


        
        void    kickUser();
        void    inviteUser();

        //debug
        void    printUsers();


		// messaging
		void	_broadcast( std::string message, std::string senderName, bool selfEcho );

    };

#endif