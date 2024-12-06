#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.h"
# include "client.hpp"
#include <netinet/in.h>
#include <sys/poll.h>

class Server
{
private:
	int								_serFd;
	const int						_port;
	static int						_signal;
	std::vector<Client>				_clients;
	std::vector<struct pollfd>		_pollFd;

	void		clientRegister();
	void		clientCommunicate(size_t i);
	void		closeFds();
	void		removeClient(int fd);
	
	void		SocketInit();
public:
	Server();
	~Server();
	
	void		ServerInit();
	void		signalMonitor();
	static void	handler(int signum);
};

#endif