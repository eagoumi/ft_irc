#ifndef __SERVER__HPP
#define __SERVER__HPP

# include <map>
# include <iostream>
# include "../Users/user.hpp"
# include "../Channels/channel.hpp"

class Server
{
    private:
        std::map<USER_ID, class User> users;
        std::map<std::string, class Channel> channels;
        // friend void User::getServerChannels();
        // friend User;
        // friend Channel;

    public:
        Server(const std::string &port, const std::string &password);
        ~Server();
};

#endif

//add to server `Command` Class with `execute` method