# include "./user.hpp"
#include <sys/socket.h>
#include "../Channels/channel.hpp"
#include <utility>

typedef std::map<std::string, Channel *>::iterator ChanIter;

bool User::isStrContains(std::string const& str, std::string const& charSet) {

    for (int i = 0; charSet[i]; i++)
        if (str.find(charSet[i]) != std::string::npos) return true;
    return false;
}

bool User::isStrStartWith(std::string const& str, std::string const& charSet) {

    for (int i = 0; charSet[i]; i++)
        if (str.at(0) == charSet[i]) return true;
    return false;
}

User::User(){}

User::User(USER_ID const& Id) : _Id(Id) {
    _isPasswordInserted = false;
    _isUserNameInserted = false;
    _isNickInserted = false;
}

USER_ID const& User::getUserId() {
    return (this->_Id);
}

void User::setUserName(USER_NAME const& name) {
    _username = name;
    _isUserNameInserted = true;
}

USER_NAME const & User::getUserName() {
    return (_username);
}

void User::setNickName(NICK_NAME const& name) {
    _nickname = name;
    _isNickInserted = true;
}

NICK_NAME const & User::getNickName() {
    return (_nickname);
}

bool User::isAuthenticated()
{
    return (_isPasswordInserted == true && _isNickInserted == true && _isUserNameInserted == true);
}

void User::insertedPassSuccessfully()
{
    _isPasswordInserted = true;
}

bool User::hasInsertedPass()
{
    return (_isPasswordInserted);
}

bool User::hasInsertedNick()
{
    return (_isNickInserted);
}

bool User::hasInsertedUsername()
{
    return (_isUserNameInserted);
}

void User::joinedChannel(Channel* channel) {
    _joinedChannels.insert(std::make_pair(channel->getChannelName(), channel));
}

void User::partedChannel(Channel* channel) {
    ChanIter it = _joinedChannels.find(channel->getChannelName());
    if (it != _joinedChannels.end())
        _joinedChannels.erase(it);
}

std::map<std::string, Channel *>& User::getJoinedChannels() {
    return this->_joinedChannels;
}

void User::appendToCmdLine(CND_LINE const &bufferToAppend) {
   this->_cmdLine += bufferToAppend;
}

CND_LINE const& User::getCmdLine() {
    return _cmdLine;
}

void User::clearCmdLine() {
    _cmdLine.clear();
}

User::~User() {
}
