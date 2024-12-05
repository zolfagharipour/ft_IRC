#include "client.hpp"

Client::Client () {
    //sth with IP?
    //sth with Fd?
    std::cout << "Client constructed" << std::endl;
}

Client::~Client() {
    std::cout << "Client destructed" << std::endl;
}

int Client::getFd() {
    return this->fd;
}

/*
int Client::getIp() {
    return this->IpAddr;
}
*/

void Client::setFd(int fd) {
    this->fd = fd;
}

/*
void Client::setIpAddr(std::string ip) {
    this->IpAddr = ip;
}
*/
