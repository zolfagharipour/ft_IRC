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
	std::string									_serverName;
	std::string									_password;
	int											_serFd;
	const int									_port;
	static int									_signal;
	std::vector<Client*>						_clients;
	std::vector<struct pollfd>					_pollFd;
	std::map<std::string, Channel *>			_channels;
	std::map<std::string, std::string>			_numericResponse;

	typedef void (Server::*CommandHandler)( std::vector<std::string>& cmds, int client );
    std::map<std::string, CommandHandler> _commandFn;

	void			_clientRegister( );
	void			_clientCommunicate( size_t i );
	void			_removeClient( int client, std::string message );
	void			_SocketInit( );
	void			_commandFnInit( );
	void			_serverRespond( int client );
	void			_ServerLoop( int pollIndx );
	bool			_parser( std::vector<std::string> &cmds, int client );
	std::string&	_lowerCase( std::string& nick );
	bool			_nickValidity( std::string& nick );
	bool			_channelNameValidity( std::string& channelName );

	// RESPONDS
	void	_capResp( std::vector<std::string> &cmds, int client );
	void	_userResp( std::vector<std::string> &cmds, int client );
	void	_nickResp( std::vector<std::string> &cmds, int client );
	void	_passResp( std::vector<std::string> &cmds, int client );
	void	_pingResp( std::vector<std::string> &cmds, int client );
	void	_privMsgResp( std::vector<std::string> &cmds, int client );
	void	_joinResp( std::vector<std::string> &cmds, int client );
	void	_partResp( std::vector<std::string> &cmds, int client );
	void	_modeResp( std::vector<std::string> &cmds, int client );
	void	_topicResp( std::vector<std::string> &cmds, int client );
	void	_kickResp( std::vector<std::string> &cmds, int client );
	void	_quitResp( std::vector<std::string> &cmds, int client );
	void	_inviteResp( std::vector<std::string> &cmds, int client );

	void	_sendMsg( std::vector<std::string> &cmds, int client, int sender );
	bool	_botActivation( int client, Channel& channel, std::string message );

public:
	Server( int port, std::string password );
	~Server( );
	
	void		ServerInit( );
	void		signalMonitor( );
	static void	handler( int signum );

	Channel 	*getChannel( std::string channelName );
	Client		*getClient( std::string clientName );

	const std::string	&getName( );

	//channel management
	void		joinChannel( Client *client, const std::string &channelName, std::string key );
	void		addChannel( Channel *);
	void		addChannel( std::string name );

	void		numericReply( Client *client, std::string numeric, std::string channel, std::string command, std::string nickName );
};

#endif