#include "channel.hpp"
#include "../Users/user.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>

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

    /**************************/
    /* MODIFIED BY TOFA7A SRY */
    /**************************/
    // modeSeted['i']; modeSeted['t']; modeSeted['k']; modeSeted['o']; modeSeted['l'];
    _modeSet.insert('+'); _modeSet.insert('t');
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
    {
        return true;
    }   
    return false;
}

bool Channel::isNickExist(std::string nick)
{
    for(std::map<size_t, User *>::iterator it = _members.begin(); it != _members.end(); it++)
    { 
        if (nick == it->second->getNickName())
            return true;
    }
    return false;
}

bool Channel::isUserMember(USER_ID Id)
{
    UserIter it = this->_members.find(Id);
    if (it != this->_members.end())
    {
      return true;
    }
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

std::string Channel::getTopic() {

    return newTopic; 
}

void Channel::setTopic(std::string nTopic) {

    this->newTopic = nTopic;
}

void Channel::setLimit(size_t nLimitMembers)
{
    this->limitMembers = nLimitMembers;
}

size_t Channel::getLimit()
{
    return this->limitMembers;
}

void Channel::setKey(std::string const& givenKey) {
    _key = givenKey;
}

bool Channel::isKeyMatch(std::string const& givenKey) {

    if (givenKey == _key) return true;
    return false;
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


/******************************/
/* MODIFIED BY TOFA7A SRY     */
/* This method does two       */
/* things : set and get       */
/* I'll try to split it       */
/******************************/

// std::map<std::string, bool> Channel::gettingModes(std::string toFind)
// {
//     std::string::size_type it = modeS.find(toFind);

//     if (it != std::string::npos)
//     {
//         it--;
//         if (modeS[it] == '+' || (modeS[0] == '+' && modeS[it] != '-'))
//         {
//             modeSeted[toFind] = true;
//         }
//         else
//         {
//             puts("test4");
//             modeSeted[toFind] = false;
//         }
//     }
//     return modeSeted;
// }

/*************************/
/* updated version of    */
/* both get and set      */
/*************************/
bool Channel::getMode(const char& modeLetter) {

    if (_modeSet.find(modeLetter) != _modeSet.end())
        return true;
    return (false);
}

void Channel::setMode(const char& modeLetter) {

    if (_modeSet.empty())   _modeSet.insert('+');
                            _modeSet.insert(modeLetter);
}

void Channel::removeMode(const char& modeLetter) {

                                _modeSet.erase(modeLetter);
    if (_modeSet.size() == 1)   _modeSet.erase('+');
}

std::string Channel::getModeStr() {

    std::ostringstream stream;
    std::copy(_modeSet.begin(), _modeSet.end(), std::ostream_iterator<char>(stream));
    return stream.str();
}

/*****************************/
/* MODIFIED BY TOFA7A SRY    */
/* I think no need for it    */
/* since we can create a     */
/* set method for that       */
/* which will parse modeStr  */
/* then set the right values */
/*****************************/
// void Channel::initializeModes(std::string modeStr)
// {
//     modeS = modeStr;
    // modeSeted["i"]; <<<<<<<< those moved to Channel() Param constructer
    // modeSeted["t"]; <<
    // modeSeted["k"]; <<
    // modeSeted["o"]; <<
    // modeSeted["l"]; <<
// }

Channel::~Channel()
{
}
