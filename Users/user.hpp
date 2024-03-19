#ifndef __USER__HPP
#define __USER__HPP

# include <map>
# include <string>
// # include "../Channels/channel.hpp"

# define USER_ID int
# define CHANNEL_NAME std::string
# define USER_NAME std::string
# define NICK_NAME std::string

class Channel;

class User
{
    private:
        std::string _nickname;
        std::string _username;
        std::map<std::string, Channel *> joinedChannels;//srsly I see no need for this, at least for now

    public:
        User();
        void setUserName(USER_NAME);
        USER_NAME getUserName();
        void setNickName(NICK_NAME);
        NICK_NAME getNickName();
        // void joinChannel(Channel *channel);
        // Channel *createChannel(CHANNEL_NAME name);
        ~User();
};

#endif