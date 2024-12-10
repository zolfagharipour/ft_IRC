#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.h"
#include "channel.hpp"

class Client //-> class for client
{
	private:
		int Fd; //-> client file descriptor
		std::string IPadd; //-> client ip address

		
		std::string					_nickName;
		std::string 				_userName;
		bool						_authenticated;
		
		std::map<Channel *, bool>	_opStatus; //key = channel name, value: if client is channel op
		std::set<Channel *>			_channels; //which channels the client joined

	public:
		Client(); //-> default constructor
		Client(int fd, const std::string &ipAddress);
		
		int GetFd() {return Fd;} //-> getter for fd
		std::string &getNickName() {return _nickName; };
		std::string &getUserName() {return _userName; };
		// std::string &getIPAdress() {return IPadd; };
		bool		isAuthenticated() {return _authenticated; }; //for each join etc command authentication needs to be checked

		void setFd(int fd){Fd = fd;} //-> setter for fd
		void setIpAdd(std::string ipadd){IPadd = ipadd;} //-> setter for ipadd
		// void setAuthenticated(bool status) {_authenticated = status; };
		
		void	joinChannel(Channel *channel);
		void	leaveChannel(Channel *channel);
		bool	isInChannel(Channel *channel);

		void	setOPStatus(Channel *channel, bool status);
		bool	isOP(Channel *channel);
};

#endif