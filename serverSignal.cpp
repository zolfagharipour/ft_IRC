#include "server.hpp"

void	Server::handler( int signum ) {
	(void)signum;
	_signal = true;
	std::cout << "\nSignal received: Server stopped" << std::endl;
}

void Server::signalMonitor ( ) {
	int signals[] = {SIGINT, SIGQUIT, SIGTERM, SIGPIPE};
	for (size_t i = 0; i < sizeof(signals) / sizeof(signals[0]); i++)
		signal(signals[i], &handler);
}
