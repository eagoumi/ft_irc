// void Server::setClientNickname(int fd, const std::string& nickname)
// {
//     // Assuming _connectedClients maps fds to a client struct that includes a nickname
//     if (_connectedClients.find(fd) != _connectedClients.end()) {
//         _connectedClients[fd].nickname = nickname;
//     }
// }

// void Server::broadcastMessage(const std::string& message)
// {
//     for (const auto& fd : _Storeusersfd) {
//         // Avoid broadcasting to the listening socket
//         if (fd.fd != _Socketsfd) {
//             send(fd.fd, message.c_str(), message.length(), 0);
//         }
//     }
// }
