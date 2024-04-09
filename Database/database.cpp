#include "database.hpp"

Database *Database::database_ = NULL;

typedef std::map<std::string, Channel *>::iterator ChannelIter;
typedef std::map<USER_ID, User *>::iterator UserIter;
#define NOT_FOUND NULL

Database::Database()
{
}

Database *Database::GetInstance()
{
    if (database_ == NULL)
        database_ = new Database();
    return database_;
}

User *Database::addNewUser(User *user)
{
    USER_ID userId = user->getUserId();
    this->_users[userId] = user;
    return user;
}

Channel *Database::addNewChannel(CHANNEL_NAME name, User *user)
{
    Channel *createdChannel;
    createdChannel = new Channel(name, user);
    this->_channels[name] = createdChannel;

    user->joinedChannel(createdChannel);
    return createdChannel;
}

User *Database::getUser(USER_ID Id)
{
    UserIter it = this->_users.find(Id);
    if (it != this->_users.end())
        return it->second;
    return NULL;
}

Channel *Database::getChannel(CHANNEL_NAME nameToFind)
{
    std::transform(nameToFind.begin(), nameToFind.end(), nameToFind.begin(), ::toupper);

    ChannelIter currChanIt = _channels.begin();
    while (currChanIt != _channels.end())
    {
        std::string currChanNameUpperCase;
        const std::string &currChanName = currChanIt->first;
        std::transform(currChanName.begin(), currChanName.end(), std::back_inserter(currChanNameUpperCase), ::toupper);
        if (currChanNameUpperCase == nameToFind)
            return currChanIt->second;
        currChanIt++;
    }
    return NULL;
}

void Database::deleteUser(USER_ID Id)
{
    UserIter it = this->_users.find(Id);
    if (it != this->_users.end())
    {
        User* currUser = it->second;
        std::map<std::string, Channel *> userChannels = it->second->getJoinedChannels();
        for (ChannelIter currChanIt = userChannels.begin(); currChanIt != userChannels.end(); currChanIt++)
        {
            Channel* currChan = currChanIt->second;
            currChan->deleteMember(currUser);
            if (currChan->isUserOperator(Id) == true)
                currChan->deleteOperator(currUser);
            if (currChan->getMembers().size() == 0)
                deleteChannel(currChan->getChannelName());
        }
        delete currUser;
        this->_users.erase(it);
    }
}

void Database::deleteChannel(CHANNEL_NAME nameToFind)
{
    std::transform(nameToFind.begin(), nameToFind.end(), nameToFind.begin(), ::toupper);

    ChannelIter currChanIt = _channels.begin();
    while (currChanIt != _channels.end())
    {
        std::string currChanNameUpperCase;
        const std::string &currChanName = currChanIt->first;
        std::transform(currChanName.begin(), currChanName.end(), std::back_inserter(currChanNameUpperCase), ::toupper);
        if (currChanNameUpperCase == nameToFind)
        {
            delete currChanIt->second;
            this->_channels.erase(currChanIt);
            break;
        }
        currChanIt++;
    }
}

std::map<size_t, User *> const &Database::getUsers()
{
    return this->_users;
}

bool Database::isNicknameUsed(NICK_NAME nameToFind)
{
    std::transform(nameToFind.begin(), nameToFind.end(), nameToFind.begin(), ::toupper);

    UserIter currUserIt = this->_users.begin();
    while (currUserIt != this->_users.end())
    {
        std::string currUserNickNameUpperCase;
        const std::string &currUserNickName = currUserIt->second->getNickName();
        std::transform(currUserNickName.begin(), currUserNickName.end(), std::back_inserter(currUserNickNameUpperCase), ::toupper);
        if (currUserNickNameUpperCase == nameToFind)
            return true;
        currUserIt++;
    }
    return false;
}

User *Database::existUser(std::string nick)
{
    std::transform(nick.begin(), nick.end(), nick.begin(), ::toupper);
    for (std::map<size_t, User *>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        std::string currUserNickCapitilized;
        std::string currUserNick = it->second->getNickName();
        std::transform(currUserNick.begin(), currUserNick.end(), std::back_inserter(currUserNickCapitilized), ::toupper);
        if (currUserNickCapitilized == nick)
        {
            return it->second;
        }
    }
    return 0;
}
