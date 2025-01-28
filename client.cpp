#include "client.hpp"

Client::Client( ) : Fd (-1), _nickName("*"), _authenticated(false), _registered(false) {};

Client::Client( const Client& other ) {
	*this = other;
}

Client&	Client::operator=( const Client& other ){
	if (this == &other)
		return (*this);
	Fd = other.Fd;
	_nickName = other._nickName;
	_userName = other._userName;
	_realName = other._realName;
	_authenticated = other._authenticated;
	_registered = other._registered;
	_buff = other._buff;
	_vectorCMD = other._vectorCMD;
	return *this;
}

Client::~Client( ) {
    _vectorCMD.clear();
}

int Client::getFd( ) {
    return Fd;
}

std::string &Client::getNickName( ) {
    return _nickName;
}

std::string &Client::getUserName( ) {
    return _userName;
}

std::string &Client::getRealName( ) {
    return _realName;
}

bool    Client::isAuthenticated( ) {
    return _authenticated;
}

bool    Client::isRegistered( ) {
    return _registered; 
}

void Client::setFd( int fd ){
    this->Fd = fd;
}

void    Client::setNickName( std::string nick ){
    _nickName = nick;
}

void    Client::setUserName( std::string user ){
    _userName = user;
}

void    Client::setRealName( std::string name ){
    _realName = name;
}

void    Client::authenticate( ){
    _authenticated = true;
}

void    Client::registered( ){
    _registered = true;
}

void		Client::addBuff( std::string buff ){
    _buff.append(buff);
}

std::string	    &Client::getBuff( ) {
    return _buff;
}

void		Client::clearBuff( ){
    _vectorCMD.clear();
}

std::vector<std::string>	&Client::getCommand( ){
    std::string                 command;
    std::string                 word;
    std::istringstream          ss;
    size_t                      pos;

    if (_buff.size() <= 0)
        return (_vectorCMD);
    pos = _buff.find("\r\n");
    if (pos != std::string::npos){
        command = _buff.substr(0, pos);
        std::istringstream          ss(command);
        while (ss >> word){
            _vectorCMD.push_back(word);
        }
        _buff = _buff.substr(pos + 2, std::string::npos);
    }
    return (_vectorCMD);
}

