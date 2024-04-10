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

Channel::Channel(CHANNEL_NAME channelName, User *creator) : _name(channelName) {

    isStrContains(channelName, " ,\a");
    USER_ID creatorId = creator->getUserId();
    this->_members[creatorId] = creator;
    this->_operators[creatorId] = creator;

    _modeSet.insert('+'); _modeSet.insert('t');
}

void Channel::addMember(User *member) {

    USER_ID memberId = member->getUserId();

    this->_members[memberId] = member;
    member->joinedChannel(this);
}

void Channel::inviteUser(User *user) {

    USER_ID userId = user->getUserId();

    this->_invited[userId] = user;
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
        return true;
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

std::map<USER_ID, User *> const& Channel::getMembers() {

    return this->_members;
}

void Channel::deleteMember(User *userToDelete) {

    if (userToDelete) {

        std::map<USER_ID, User *>::iterator it = _members.find(userToDelete->getUserId());
        userToDelete->partedChannel(this);
        if(it != _members.end())
            _members.erase(it);
    }
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
    if (givenKey == _key) return true;
    return false;
}

std::string Channel::getKey(){
    return _key;
}

void Channel::addOperator(size_t fdo){
    
    this->_operators[fdo] = getMember(fdo);
}

void Channel::deleteOperator(User *operatorToDelete) {
    std::map<USER_ID, User *>::iterator it = _operators.find(operatorToDelete->getUserId());

    if(it != _operators.end())
        _operators.erase(it);
}

std::string Channel::getModes(){
    std::string _modes;
    for (std::set<char>::iterator it = _modeSet.begin(); it != _modeSet.end(); it++ ) 
       _modes += *it;
    return _modes;
}

bool Channel::getMode(const char& modeLetter) {
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
