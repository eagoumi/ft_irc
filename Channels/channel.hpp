#ifndef __CHANNEL__HPP
#define __CHANNEL__HPP

#include <cstddef>
#include <map>
#include <string>
#include "../Commands/Commands.hpp"
#include "../Database/database.hpp"
// #include "../Users/user.hpp"

# define USER_ID size_t
# define CHANNEL_NAME std::string

class User;

class Channel
{
    private:
        // Database *db;
        Channel();
        CHANNEL_NAME					_name;
        std::map<USER_ID, User *>		_members;
        std::map<USER_ID, User *>		_operators;
        std::map<USER_ID, User *>		_invited;
        std::pair<bool, size_t>			_limit; //hmmm not sure if we have to implement another limit for how much channel a user could join to
        std::pair<bool, std::string>	_topic;
        std::pair<bool, std::string>	_key;

        //added
        std::map<std::string, bool> modeSeted;
        std::string newTopic;
        std::string mode;

    public:
        Channel(CHANNEL_NAME, User *);
        void addMember(User *);
        void inviteUser(User *);
        bool isUserInvited(USER_ID);
        bool isUserOperator(USER_ID);
        User *getMember(USER_ID);

        //added
        std::map<USER_ID, User *>   getMembers();
        std::map<USER_ID, User *>   getOperators();
        std::map<std::string, bool> gettingModes(char toFind);
        void                        deleteMember(std::string nickTarget);
        void                        setTopic(std::string nTopic);
        void                        initializeModes(std::string modeStr);
        std::string                 getTopic();
        // std::string                 channelTopic(int fd);



        //getChannelUsers operators etc ..
        ~Channel();
};

#endif




