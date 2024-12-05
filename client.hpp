#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.h"

class Client {
    private:
        int fd;
        // std::string IpAddr;
    public:
        Client ();
        ~Client();
        
        int     getFd();
        // int     getIp();
        
        void    setFd(int fd);
        // void    setIpAddr(std::string ip);
};

#endif