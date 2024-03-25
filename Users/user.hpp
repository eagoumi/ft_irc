#ifndef __USER__HPP
#define __USER__HPP

#include <cstddef>
# include <map> 
# include <string>

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
        std::string							_nickname;
        std::string							_username;
        std::map<std::string, Channel *>	_joinedChannels;//srsly I see no need for this, at least for now
        bool								_isAuthenticated;
        bool                                _Correct_Password;
        bool                                _NickCheck;
        bool                                _UserCheck;

    public:
        User(USER_ID const&);
        USER_ID const& getUserId();
        void setUserName(USER_NAME const &);
        USER_NAME const& getUserName();
        void setNickName(NICK_NAME const &);
        NICK_NAME const& getNickName();
        
        //by agoumi
        void SetAuthenticated();
        bool isAuthenticated();
        //by agoumi checking password authen
        void Correct_Password();
        bool isCorrect_Password();
        void NickCheck();
        bool isNickCheck();
        void UserCheck();
        bool isUserCheck();

        // void joinChannel(Channel *channel);
        // Channel *createChannel(CHANNEL_NAME name);
        ~User();
};

#endif