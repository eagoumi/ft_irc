# include "./user.hpp"
# include "../Channels/channel.hpp"
# include "../Database/database.hpp"

void User::joinChannel(Channel *channel) {
    
}

User::User() {

}

Channel* User::createChannel(CHANNEL_NAME name) {

    Database *database = Database::GetInstance();
    Channel *createdChannel = new Channel(this);
    return database->addNewChannel(name, new Channel(this));
}

User::~User() {

}
