#ifndef __USER__HPP
#define __USER__HPP

#include <cstddef>
#include <iostream>
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
# define CND_LINE std::string
# define USER_AUTH bool

class Channel;

class   User
{
    private:
        USER_ID								_Id;
        std::string                         _cmdLine;
        NICK_NAME							_nickname;
        USER_NAME						    _username;
        bool                                _isNickInserted;
        bool                                _isPasswordInserted;
        bool                                _isUserNameInserted;
        std::map<std::string, Channel *>	_joinedChannels;

    public:
        User();
        User(USER_ID const&);
        USER_ID const& getUserId();
        void setUserName(USER_NAME const &);
        USER_NAME const& getUserName();
        void setNickName(NICK_NAME const &);
        NICK_NAME const& getNickName();
        void appendToCmdLine(CND_LINE const &);
        CND_LINE const& getCmdLine();
        void clearCmdLine();
        bool isStrContains(std::string const& str, std::string const& charSet);
        bool isStrStartWith(std::string const& str, std::string const& charSet);
        bool isAuthenticated();
        bool hasInsertedPass();
        bool hasInsertedNick();
        bool hasInsertedUsername();
        void insertedPassSuccessfully();
        void joinedChannel(Channel *channel);
        void partedChannel(Channel *channel);
        std::map<std::string, Channel *> & getJoinedChannels();
        ~User();
};

#endif