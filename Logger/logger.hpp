#ifndef __LOGGER__HPP
#define __LOGGER__HPP

#include "../Database/database.hpp"
#include <cstddef>
# include <string>

# define USER_ID size_t  
# define CHANNEL_NAME std::string
# define USER_NAME std::string
# define NICK_NAME std::string
# define USER_AUTH bool

/* Meyers' Singleton */
class   Logger
{
    private:
        Logger();
        ~Logger();
        Logger(Logger const&);
        void operator=(Logger const&);

        USER_ID	    _userFd;
        NICK_NAME   _nickname;
        USER_NAME   _username;
        std::string _serverIp;
        User*       _currUser;

    public:
        static Logger& GetInstance();
        void setCurrUser(User* userFd);
        void setServerIp(std::string const& serverIp);
        std::string GetUserName();
        std::string GetNickName();
        std::string getServerIP();
        std::string PrefixLogs();
        void IRCPrint(std::string string);
        void ServertoClient(std::string string);
        void CleintToClient(std::string msg);
        void SendJoinedMembers(Channel *Channel_name, std::string command);

};

#endif