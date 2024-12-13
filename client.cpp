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

// /*if channel does not exist yet, create it and make *this op
// otherwise just add this client*/
// /*SHOULD BE A SERVER FUNCTION*/
// void	Client::joinChannel(Channel *channel) {

//     if (_channels.find(channel) == _channels.end()) {
//         _channels.insert(channel);
//         _opStatus[channel] = false;
//         channel->addUser(this);
//     }
// }

// /*leave channel, if there is no other op, make the next user op
// if no user left, channel stays empty
// channel does not get deleted in any case*/
// void	Client::leaveChannel(Channel *channel) {
//     if (_channels.find(channel) != _channels.end()) {
//         _channels.erase(channel);
//         _opStatus.erase(channel);
//         channel->removeUser(this);
//     }
// }

// bool	Client::isInChannel(Channel *channel) {
//     if (_channels.find(channel) != _channels.end()) {
//         return true;
//     }
//     else
//         return false;
// }

// /*correct implementation?*/
// void	Client::setOPStatus(Channel *channel, bool status) {
//     if (_channels.find(channel) != _channels.end())
//         _opStatus[channel] = status;
// }

// void	Client::setChannel(Channel *channel) {
//     if (_channels.find(channel) != _channels.end())
//         this->_channels.insert(channel);
// }

// /*correct implementation?*/
// bool	Client::isOP(Channel *channel) {
//     if (_channels.find(channel) != _channels.end())
//         return _opStatus[channel];
//     else
//         return false;
// }

// void	Client::removeOP( Channel * channel ) {
//     this->_channels.erase(channel);
// }

// void	Client::removeChannel( Channel *channel ) {
//     this->_opStatus.erase(channel);
// }
