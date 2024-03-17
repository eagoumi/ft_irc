#include "database.hpp"

typedef std::map<std::string, Channel*>::iterator ChannelsIter;


Database* Database::database_ = NULL;

Database::Database() {
    
}

Database* Database::GetInstance() {
    if(database_ == NULL) {
        database_ = new Database();
    }
    return database_;
}

User* Database::addNewUser(USER_ID Id, User* user) {
    this->users[Id] = user;
    return user;
}

Channel* Database::addNewChannel(CHANNEL_NAME name, Channel* channel) {
    this->channels[name] = channel;
    return channel;
}

User* Database::getUser(USER_ID) {
    return new User;
}

Channel* Database::getChannel(CHANNEL_NAME name) {
    ChannelsIter it = this->channels.find(name);
    if (it != this->channels.end())
        return it->second;
    return NULL;
}