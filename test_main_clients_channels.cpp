#include "client.hpp"
#include "channel.hpp"
#include "server.hpp"
#include <iostream>

// int main() {
//     Client client1(1, "111.111.1.1", "Hugo", "Hugo Bond");
//     client1.setFd(1);
//     std::cout << "nickname: " << client1.getNickName() << std::endl;
//     std::cout << "username: " << client1.getUserName() << std::endl;


//     Client client2(1, "222.222.2.2", "Alice", "Alice Wonderland");
//     client2.setFd(4);
//     std::cout << "nickname: " << client2.getNickName() << std::endl;
//     std::cout << "username: " << client2.getUserName() << std::endl;
//     Channel channel("general");
//     channel.addUser(&client1);
//     channel.addUser(&client2);
//     return 0;
// }
