#ifndef __CHANNEL__HPP
#define __CHANNEL__HPP

#include <map>
// # include "../Users/user.hpp"

# define USER_ID int
# define CHANNEL_NAME std::string

class User;

class Channel
{
    private:
        Channel();
        std::map<USER_ID, User *> joinedUsers;
        std::map<USER_ID, User *> operators;
        std::map<USER_ID, User *> invited;
        
        //since we have just two modes define one data type for operator
        /* map or set of operators */

        /*  */
    public:
        Channel(User *creator);
        void addUser(User *member);
        ~Channel();
};

#endif