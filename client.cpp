#include "client.hpp"

/*don't i need to initiate the _opStatus and _channel anyhow?*/

Client::Client() : Fd (-1), _authenticated(false), _nickName("*") {};

Client::Client(int fd, const std::string &ipAddress, std::string nickname, std::string username) : Fd(fd), IPadd(ipAddress), _authenticated(false) {
    _userName = username;
    _nickName = nickname;
    std::cout << username << ": " << "client created! Username: " << _userName << " Nickname: " << _nickName << std::endl;
}

/*destructor missing*/

int Client::getFd() {
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
void    Client::setNick( std::string nick ){
    _nickName = nick;
}

void		Client::addBuff( std::string buff ){
    _buff.append(buff);
}

std::string	    &Client::getBuff() {
    return _buff;
}

void		Client::clearBuff(){
    _vectorCMD.clear();
}

std::vector<std::string>	&Client::getCommand(){
    std::string                 command;
    std::string                 word;
    std::istringstream          ss;
    size_t                      pos;

    pos = _buff.find("\r\n");
    if (pos != std::string::npos){
        command = _buff.substr(0, pos);
        std::istringstream          ss(command);
        // _vectorCMD.clear();
        while (ss >> word){
            _vectorCMD.push_back(word);
        }
        _buff = _buff.substr(pos + 2, std::string::npos);
    }
    return (_vectorCMD);
}

