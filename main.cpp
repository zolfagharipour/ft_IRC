#include "ft_irc.h"
#include "server.hpp"

int	isPortValid( std::string port ){
	int	numPort = 0;
	if (port.empty() || port[0] == 0)
		return 0;
	for (size_t i = 0; i < port.size(); i++){
		if (!isdigit(port[i]))
			return 0;
		numPort *= 10;
		numPort += port[i] - 48;
	}
	if (numPort < 1024 || numPort > 65535)
		return 0;
	return numPort;
}

int	main( int ac, char** av ){
	if (ac != 3){
		std::cerr << "We need two arguements:\n\t-Port: The port to connect to server.\n\t-Password: The connection password." << std::endl;
		return 1;
	}
	int			port = isPortValid(av[1]);
	std::string password (av[2]);
	if (!port){
		std::cerr << "Only port number between 1024-65535 (with no leading zeroes) is accepted.";
		return 1;
	}
    try {
        Server server(port, password);
        server.signalMonitor();
        server.ServerInit();
    } catch(std::exception &e) {
        std::cout << "ERROR: " << std::endl;
    }
}																								