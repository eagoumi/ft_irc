#include "./Channels/channel.hpp"
#include "./Users/user.hpp"

typedef std::map<std::string, Channel*>::iterator ChannelsIter;

int main(int argc, char **argv) {

    std::map<USER_ID, User*> users;
    std::map<std::string, Channel*> channels;

    int EliasUserId = 1337;
    users[EliasUserId] = new User(EliasUserId);
    
    //when `/join #uWu` command sent
    ChannelsIter retrievedChannel = channels.find("uWu");
    if (retrievedChannel == channels.end()) {
        //channel creati fiha a new channel with specifing the operator
        /*
            channels["uWu"] = new Channel(users[EliasUserId]);
            users[EliasUserId]->joinChannel(channels["uWu"]);
            */
        channels["uWu"] = users[EliasUserId]->createChannel();
    }
    else {
        //ila kanet channel already kayna 4ay5ess user joini liha
        /*
            retrievedChannel->second->addUser();
            users[EliasUserId]->joinChannel(retrievedChannel->second);
        */
        retrievedChannel->second->addUser(users[EliasUserId]);

    }
    return (0);
}