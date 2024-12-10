#include "ft_irc.hpp"

class Client //-> class for client
{
	private:
		int Fd; //-> client file descriptor
		std::string IPadd; //-> client ip address

		
		std::string	_nickName;
		std::string _userName;
		// ??std::string _password;
		bool		_authenticated;
		
		std::map<std::string, bool>	_opStatus; //key = channel name, value: if client is channel op
		std::set<std::string>		_channels; //which channels the client joined
		
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
		
		void	joinChannel(std::string &channel);
		void	leaveChannel(std::string &channel);
		bool	isInChannel(std::string &channel);

		void	setOPStatus(std::string &channel, bool status);
		bool	isOP(std::string &channel);
};