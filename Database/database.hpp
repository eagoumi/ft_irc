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
        std::map<CHANNEL_NAME, Channel*> channels;

    public:
        static Database *GetInstance();
        User* addNewUser(USER_ID, User* user);
        Channel* addNewChannel(CHANNEL_NAME name, User* user);
        User* getUser(USER_ID);
        Channel* getChannel(CHANNEL_NAME);
        void deleteUser(USER_ID);
        void deleteChannel(CHANNEL_NAME);

        // ~Database();
};

#endif

//add to server `Command` Class with `execute` method