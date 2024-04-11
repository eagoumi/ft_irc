<!-- ```Given the specifics of your `ft_irc` project, let's draft a more detailed README with sections that highlight its architecture, detailed build instructions, usage examples, and more. This template can be adapted as your project evolves. -->

---

# ft_irc: Custom IRC Server Implementation

`ft_irc` is an Internet Relay Chat (IRC) server built from scratch in C++98, adhering strictly to the standards and protocols that govern real-time internet text messaging. This project allows for an understanding of network programming, multi-client server architecture, and the IRC protocol.

## Project Overview

This IRC server implementation supports multiple client connections, enabling users to join channels, send and receive messages, and perform channel operations. It's designed to be compatible with standard IRC clients and provides a foundation for extending functionality to more complex IRC commands and features.

### Features

- **Multi-client Management**: Handles multiple client connections simultaneously without blocking.
- **Channel Operations**: Supports creating and joining channels, messaging within channels, and managing channel topics and modes.
- **User Commands**: Implements a subset of IRC commands including NICK, USER, JOIN, PRIVMSG, NOTICE, PART, QUIT, and MODE.
- **Operator Privileges**: Special privileges for channel operators like KICK, INVITE, and managing channel modes.
- **Authentication**: Requires a password for client connections to ensure controlled access.

## Installation

### Prerequisites

- A Unix-like operating system (Linux/MacOS)
- C++98 compliant compiler (e.g., g++)
- Make (for building the server using the provided Makefile)

### Building the Server

Clone the project repository:

```bash
git clone https://github.com/eagoumi/ft_irc.git
cd ft_irc
```

Compile the server using the Makefile:

```bash
make
```

This will create the `ircserv` executable within the project directory.

## Usage

To start the IRC server, run:

```bash
./ircserv <port> <server_password>
```

- `<port>`: Specifies the port number on which the server listens for incoming connections.
- `<server_password>`: Sets the password required for clients to connect to the server.

### Connecting to the Server

Clients can connect to the server using any standard IRC client by specifying the server's IP address and the port number. The connection password must be provided upon connection.

## Configuration (Optional)

An optional configuration file can be used to specify server settings such as default channels, operator passwords, and server modes. Refer to `server.config.example` for a template.

## Supported Commands

This server supports a variety of IRC commands essential for channel management and communication. Here is a list of some supported commands:

- `NICK`: Set a nickname.
- `USER`: Specify username, hostname, servername, and realname.
- `JOIN`: Join a channel.
- `PART`: Leave a channel.
- `MODE`: Set or unset user or channel modes.
- `PRIVMSG`: Send private messages to a user or channel.
- `QUIT`: Disconnect from the server.

For a full list of commands and their syntax, refer to the IRC protocol documentation.

## Acknowledgments

- Special thanks to the 42 School for providing the guidelines and specifications for this project.
- Appreciation to all contributors and peers who have offered feedback and suggestions.
