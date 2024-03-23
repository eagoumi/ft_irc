#ifndef __CHANNEL__HPP
#define __CHANNEL__HPP

#include <cstddef>
#include <map>

# define USER_ID size_t
# define CHANNEL_NAME std::string

class User;

class Channel
{
    private:
        // Channel();
        std::map<USER_ID, User *> _members;
        std::map<USER_ID, User *> _operators;
        std::map<USER_ID, User *> _invited;
            
        //since we have just two modes define one data type for operator
        /* map or set of operators */

        /*  */
    public:
        Channel();

        Channel(User *);
        void addMember(User *);
        User *getMember(USER_ID);
        //getChannelUsers operators etc ..
        ~Channel();
};

#endif