#ifndef __DATABASE__HPP
#define __DATABASE__HPP

# include <map>
# include "../Users/user.hpp"
# include "../Channels/channel.hpp"
#include <iostream>

/* Meyers' Singleton */
class Database
{
    private:
        Database();
        static Database database_;

        std::map<USER_ID, User*> _users;
        std::map<CHANNEL_NAME, Channel*> _channels;

    public:
        static Database& GetInstance();
        User* addNewUser(User*);
        Channel* addNewChannel(CHANNEL_NAME, User*);
        User* getUser(USER_ID);
        Channel* getChannel(CHANNEL_NAME);
        void deleteUser(USER_ID);
        void deleteChannel(CHANNEL_NAME);

        // ~Database();

};

#endif

//add to server `Command` Class with `execute` method