#include "client.hpp"
#include "channel.hpp"
#include "server.hpp"
#include <iostream>

// int chatgpt_main();

// int main() {
//     chatgpt_main();
//     return 0;
//     // Server server(6667);

//     // Client Hugo(1, "111.111.1.1", "Hugo", "Hugo Bond");
//     // Hugo.setFd(1);

    // Client Alice(1, "222.222.2.2", "Alice", "Alice Wonderland");
    // Alice.setFd(4);

    // Client Joye(1, "322.222.2.2", "Joye", "Joye Buster");
    // Joye.setFd(6);

    // Channel channel2("memes");
    // server.addChannel(&channel2);


    // std::cout << std::endl;

    // /*++++++++++++++++++*/
    // /*TESTING limits etc*/
    // /*++++++++++++++++++*/

    // // server.joinChannel(Hugo, channel.getName());
    // // server.joinChannel(Alice, channel.getName());
    // // std::cout << std::endl;

    // // channel.printUsers();    
    // // server.leaveChannel(Hugo, channel.getName());
    // // channel.printUsers();    
    
    // // server.leaveChannel(Alice, channel.getName());
    
    // // std::cout << std::endl;

    // /*++++++++++++++++++*/
    // /*TESTING MODES etc+*/
    // /*++++++++++++++++++*/

    // server.printChannels();

    // std::cout << std::endl;
    // server.joinChannel(Alice, channel2.getName(), "");
    // server.joinChannel(Hugo, channel2.getName(), "");
    // server.joinChannel(Joye, channel2.getName(), "");
    

    // channel2.printUsers();
    // std::cout << std::endl;

    // /*MODE: set/remove User Limit Restriction*/
    // channel2.setUserLimitRestriction(&Alice, true);
    // channel2.setUserLimit(&Hugo, 2);
    // std::cout << std::endl;

    // /*MODE: set/remove Topic Restriction*/
    // channel2.setTopicRestriction(&Alice, true);
    // channel2.setTopic(&Hugo, "This is fun");
    // std::cout << std::endl;
    
    // /*MODE: give/take OP privilege*/
    // channel2.changeOperatorPrivilege(&Alice, &Joye, 1);
    // channel2.changeOperatorPrivilege(&Hugo, &Joye, 1);
    // channel2.printUsers();
    // channel2.changeOperatorPrivilege(&Alice, &Joye, 0);
    // channel2.printUsers();
    // std::cout << std::endl;

    // /*KICK*/
    // channel2.kickUser(&Alice, &Joye);
    // std::cout << std::endl;

    // /*MODE: set/remove Invite-only channel*/
    // channel2.printUsers();
    // channel2.setInviteOnly(&Alice, true);
    // std::cout << std::endl;

    // server.joinChannel(Alice, channel2.getName(), "hi");
    // std::cout << std::endl;

    // channel2.kickUser(&Alice, &Hugo);
    // channel2.printUsers();

    // channel2.inviteUser(&Alice, &Joye);
    // channel2.printUsers();
    // std::cout << std::endl;


    // /*MODE: set/remove key*/
    // channel2.setInviteOnly(&Alice, false);

    // channel2.setKey(&Alice, "");
    // // channel2.inviteUser(&Alice, &Hugo);
    // server.joinChannel(Hugo, channel2.getName(), "");
    // return 0;
// }

// int chatgpt_main() {
//     Server server;

//     // Create clients
//     Client Hugo(1, "111.111.1.1", "Hugo", "Hugo Bond");
//     Hugo.setFd(1);

//     Client Alice(1, "222.222.2.2", "Alice", "Alice Wonderland");
//     Alice.setFd(4);

//     Client Joye(1, "322.222.2.2", "Joye", "Joye Buster");
//     Joye.setFd(6);

//     // Create and add a channel to the server
//     Channel memesChannel("memes", );
//     server.addChannel(&memesChannel);

//     std::cout << "\n=====================\n";
//     std::cout << "   TESTING CHANNELS  \n";
//     std::cout << "=====================\n";

//     // Test: Join, leave, and print users
//     server.joinChannel(&Alice, memesChannel.getName(), "");
//     server.joinChannel(&Hugo, memesChannel.getName(), "");
//     memesChannel.printUsers();
//     std::cout << "\n";

//     server.leaveChannel(Hugo, memesChannel.getName());
//     memesChannel.printUsers();
//     std::cout << "\n";

//     server.leaveChannel(Alice, memesChannel.getName());
//     memesChannel.printUsers();
//     std::cout << "\n";

//     std::cout << "\n=====================\n";
//     std::cout << "   TESTING MODES     \n";
//     std::cout << "=====================\n";

//     // Add users again
//     server.joinChannel(&Alice, memesChannel.getName(), "");
//     server.joinChannel(&Hugo, memesChannel.getName(), "");
//     server.joinChannel(&Joye, memesChannel.getName(), "");
//     memesChannel.printUsers();
//     std::cout << "\n";

//     // Test: User limit restriction
//     memesChannel.setUserLimitRestriction(&Alice, true); // Alice is not an operator; should fail
//     memesChannel.setUserLimit(&Hugo, 2);                // Hugo sets the limit to 2
//     std::cout << "\n";

//     // Test: Topic restriction
//     memesChannel.setTopicRestriction(&Alice, true);     // Alice is not an operator; should fail
//     memesChannel.setTopic(&Hugo, "This is fun");        // Hugo sets the topic
//     std::cout << "\n";

//     // Test: Operator privilege
//     memesChannel.changeOperatorPrivilege(&Hugo, &Joye, 1); // Hugo makes Joye an operator
//     memesChannel.printUsers();
//     memesChannel.changeOperatorPrivilege(&Alice, &Joye, 0); // Alice removes Joye as operator; should fail
//     memesChannel.printUsers();
//     std::cout << "\n";

//     // Test: KICK command
//     memesChannel.kickUser(&Hugo, &Joye); // Hugo kicks Joye
//     memesChannel.printUsers();
//     std::cout << "\n";

//     // Test: Set/remove key
//     memesChannel.setKey(&Alice, "secret");               // Hugo sets a key
//     memesChannel.setKey(&Hugo, "");                     // Hugo removes the key
//     server.joinChannel(&Hugo, memesChannel.getName(), ""); // Hugo rejoins without a key
//     memesChannel.printUsers();
//     std::cout << "\n";

//     // Test: Invite-only channel
//     memesChannel.setInviteOnly(&Hugo, true);             // Hugo enables invite-only mode
//     server.joinChannel(&Alice, memesChannel.getName(), "hi"); // Alice tries to join with incorrect key; should fail
//     memesChannel.printUsers();
//     std::cout << "\n";
//     std::cout << "\n";

//     memesChannel.inviteUser(&Hugo, &Joye);              // Hugo invites Joye
//     std::cout << "\n";
    
//     server.joinChannel(&Joye, memesChannel.getName(), ""); // Joye joins successfully
//     memesChannel.printUsers();
//     std::cout << "\n";

//     std::cout << "=====================\n";
//     std::cout << "   TESTING COMPLETE  \n";
//     std::cout << "=====================\n";

//     return 0;

// }
