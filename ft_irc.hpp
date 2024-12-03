#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# include <iostream>
# include <vector> //-> for vector
# include <sys/socket.h> //-> for socket()
# include <sys/types.h> //-> for socket()
# include <netinet/in.h> //-> for sockaddr_in
# include <fcntl.h> //-> for fcntl()
# include <unistd.h> //-> for close()
# include <arpa/inet.h> //-> for inet_ntoa()
# include <poll.h> //-> for poll()
# include <csignal> //-> for signal()
//-------------------------------------------------------//
# define RED "\e[1;31m" //-> for red color
# define WHI "\e[0;37m" //-> for white color
# define GRE "\e[1;32m" //-> for green color
# define YEL "\e[1;33m" //-> for yellow color
#endif