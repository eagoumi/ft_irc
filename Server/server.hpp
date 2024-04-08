#ifndef __SERVER__HPP
#define __SERVER__HPP


# include "../Database/database.hpp"
# include "../Logger/logger.hpp"
# include "../error_request.hpp"
// #include <i386/types.h>
#include <string>
#include <iostream>
#include <iterator>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <istream>
#include <csignal>
#include <map>



class Server 
{
    private:
        Database*                       _db;
        Logger&                         _logger;
        int                             _Port;
        std::string                     _Password;
        std::string                     _IPHostAdress;
        int                             _Socketsfd;
        sockaddr_in                     _Sockaddsrv;
        sockaddr_in                     _Sockaddclient;
        std::vector<struct pollfd>      _Storeusersfd; //Storing used fds to monitor multiple file descriptors all in one
		struct pollfd                   _pollfds;
        std::string                     _pass;
        User                            *_User;
        // bool                            _IsAuth;
        // bool                            _correct_pass;
        // bool                            _NickCheck;
        // bool                            _UserCheck;

        // std::map<int, USER>          _ConnectedUser;
    public:
        Server(const int &port, const std::string &password);
        ~Server();

        // std::string skipSpace(std::string string);

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
        // void CommandMapinit();

        void CheckForConnectionClients();
        void HandleClientData(size_t index, const char *data);
        void SetClientNickName(int fd, std::string& nickname);
        std::string getUsername();
        std::string getNickname();
        
        // Authentication
        // void Authentication(int index, const char* data);//, bool& _IsAuth, bool& _correct_pass, bool& _NickCheck, bool& _UserCheck);
        void getregestred(int index, std::string data);

        std::string HostIPADress();
        void WelcomeClient(User *currUser);

        // //Send
        // void IRCPrint(size_t fd, std::string string);
        // void ServertoClient(std::string string);

        //Command Quit 
        void Quit(std::string reason);
};

#endif

//add to server `Command` Class with `execute` method