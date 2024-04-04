#ifndef __USER__HPP
#define __USER__HPP

#include <cstddef>
#include <iostream>
// #include "../server/server.hpp"
# include <map>
#include <sys/socket.h>
# include <string>
#include <sstream>
#include <fstream>
#include <istream>

# define USER_ID size_t  
# define CHANNEL_NAME std::string
# define USER_NAME std::string
# define NICK_NAME std::string
# define USER_AUTH bool

class Channel;

class   User
{
    private:
        USER_ID								_Id;
        NICK_NAME							_nickname;
        USER_NAME						    _username;
        std::map<std::string, Channel *>	_joinedChannels;//srsly I see no need for this, at least for now, :3 well I see now y I need it
        // bool								_isAuthenticated;
        bool                                _isPasswordInserted;
        bool                                _isNickInserted;
        bool                                _isUserNameInserted;
        std::string                         _IPServer;//this is will be removed

    public:
        User();
        User(USER_ID const&);
        USER_ID const& getUserId();
        void setUserName(USER_NAME const &);
        USER_NAME const& getUserName();
        void setNickName(NICK_NAME const &);
        NICK_NAME const& getNickName();
        bool isStrContains(std::string const& str, std::string const& charSet);
        bool isStrStartWith(std::string const& str, std::string const& charSet);
        //by agoumi
        // void SetAuthenticated();
        bool isAuthenticated();
        //by agoumi checking password authen
        // void Correct_Password();
        bool hasInsertedPass();
        // void NickCheck();
        bool hasInsertedNick();
        // void UserCheck();
        bool hasInsertedUsername();
        void insertedPassSuccessfully();
        //Print Errors
        // void setServerIP(std::string const &ServerIP);//this is not called anywhere
        // std::string const &getServerIP();

        //Send Server To Clients
        // void IRCPrint(size_t fd, std::string string);
        // void ServertoClient(std::string string);

        //Send Cleint to Clients
        // void CleintToClient(int id, std::string msg);
        // std::string GetIpAddress();
    
        void joinedChannel(Channel *channel);
        void partedChannel(Channel *channel);
        std::map<std::string, Channel *> const& getJoinedChannels();
        // Channel *createChannel(CHANNEL_NAME name);
        ~User();
};

#endif