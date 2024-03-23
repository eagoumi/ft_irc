#include "channel.hpp"
#include "../Users/user.hpp"

# define NOT_FOUND NULL
typedef std::map<USER_ID, User*>::iterator UserIter;

//to remove later
Channel::Channel(){
    
}

Channel::Channel(User *creator) {

    creator == NULL ? throw std::string("Channel::Channel() -> creator cannot be NULL") : NULL;
    //here I'll have to assign the creator to this channel
    USER_ID creatorId = creator->getUserId();
    //I think it is better to add the creator to both of them
    this->_members[creatorId] = creator;
    this->_operators[creatorId] = creator;
}

void Channel::addMember(User *member) {

    USER_ID memberId = member->getUserId();
    
    member == NULL ? throw std::string("Channel::addMember() -> member cannot be NULL") : NULL;
    getMember(memberId) != NOT_FOUND ? throw std::string("Channel::addMember() -> member already exist") : NULL;

    this->_members[memberId] = member;
}

User* Channel::getMember(USER_ID Id) {

    UserIter it = this->_members.find(Id);
    if (it != this->_members.end())
        return it->second;
    return NULL;
}

Channel::~Channel() {
    
}