#ifndef __CHANNEL__HPP
#define __CHANNEL__HPP

#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>
// #include "../Commands/Commands.hpp"
// #include "../Database/database.hpp"
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
        std::map<char, bool> modeSeted;
        // std::vector<std::string> invitedList;
        std::string newTopic;
        std::set<char> _modeSet;
        size_t limitMembers;

    public:
        Channel(CHANNEL_NAME, User *);
        void addMember(User *);
        void inviteUser(User *);
        bool isUserInvited(USER_ID);
        bool isUserOperator(USER_ID);
        User *getMember(USER_ID);
        CHANNEL_NAME const & getChannelName();
        void setMode(const char& modeLetter);
        void removeMode(const char& modeLetter);
        bool getMode(const char& modeLetter);
        std::string getModeStr();

        //added
        std::map<USER_ID, User *> const& getMembers();
        std::map<USER_ID, User *> const& getOperators();
        // std::map<std::string, bool> 	 gettingModes(std::string toFind);
        std::string                 	 getTopic();
        void                        	 deleteMember(User* userToDelete);
        void                        	 setTopic(std::string nTopic);
        // void                        	 initializeModes(std::string modeStr);
        void                        	 setLimit(size_t limitMembers);
        void                             addOperator(size_t fdo);
        void                             deleteOperator(User *operatorToDelete);
        size_t                      	 getLimit();
        bool                             isUserMember(USER_ID Id);
        // bool                             isUserMember(std::string nick);

        // std::string                 channelTopic(int fd);



        //getChannelUsers operators etc ..
        ~Channel();
};

#endif




