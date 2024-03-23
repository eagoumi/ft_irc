#ifndef __CHANNEL__HPP
#define __CHANNEL__HPP

#include <cstddef>
#include <map>
#include <string>

# define USER_ID size_t
# define CHANNEL_NAME std::string

class User;

class Channel
{
    private:
        Channel();
        CHANNEL_NAME					_name;
        std::map<USER_ID, User *>		_members;
        std::map<USER_ID, User *>		_operators;
        std::map<USER_ID, User *>		_invited;
        std::pair<bool, size_t>			_limit; //hmmm not sure if we have to implement another limit for how much channel a user could join to
        std::pair<bool, std::string>	_topic;
        std::pair<bool, std::string>	_key;

    public:
        Channel(CHANNEL_NAME, User *);
        void addMember(User *);
        void inviteUser(User *);
        bool isUserInvited(USER_ID);
        bool isUserOperator(USER_ID);
        User *getMember(USER_ID);
        //getChannelUsers operators etc ..
        ~Channel();
};

#endif