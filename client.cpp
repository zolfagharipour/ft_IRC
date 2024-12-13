#include "client.hpp"

/*don't i need to initiate the _opStatus and _channel anyhow?*/

Client::Client() : Fd (-1), _authenticated(false) {};

Client::Client(int fd, const std::string &ipAddress, std::string nickname, std::string username) : Fd(fd), IPadd(ipAddress), _authenticated(false) {
    _userName = username;
    _nickName = nickname;
    std::cout << username << ": " << "client created! Username: " << _userName << " Nickname: " << _nickName << std::endl;
}

/*destructor missing*/

int Client::GetFd() {
    return Fd;
}

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
