#include "ft_irc.h"
#include "server.hpp"


void	Server::_capResp( std::vector<std::string> &cmds, int client ){
	if (cmds.size() > 1 && cmds[1] == "LS"){
		std::string respond = "CAP * LS\r\n";
		send(_pollFd[client + 1].fd, respond.data(), respond.size(), 0);
		std::cout << "---CAP SENT---" << std::endl; 
	}
	else if (cmds.size() > 1 && cmds[1] == "END"){
		// DO ANYTHING?
	}
}


void	Server::_nickResp( std::vector<std::string> &cmds, int client ){
	std::string	respond;

	if (cmds.size() < 2)
		return ;
	for (int i = 0; i < _clients.size(); i++){
		if (_clients[i].getNickName() == cmds[1]){
			respond = "myserver 433 " + _clients[client].getNickName() + cmds[1] + " :Nickname is already in use";
			send(_pollFd[client + 1].fd, respond.data(), respond.size(), 0);
			return ;
		}
	}
	_clients[client].setNick(cmds[1]);
}


void	Server::_userResp( std::vector<std::string> &cmds, int client ){
	std::string	respond;

	if (cmds.size() < 2)
		return ;
	respond = ":myserver 001 " + _clients[client].getNickName() + " :Welcome to the MyIrc, " + _clients[client].getNickName() + "\r\n";
	send (_pollFd[client + 1].fd, respond.data(), respond.size(), 0);
	std::cout << "---WELCOME SENT---" << std::endl; 
}









void	Server::_parser( std::vector<std::string> &cmds, int client ){
	if (cmds[0] == "CAP"){
		// std::cout << "cap starts" << std::endl;
		_capResp(cmds, client);
	}
	else if (cmds[0] == "NICK")
		_nickResp(cmds, client);
	else if (cmds[0] == "USER")
		_userResp(cmds, client);
}



void	Server::_serverRespond(){
	std::vector<std::string>	cmds;

	for (int i = 0; i <_clients.size(); i++){
		cmds = _clients[i].getCommand();
		while (cmds.size()){
			_parser(cmds, i);
			_clients[i].clearBuff();
			cmds = _clients[i].getCommand();
			// std::cout << "\n\n-------------start\n";

			// for (int j = 0; j < cmds.size(); j++){
			// 	std::cout << "** " << cmds[j] << std::endl;
			// 	if (cmds[j] == "CAP")
			// 		send (_pollFd[i + 1].fd, "CAP * LS\r\n:myserver 001 moamad :Welcome to the MyIrc, moamad\r\n", 63, 0);
			// }
			// std::cout << "-------------ends\n\n";
		}
		std::cout << std::endl;
	}
}

