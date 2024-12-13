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
		std::string	_nickName;
		std::string	_userName;
		bool		_authenticated;

	public:
		Client();
		Client( int fd, const std::string &ipAddress, std::string nickname, std::string username );
		
		int 		GetFd();
		std::string &getNickName();
		std::string &getUserName();
		bool		isAuthenticated();

		void 		setFd( int fd );
		void 		setIpAdd( std::string ipadd );
};

#endif