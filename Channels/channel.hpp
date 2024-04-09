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
        
        size_t                          limitMembers;
        size_t                          limitMembers;
        std::string                     newTopic;
        std::string                     _key;
        std::string                     _key;
        std::string                     newTopic;
        CHANNEL_NAME                    _name;
        std::set<char>                  _modeSet;
        std::set<char>                  _modeSet;
        std::map<char, bool>            modeSeted;
        std::map<USER_ID, User *>		_members;
        std::map<USER_ID, User *>		_operators;
        std::map<USER_ID, User *>		_invited;

    public:
        Channel(CHANNEL_NAME, User *);

        bool                                isUserInvited(USER_ID);
        bool                                isUserOperator(USER_ID);
        bool	                            isUserMember(USER_ID Id);
        bool	                            isKeyMatch(std::string const& givenKey);
        bool	                            isNickExist(std::string nick);
        bool                                getMode(const char& modeLetter);
        void                                addMember(User *);
        void                                inviteUser(User *);
        void	                            setTopic(std::string nTopic);
        void	                            setLimit(size_t limitMembers);
        void	                            addOperator(size_t fdo);
        void	                            deleteMember(User* userToDelete);
        void	                            deleteOperator(User *operatorToDelete);
        void	                            setKey(std::string const& givenKey);
        void	                            deleteInvited(User *userToDelete);
        void	                            clearKey();
        void                                setMode(const char& modeLetter);
        void                                removeMode(const char& modeLetter);
        User                                *getMember(USER_ID);
        size_t	                            getLimit();
        std::string                         getModeStr();
        std::string	                        getTopic();
        std::string                         getModes();
        CHANNEL_NAME const&                 getChannelName();
        std::map<USER_ID, User *> const&    getMembers();
        std::map<USER_ID, User *> const&    getOperators();

        ~Channel();
};

#endif
