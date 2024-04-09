#ifndef __CHANNEL__HPP
#define __CHANNEL__HPP

#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>

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
        std::string                     _key;
        std::map<char, bool>            modeSeted;
        std::string                     newTopic;
        std::set<char>                  _modeSet;
        size_t                          limitMembers;

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
        std::map<USER_ID, User *> const& getMembers();
        std::map<USER_ID, User *> const& getOperators();
        std::string	getTopic();
        void		setTopic(std::string nTopic);
        void		setLimit(size_t limitMembers);
        void		addOperator(size_t fdo);
        void		deleteMember(User* userToDelete);
        void		deleteOperator(User *operatorToDelete);
        size_t		getLimit();
        bool		isUserMember(USER_ID Id);
        void		setKey(std::string const& givenKey);

        bool		isKeyMatch(std::string const& givenKey);
        bool		isNickExist(std::string nick);
        void		deleteInvited(User *userToDelete);
        void		clearKey();
        std::string getModes();
        ~Channel();
};

#endif




