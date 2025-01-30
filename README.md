# FT_IRC

This project involves creating your own IRC (Internet Relay Chat) server in C++98. The server will allow multiple clients to connect, communicate in real-time, and interact with channels. The project emphasizes understanding and implementing network protocols, handling multiple clients simultaneously, and ensuring non-blocking I/O operations.


## Warning for 42 Students

This repository is intended as a reference and educational tool. **42 students are strongly advised not to copy this code without fully understanding its functionality.** Plagiarism in any form is against 42's principles and could lead to serious academic consequences. Use this repository responsibly to learn and better understand how to implement similar functionalities on your own.

## Features
  **Real-time Messaging**: Supports both public and private messaging.

  **Channel Management**: Users can join channels, send messages, and manage channel settings.

  **User Roles**: Supports operators and regular users with different privileges.

  **Commands**: Implements essential IRC commands such as:

      - KICK: Eject a client from a channel.

      - INVITE: Invite a client to a channel.

      - TOPIC: Change or view the channel topic.

      - MODE: Change the channel's mode (e.g., invite-only, channel key, user limit).

Requirements

    C++98 Compliance: The code must comply with the C++98 standard.

    Non-blocking I/O: All I/O operations must be non-blocking, and forking is not allowed.

    Single poll() Call: Only one poll() (or equivalent) call can be used to handle all I/O operations.

    Makefile: The project must include a Makefile with the rules: $(NAME), all, clean, fclean, and re.

Installation

    Clone the repository:
    bash
    Copy

    git clone https://github.com/yourusername/ft_irc.git
    cd ft_irc

    Compile the project:
    bash
    Copy

    make

    Run the server:
    bash
    Copy

    ./ircserv <port> <password>

    Replace <port> with the desired port number and <password> with the connection password.

Usage

    Connecting to the Server: Use an IRC client to connect to the server using the specified port and password.

    Commands: Once connected, you can use the supported IRC commands to interact with the server and other clients.

Testing

To ensure the server handles various scenarios correctly, you can use the nc (netcat) command to simulate client connections and send commands in parts:
bash
Copy

nc -C 127.0.0.1 6667
com^Dman^Dd

Use Ctrl+D to send the command in several parts.
