#include "client.hpp"

Client::Client() : Fd (-1), _authenticated(false) {};

Client::Client(int fd, const std::string &ipAddress) : Fd(fd), IPadd(ipAddress), _authenticated(false) {}

int Client::GetFd() {return Fd;} //-> getter for fd

std::string &Client::getNickName() {
    return _nickName;
}

std::string &Client::getUserName() {
    return _userName;
}

bool		Client::isAuthenticated() {
    return _authenticated; 
}
void Client::setFd(int fd){
    Fd = fd;
}

void Client::setIpAdd(std::string ipadd){
    IPadd = ipadd;
}

void	Client::joinChannel(std::string &channel) {
    
}

void	Client::leaveChannel(std::string &channel)
bool	Client::isInChannel(std::string &channel)
void	Client::setOPStatus(std::string &channel, bool status)
bool	Client::isOP(std::string &channel)