#ifndef __DATABASE__HPP
#define __DATABASE__HPP

# include <map>
# include <iostream>
#include <string>
# include "../Users/user.hpp"
# include "../Channels/channel.hpp"

class Database
{
    private:
        Database();
        static Database* database_;

        std::map<USER_ID, User*> users;
        std::map<std::string, Channel*> channels;

    public:
        static Database *GetInstance();
        User* addNewUser(USER_ID, User* user);
        Channel* addNewChannel(CHANNEL_NAME, Channel* channel);
        User* getUser(USER_ID);
        Channel* getChannel(CHANNEL_NAME);

        // ~Database();
};

#endif

//add to server `Command` Class with `execute` method