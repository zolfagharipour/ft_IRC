#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.h"
#include "channel.hpp"

class Channel;

class Client
{
	private:
		int Fd;
		/*do we need the ip adress?*/
		std::string IPadd;

		
		std::string					_nickName;
		std::string 				_userName;
		bool						_authenticated;
		
		/*might be not needed the client does need to know if its operator*/
		std::map<Channel *, bool>	_opStatus; //key = channel name, value: if client is channel op
		
		/*might not be nessesary*/
		std::set<Channel *>			_channels; //which channels the client joined

	public:
		Client();
		Client( int fd, const std::string &ipAddress, std::string nickname, std::string username );
		
		int getFd();
		std::string &getNickName();
		std::string &getUserName();
		// std::string &getIPAdress() {return IPadd; };
		bool		isAuthenticated();

		void setFd( int fd );
		void setIpAdd( std::string ipadd );
		// void setAuthenticated(bool status) {_authenticated = status; };
		
		/*SHOULD BE A SERVER FUNCTION*/
		void	joinChannel( Channel *channel );
		void	leaveChannel( Channel *channel );
		
		/*SHOULD BE A CHANNEL FT*/
		bool	isInChannel( Channel *channel );

		void	setOPStatus( Channel *channel, bool status );
		bool	isOP( Channel *channel );
};

#endif