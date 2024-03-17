#include "database.hpp"

typedef std::map<std::string, Channel*>::iterator ChannelsIter;
typedef std::map<USER_ID, User*>::iterator UserIter;


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
    user == NULL ? throw std::string("db.addNewUser() -> user cannot be NULL") : NULL;
    this->users[Id] = user;
    return user;
}

Channel* Database::addNewChannel(CHANNEL_NAME name, Channel* channel) {
    this->channels[name] = channel;
    return channel;
}

User* Database::getUser(USER_ID Id) {
    UserIter it = this->users.find(Id);
    if (it != this->users.end())
        return it->second;
    return NULL;
}

Channel* Database::getChannel(CHANNEL_NAME name) {
    ChannelsIter it = this->channels.find(name);
    if (it != this->channels.end())
        return it->second;
    return NULL;
}