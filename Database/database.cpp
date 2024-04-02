#include "database.hpp"

Database *Database::database_ = NULL;

typedef std::map<std::string, Channel *>::iterator ChannelIter;
typedef std::map<USER_ID, User *>::iterator UserIter;
#define NOT_FOUND NULL


Database::Database()
{
}

Database *Database::GetInstance() {

    if (database_ == NULL)
    {
        database_ = new Database();
    }
    return database_;
}

User *Database::addNewUser(User *user) {

    USER_ID userId = user->getUserId();

    user == NULL ? throw std::string("db->addNewUser() -> user cannot be NULL") : NULL;
    getUser(userId) != NOT_FOUND ?  throw std::string("db->addNewUser() -> user already exist") : NULL;

    this->_users[userId] = user;
    return user;
}

Channel *Database::addNewChannel(CHANNEL_NAME name, User *user) {

    Channel *createdChannel;
    user == NULL ? throw std::string("db->addNewChannel() -> user cannot be NULL") : NULL;
    // since the channel take a user in its constructor, it has to assign that user as its operator
    createdChannel = new Channel(name, user);
    this->_channels[name] = createdChannel;

    user->joinedChannel(createdChannel);//srsly I see no need for this, at least for now
    return createdChannel;
}

User *Database::getUser(USER_ID Id) {

    UserIter it = this->_users.find(Id);
    if (it != this->_users.end())
        return it->second;
    return NULL;
}

Channel *Database::getChannel(CHANNEL_NAME nameToFind) {

    std::transform(nameToFind.begin(), nameToFind.end(), nameToFind.begin(), ::toupper);

    std::string currChanNameUpperCase;
    ChannelIter currChanIt = _channels.begin();
    while (currChanIt != _channels.end()) {
        const std::string& currChanName = currChanIt->first;
        std::transform(currChanName.begin(), currChanName.end(), currChanNameUpperCase.begin(), ::toupper);
        if (currChanNameUpperCase == nameToFind)
            return currChanIt->second;
        currChanIt++;
    }
    return NULL;
}

void Database::deleteUser(USER_ID Id) {

    UserIter it = this->_users.find(Id);
    delete it->second;
    this->_users.erase(it);
}

void Database::deleteChannel(CHANNEL_NAME nameToFind) {

    std::transform(nameToFind.begin(), nameToFind.end(), nameToFind.begin(), ::toupper);

    std::string currChanNameUpperCase;
    ChannelIter currChanIt = _channels.begin();
    while (currChanIt != _channels.end()) {
        const std::string& currChanName = currChanIt->first;
        std::transform(currChanName.begin(), currChanName.end(), currChanNameUpperCase.begin(), ::toupper);
        if (currChanNameUpperCase == nameToFind)
        {
            delete currChanIt->second;
            this->_channels.erase(currChanIt);
            break ;
        }
        currChanIt++;
    }

    // ChannelIter it = this->_channels.find(name);
    // delete it->second;
    // this->_channels.erase(it);
}

std::map<size_t, User *> const& Database::getUsers() {

    return this->_users;
}

bool Database::isNicknameUsed(NICK_NAME nameToFind) {

    std::transform(nameToFind.begin(), nameToFind.end(), nameToFind.begin(), ::toupper);

    UserIter currUserIt = this->_users.begin();
    while (currUserIt != this->_users.end()) {
        const std::string& currUserNickName = currUserIt->second->getNickName();
        std::transform(currUserNickName.begin(), currUserNickName.end(), nameToFind.begin(), ::toupper);
        if (currUserNickName == nameToFind)
            return true;
        currUserIt++;
    }
    return false;
}