#ifndef __USER__HPP
#define __USER__HPP

# include <map>
// # include "../Channels/channel.hpp"

# define USER_ID int
# define CHANNEL_NAME std::string

class Channel;

class User
{
    private:
        std::map<std::string, Channel *> joinedChannels;

    public:
        User();
        void joinChannel(Channel *channel);
        Channel *createChannel(CHANNEL_NAME name);
        ~User();
};

#endif