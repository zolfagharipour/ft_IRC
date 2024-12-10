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

bool    Client::isAuthenticated() {
    return _authenticated; 
}

void Client::setFd(int fd){
    Fd = fd;
}

void Client::setIpAdd(std::string ipadd){
    IPadd = ipadd;
}

void	Client::joinChannel(Channel *channel) {
    
}

void	Client::leaveChannel(Channel *channel) {

}

bool	Client::isInChannel(Channel *channel) {
    if (_channels.find(channel))
}

void	Client::setOPStatus(Channel *channel, bool status) {

}

bool	Client::isOP(Channel *channel) {

}
