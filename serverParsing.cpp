#include "ft_irc.h"
#include "server.hpp"

void	Server::_serverRespond(){
	std::vector<std::string>	cmds;

	for (int i = 0; i <_clients.size(); i++){
		cmds = _clients[i].getCommand();
		while (cmds.size()){
		std::cout << "\n\n-------------start\n";
			_clients[i].clearBuff();
			for (int j = 0; j < cmds.size(); j++){
				std::cout << "** " << cmds[j] << std::endl;
				if (cmds[j] == "CAP")
					send (_pollFd[i + 1].fd, "CAP * LS\r\n:myserver 001 moamad :Welcome to the MyIrc, moamad\r\n", 63, 0);
			}
			cmds = _clients[i].getCommand();
		std::cout << "-------------ends\n\n";
		}
		std::cout << std::endl;
	}
}

