#ifndef __DATABASE__HPP
#define __DATABASE__HPP

# include <map>
# include "../Users/user.hpp"
# include "../Channels/channel.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>
#include <sys/socket.h>
#include <algorithm>

# ifndef DEBUG
#  define DEBUG true
# endif
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

        User  *existUser(std::string nick);
        std::map<size_t, User *> const& getUsers();

        void    debug();

};

#endif
