#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "ft_irc.h"
#include "client.hpp"
#include "bot.hpp"
#include "server.hpp"

class Client;
class Server;
class Bot;

class Channel {
    private:
		Server*							_server;
        Bot                             _bot;
        bool                            _botActivated;
        std::string                     _name;
        std::string                     _topic;
        bool                            _topicRestricted;
        std::map<std::string, Client*>  _users;
        std::set<Client *>              _operators;
        std::set<std::string>              _guestList;
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

        const std::string                       &getName();
        const std::string                       &getTopic();
        bool                                    getInviteOnly();
        int                                     getUserLimit();
        bool                                    getUserLimitStatus();
        bool                                    isBotActivated();
        Client                                  *getOperator();
        const std::map<std::string, Client*>    &getUsers();
        const std::string                       getKey( );

        //User management
        bool    addUser( Client *client);
        void    removeUser( Client *client, std::string message, bool selfEcho );
        void    removeUser( Client *toRemove, Client *kicker, std::string message, bool selfEcho );
        bool    isUserInChannel( std::string &nickname );
        bool    isGuestList( std::string &nickname );
        void    removeFromGuestList( std::string nick );

        //OP MANAGEMENT
        void    addOperator( Client *client, std::string sourceName );
        void    kickUser( Client *sourceClient, Client *targetClient, std::string message );
        void    removeOperator( Client *client, std::string sourceName );
        bool    isOperator( Client *client );
        bool    hasPersmission( Client *client );
        void    inviteUser( Client *sourceClient, Client *targetClient );

        //Mode
        void    setInviteOnly( Client *client, bool inviteOnly );
        void    setUserLimit( Client *client, std::vector<std::string> &cmds );
        void    removeUserLimit( Client *client );
        void    setTopic( Client *client, const std::string &topic );
        void    setTopicRestriction( Client *client, bool status );
        void    changeOperatorPrivilege( Client *sourceClient, bool give, std::vector<std::string> &cmds );
        void    setKey( Client *client, std::string key );
        void    removeKey( Client *client );

        //debug
        void    printUsers();

		// messaging
		void	_broadcast( std::string message, std::string senderName, bool selfEcho );
        
        void    setBotActivation( bool status );
        std::string censor( std::vector<std::string> &cmds );
        bool        shallKick( std::string message );
    };

#endif