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

Channel::Channel(CHANNEL_NAME channelName, User *creator) : _name(channelName) {

    // db = Database::GetInstance();
    // creator == NULL ? throw std::string("Channel::Channel() -> creator cannot be NULL") : NULL;
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

    // member == NULL ? throw std::string("Channel::addMember() -> member cannot be NULL") : NULL;
    // getMember(memberId) != NOT_FOUND ? throw std::string("Channel::addMember() -> member already exist") : NULL;

    this->_members[memberId] = member;
    member->joinedChannel(this);
}

//Problem when we invite user twice
void Channel::inviteUser(User *user) {

    USER_ID userId = user->getUserId();

    // user == NULL ? throw std::string("Channel::inviteUser() -> user cannot be NULL") : NULL;
    // isUserInvited(userId) == true ? throw std::string("Channel::inviteUser() -> user already invited") : NULL;

    this->_invited[userId] = user;
    for(std::map<size_t, User *>::iterator it = _invited.begin(); it != _invited.end(); it++)
        std::cout << "INVITED = " << it->second->getNickName() << std::endl;
}

bool Channel::isUserInvited(USER_ID Id) {

    UserIter it = this->_invited.find(Id);
    if (it != this->_invited.end())
        return true;
    return false;
}

void Channel::deleteInvited(User *userToDelete) {
    std::map<USER_ID, User *>::iterator it = _invited.find(userToDelete->getUserId());

    if(it != _invited.end())
        _invited.erase(it);
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

void Channel::clearKey()
{
    _key.clear();
}

void Channel::setKey(std::string const& givenKey) {
    _key = givenKey;
}

bool Channel::isKeyMatch(std::string const& givenKey) {
    std::cout << "givenkey = " << givenKey << " key = " << _key << std::endl;
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

std::string Channel::getModes(){
    std::string _modes;
    for (std::set<char>::iterator it = _modeSet.begin(); it != _modeSet.end(); it++ ) 
     {  
        std::cout << "MOODE IN GETMODES = " << *it << std::endl; 
        _modes += *it;
        std::cout << "STRING IN GETMODES = " << _modes << std::endl; 
     }
    return _modes;
}

bool Channel::getMode(const char& modeLetter) {
    for(std::set<char>::iterator it = _modeSet.begin(); it != _modeSet.end(); it++)
        std::cout << "MOOOODE = " << *it << std::endl;
    if (_modeSet.find(modeLetter) != _modeSet.end())
        return true;
    return (false);
}

void Channel::setMode(const char& modeLetter) {

    if (_modeSet.empty())   
        _modeSet.insert('+');
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

Channel::~Channel()
{
}
