#include "channel.hpp"
#include "../Users/user.hpp"
#include <algorithm>
#include <iostream>
#include <utility>

#define NOT_FOUND NULL
typedef std::map<USER_ID, User *>::iterator UserIter;

static bool isStrContains(std::string const &str, std::string const &charSet) {

    for (int i = 0; charSet[i]; i++)
        if (str.find(charSet[i]) != std::string::npos)
            return true;
    return false;
}

// static bool isStrStartWith(std::string const& str, std::string const& charSet) {

//     for (int i = 0; charSet[i]; i++)
//         if (str.at(0) == charSet[i]) return true;
//     return false;
// }

Channel::Channel(CHANNEL_NAME channelName, User *creator) {

    // db = Database::GetInstance();
    creator == NULL ? throw std::string("Channel::Channel() -> creator cannot be NULL") : NULL;
    // welp I think I'll add some extra work for checking name syntax
    isStrContains(channelName, " ,\a");
    // here I'll have to assign the creator to this channel
    USER_ID creatorId = creator->getUserId();
    // I think it is better to add the creator to both of them
    this->_members[creatorId] = creator;
    this->_operators[creatorId] = creator;
}

void Channel::addMember(User *member) {

    USER_ID memberId = member->getUserId();

    member == NULL ? throw std::string("Channel::addMember() -> member cannot be NULL") : NULL;
    getMember(memberId) != NOT_FOUND ? throw std::string("Channel::addMember() -> member already exist") : NULL;

    this->_members[memberId] = member;
    member->joinedChannel(this);
}

void Channel::inviteUser(User *user) {

    USER_ID userId = user->getUserId();

    user == NULL ? throw std::string("Channel::inviteUser() -> user cannot be NULL") : NULL;
    isUserInvited(userId) == true ? throw std::string("Channel::inviteUser() -> user already invited") : NULL;

    this->_invited[userId] = user;
}

bool Channel::isUserInvited(USER_ID Id) {

    UserIter it = this->_invited.find(Id);
    if (it != this->_invited.end())
        return true;
    return false;
}

bool Channel::isUserOperator(USER_ID Id) {

    UserIter it = this->_operators.find(Id);
    if (it != this->_operators.end())
        return true;
    return false;
}

bool Channel::isUserMember(USER_ID Id)
{
    UserIter it = this->_members.find(Id);
    if (it != this->_members.end())
        return true;
    return false;
}

CHANNEL_NAME const & Channel::getChannelName() {

    return this->_name;
}

User *Channel::getMember(USER_ID Id) {

    UserIter it = this->_members.find(Id);
    if (it != this->_members.end())
        return it->second;
    return NULL;
}

std::string Channel::getTopic() {

    return newTopic; 
}

void Channel::setTopic(std::string nTopic) {

    this->newTopic = nTopic;
}

std::map<USER_ID, User *> const& Channel::getMembers() {

    return this->_members;
}

void Channel::deleteMember(User *userToDelete) {
    std::map<USER_ID, User *>::iterator it = _members.find(userToDelete->getUserId());

    if(it != _members.end())
        _members.erase(it);
    
}


std::map<USER_ID, User *> const& Channel::getOperators()
{
    return this->_operators;
}

void Channel::setLimit(size_t nLimitMembers)
{
    this->limitMembers = nLimitMembers;
}

size_t Channel::getLimit()
{
    return this->limitMembers;
}

void Channel::addOperator(size_t fdo){
    this->_operators[fdo] = getMember(fdo);
    for(std::__1::map<size_t, User *>::iterator it = _operators.begin(); it != _operators.end(); it++)
        std::cout << "operator fd  = " << it->first << " nickname =  " << it->second->getNickName() << std::endl;
}

void Channel::deleteOperator(User *operatorToDelete) {
    std::map<USER_ID, User *>::iterator it = _operators.find(operatorToDelete->getUserId());

    if(it != _operators.end())
        _operators.erase(it);
    
}

Channel::~Channel()
{
}
