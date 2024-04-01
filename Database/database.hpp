#ifndef __DATABASE__HPP
#define __DATABASE__HPP

# include <map>
# include "../Users/user.hpp"
# include "../Channels/channel.hpp"
#include <iostream>
#include <sys/socket.h>


/* Meyers' Singleton */
class Database
{
    private:
        Database();
        Database(Database const&);
        void operator=(Database const&);
        static Database* database_;

        std::map<USER_ID, User*> _users;
        std::map<CHANNEL_NAME, Channel*> _channels;

    public:
        static Database* GetInstance();
        User* addNewUser(User*);
        Channel* addNewChannel(CHANNEL_NAME, User*);
        User* getUser(USER_ID);
        Channel* getChannel(CHANNEL_NAME);
        void deleteUser(USER_ID);
        void deleteChannel(CHANNEL_NAME);
        bool isNicknameUsed(NICK_NAME);


        // void sendResponse(int fd, std::string massage);
        
        std::map<size_t, User *> const& getUsers();

        // ~Database();

};

#endif

//add to server `Command` Class with `execute` method