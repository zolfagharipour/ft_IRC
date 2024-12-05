#include "ft_irc.h"
#include "server.hpp"



int	main(){
    try {
        Server server;
        server.signalMonitor();
        server.ServerInit();
        server.~Server();
    } catch(std::exception &e) {
        std::cout << "ERROR: " << std::endl;
    }
}