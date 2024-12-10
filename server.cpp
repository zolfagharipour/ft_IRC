#include "server.hpp"

int Server::_signal = false;

Server::Server() : _port(6667){ }

Server::Server( int port ) : _port(port) { }

Server::~Server()
{
	_closeFds();
	// removeClient()
}
