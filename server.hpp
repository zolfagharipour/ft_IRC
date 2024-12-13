#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.h"
# include "client.hpp"
# include "channel.hpp"
# include <netinet/in.h>
# include <sys/poll.h>

class Server
{
private:
	int									_serFd;
	const int							_port;
	static int							_signal;
	std::vector<Client>					_clients;
	std::vector<struct pollfd>			_pollFd;
	std::map<std::string, Channel *>	_channels; //map of channel name to channel

	
	/*constructor add*/
	// std::set<Channel *>				_channels;

	void		_clientRegister();
	void		_clientCommunicate( size_t i );
	void		_closeFds();
	void		_removeClient( int fd );
	
	void		_SocketInit();
public:
	Server();
	Server( int port );
	~Server();
	
	void		ServerInit();
	void		signalMonitor();
	static void	handler( int signum );

	Channel *getChannel( std::string channelName);

	//channel management
	void		joinChannel( Client &client, const std::string &channelName, std::string key );
	void		leaveChannel( Client &client, const std::string &channelName );
	void		addChannel( Channel *);
	void		addChannel( std::string name );

	//debug
	void		printChannels();
};

#endif