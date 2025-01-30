# FT_IRC

This project involves creating our own IRC (Internet Relay Chat) server in **C++98**. The server allows multiple clients to connect, communicate in real-time, and interact within channels. It emphasizes understanding and implementing **network protocols**, **handling multiple clients simultaneously**, and ensuring **non-blocking I/O operations**.

Our reference client for development and testing is **irssi**, ensuring compatibility and stability. While the server is tested with irssi, it is designed to work with other IRC clients as well.


## Warning for 42 Students

This repository is intended as a reference and educational tool. **42 students are strongly advised not to copy this code without fully understanding its functionality.** Plagiarism in any form is against 42's principles and could lead to serious academic consequences. Use this repository responsibly to learn and better understand how to implement similar functionalities on your own.

## Features

**Real-time Messaging**: Supports both public and private messaging. Messages sent in a channel are broadcast to all members.

**Channel Management**: Users can create and join channels, send messages, and manage channel settings.

**User Roles**: Implements both operators and regular users with distinct privileges.

**C++98 Compliance**: The server is built in C++98, ensuring compatibility with the specified standard.

**Single poll() Call**: The server must handle all I/O operations with a single poll().

**Non-blocking I/O**: The server operates in a non-blocking manner, ensuring no process hangs. (Forking was not allowed).

## Implemented IRC Commands:

- `KICK:` Removes a user from a channel.
- `INVITE:` Invites a user to a channel.
- `TOPIC:` Changes or displays the channel's topic.
- `MODE:` Modifies channel settings with the following flags:
    - ``+i / -i``: Enable/disable invite-only mode.
    - ``+t / -t``: Restrict topic changes to operators.
    - ``+k <key> / -k``: Set/remove a channel key (password).
    - ``+o <user> / -o <user>``: Grant/revoke operator privileges.
    - ``+l <limit> / -l``: Set/remove a user limit on the channel.

## Bonus Features

- **File Transfer**: Implements a file transfer feature, allowing users to send files through the IRC server.

- **Chat Moderation Bot**: A built-in bot that can be activated or deactivated by a channel operator.
     - Activated by sending ```bot on``` in the channel and deactivated with ```bot off```.
     - When active, it automatically replaces certain insulting words with more polite alternatives (e.g., ```shit``` → ```sheet```).
     - The bot also monitors messages and forcefully kicks any user who criticizes the Chinese government, making it appear as if they left voluntarily.

## Compilation

Clone the repository and compile the Minishell with the following commands:

```bash
git clone https://github.com/yourusername/ft_irc.git
cd ft_irc
make
```

## Usage

Connecting to the Server: Use an IRC client to connect to the server using the specified port and password.
```bash
./ircserver <port> <password>
```
Commands: Once connected, you can use the supported IRC commands to interact with the server and other clients.
