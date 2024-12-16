#include "ft_irc.h"
#include "server.hpp"


void	Server::_capResp( std::vector<std::string> &cmds, int client ){
	if (cmds.size() > 1 && cmds[1] == "LS"){
		// std::string respond = "CAP * LS\r\n";
		std::string respond = "CAP * LS :\r\n";
		send(_pollFd[client + 1].fd, respond.data(), respond.size(), 0);
		std::cout << "\n>> " << respond;
	}
}
void	Server::_passResp( std::vector<std::string> &cmds, int client ){
	if (cmds.size() < 2)
		return ;
	if (cmds[1] == _password){
		_clients[client].authenticate();
		return ;
	}
	_numericReply(client, "464", "");
	// _removeClient(_clients[client].getFd());
}

void	Server::_userResp( std::vector<std::string> &cmds, int client ){
	std::string	respond;

	if (cmds.size() < 2 || _clients[client].getNickName() == "*")
		return ;
	_clients[client].registered();
	_numericReply(client, "001", "");
}

void	Server::_pingResp( std::vector<std::string> &cmds, int client ){
		std::string respond = "PONG";

		if (cmds.size() > 1)
			respond += " " + cmds[1];
		send(_pollFd[client + 1].fd, respond.data(), respond.size(), 0);
		std::cout << "\n>> " << respond;
}



void	Server::_parser( std::vector<std::string> &cmds, int client ){
	if (!_clients[client].isAuthenticated()
			&& cmds[0] != "CAP" && cmds[0] != "PASS" && cmds[0] != "JOIN"){
		_numericReply(client, "464", "");
		// _removeClient(_clients[client].getFd());
		return ;
	}

	if (cmds[0] == "CAP")
		_capResp(cmds, client);
	else if(cmds[0] == "PASS")
		_passResp(cmds, client);
	else if (cmds[0] == "NICK")
		_nickResp(cmds, client);
	else if (cmds[0] == "USER")
		_userResp(cmds, client);
	else if (cmds[0] == "PING")
		_pingResp(cmds, client);

}



void	Server::_serverRespond(){
	std::vector<std::string>	cmds;

	for (int i = 0; i <_clients.size(); i++){
		cmds = _clients[i].getCommand();
		while (cmds.size()){
			_parser(cmds, i);
			_clients[i].clearBuff();
			cmds = _clients[i].getCommand();
		}
		std::cout << std::endl;
	}
}

