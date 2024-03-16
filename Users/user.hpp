#ifndef __USER__HPP
#define __USER__HPP

# include <map>
# include "../Channels/channel.hpp"
# include "../Server/server.hpp"

# define USER_ID int

class Channel;

class User
{
    private:
        User();
        // std
        std::map<std::string, Channel *> joinedChannels;
        // void getServerChannels();

    public:
        User(USER_ID &id);
        void joinChannel(Channel *channel);
        // void joinChannel(std::string const &name, std::map<std::string, class Channel> &serverChannels);
        ~User();
};

#endif