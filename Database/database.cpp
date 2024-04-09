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
    {
        database_ = new Database();
    }
    return database_;
}

User *Database::addNewUser(User *user)
{

    USER_ID userId = user->getUserId();

    // user == NULL ? throw std::string("db->addNewUser() -> user cannot be NULL") : NULL;
    // getUser(userId) != NOT_FOUND ?  throw std::string("db->addNewUser() -> user already exist") : NULL;

    this->_users[userId] = user;
    return user;
}

Channel *Database::addNewChannel(CHANNEL_NAME name, User *user)
{

    Channel *createdChannel;
    // user == NULL ? throw std::string("db->addNewChannel() -> user cannot be NULL") : NULL;
    // getChannel(name) != NOT_FOUND ?  throw std::string("db->addNewChannel() -> channel already exist") : NULL;

    // since the channel take a user in its constructor, it has to assign that user as its operator
    createdChannel = new Channel(name, user);
    this->_channels[name] = createdChannel;

    user->joinedChannel(createdChannel); // srsly I see no need for this, at least for now
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

    // std::cout << "TOFIND = " << nameToFind << std::endl;
    // ChannelIter currChanItD = _channels.begin();
    // std::cout << "Channels:" << std::endl;
    // while (currChanItD != _channels.end()) {
    //     std::cout << "Channel : [" << currChanItD->first << "]" << std::endl;
    //     currChanItD++;
    // }

    std::transform(nameToFind.begin(), nameToFind.end(), nameToFind.begin(), ::toupper);

    ChannelIter currChanIt = _channels.begin();
    while (currChanIt != _channels.end())
    {
        std::string currChanNameUpperCase;
        const std::string &currChanName = currChanIt->first;
        // currChanNameUpperCase = currChanName;
        std::transform(currChanName.begin(), currChanName.end(), std::back_inserter(currChanNameUpperCase), ::toupper);
        // std::cout << currChanNameUpperCase + " =? " + nameToFind << std::endl;
        if (currChanNameUpperCase == nameToFind)
            return currChanIt->second;
        currChanIt++;
    }
    return NULL;
}

void Database::deleteUser(USER_ID Id)
{
    UserIter it = this->_users.find(Id);
    std::map<std::string, Channel *> userChannels;
    userChannels = getUser(Id)->getJoinedChannels();
    for (std::map<std::string, Channel *>::iterator ite = userChannels.begin(); ite != userChannels.end(); ite++)
    {
        ite->second->deleteMember(getUser(Id));
        if (ite->second->isUserOperator(Id) == true)
            ite->second->deleteOperator(getUser(Id));
        if(getChannel(ite->first)->getMembers().size() == 0)
        {
            deleteChannel(ite->first);
        }
    }
    if (it != this->_users.end())
    {
        delete it->second;
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

    // ChannelIter it = this->_channels.find(name);
    // delete it->second;
    // this->_channels.erase(it);
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
    puts("12");
    // SGF CHECING FOR !@#!!#$#@$%!$% CHARACTERS
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
    puts("13");
    return 0;
}
