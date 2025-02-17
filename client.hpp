#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.h"
# include "channel.hpp"

class Channel;

class Client
{
	private:
		int 						Fd;
		std::string					_nickName;
		std::string					_userName;
		std::string					_realName;
		std::string					_buff;
		std::vector<std::string>	_vectorCMD;
		bool						_authenticated;
		bool						_registered;


	public:
		Client( );
		Client( const Client& otehr );
		Client&	operator=( const Client& otehr );
		~Client( );

		int getFd( );
		std::string &getNickName( );
		std::string &getUserName( );
		std::string &getRealName( );
		bool		isAuthenticated( );
		bool		isRegistered( );

		void 		setFd( int fd );
		void		setNickName( std::string nick );
		void		setUserName( std::string nick );
		void		setRealName( std::string nick );
		void		authenticate();
		void		registered();

		void						addBuff( std::string buff );
		void						clearBuff();
		std::string					&getBuff();
		std::vector<std::string>	&getCommand();
};

#endif