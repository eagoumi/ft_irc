#ifndef __SERVER__HPP
#define __SERVER__HPP

#include <i386/types.h>
#include <iostream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <vector>
#include <map>

class USER
{
    public:
};

class Server 
{
    private:
        int                         _Port;
        std::string                 _Password;
        std::string                 _IPHostAdress;
        int                         _Socketsfd;
        sockaddr_in                 _Sockadd;
        std::vector<struct pollfd>    _Storeusersfd; //Storing used fds to monitor multiple file descriptors all in one
        // std::map<int, USER>          _ConnectedUser;
    public:
        Server(const int &port, const std::string &password);
        ~Server();

        //setter
        void SetUserHostip(std::string USERHOST);
        void SetServerHostip(std::string SERVERHOST);
        void ServerStarting();
        // Create Sockets
        void createSockets();
        void setSocketsopt();
        void listtenSock();
        void accept_connection();
        //Call and initial Commands and store it on map
        void CommandMapinit();
};

#endif