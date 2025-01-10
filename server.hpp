#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.h"
# include "client.hpp"
# include "channel.hpp"
# include <netinet/in.h>
# include <sys/poll.h>

class Client;
class Channel;

class Server
{
private:
	std::string							_serverName;
	std::string							_password;
	int									_serFd;
	const int							_port;
	static int							_signal;
	std::vector<Client*>				_clients;
	std::vector<struct pollfd>			_pollFd;
	std::map<std::string, Channel *>	_channels; //map of channel name to channel
	
	/*constructor add*/
	// std::set<Channel *>				_channels;

	void		_clientRegister();
	void		_clientCommunicate( size_t i );
	void		_removeClient( int client, std::string message );
	
	void		_SocketInit();
	void		_serverRespond( int client );
	void		_ServerLoop( int pollIndx );
	bool		_parser( std::vector<std::string> &cmds, int client );
	std::string& _nickLower( std::string& nick );
	bool		_nickValidity( std::string& nick );

	

	// RESPONDS
	void	_capResp( std::vector<std::string> &cmds, int client );
	void	_userResp( std::vector<std::string> &cmds, int client );
	void	_nickResp( std::vector<std::string> &cmds, int client );
	void	_passResp( std::vector<std::string> &cmds, int client );
	void	_pingResp( std::vector<std::string> &cmds, int client );
	void	_privMsgResp( std::vector<std::string> &cmds, int client );
	void	_joinResp( std::vector<std::string> &cmds, int client );
	void	_partResp( std::vector<std::string> &cmds, int client );
	void	_quitResp( std::vector<std::string> &cmds, int client );


	void	_sendMsg( std::vector<std::string> &cmds, int client, int sender );

public:
	Server();
	Server( int port );
	~Server();
	
	void		ServerInit();

	void		signalMonitor();
	static void	handler( int signum );

	Channel *getChannel( std::string channelName );
	Client *getClient( std::string clientName );

	const std::string	&getName();

	//channel management
	void		joinChannel( Client *client, const std::string &channelName, std::string key );
	void		addChannel( Channel *);
	void		addChannel( std::string name );

	//debug
	void		printChannels();
	void		numericReply( Client *client, std::string numeric, std::string channel );
};

#endif