#include "./Channels/channel.hpp"
#include "./Users/user.hpp"

int main(int argc, char **argv) {

    std::map<USER_ID, User*> users;
    std::map<std::string, Channel*> channels;

    int EliasUserId = 1337;
    users[EliasUserId] = new User(EliasUserId);
    
    //when `/join #uWu` command sent
    if (channels.find("uWu") == channels.end()) {
        //channel creati fiha a new channel with specifing the operator
        channels["uWu"] = new Channel();
        users[EliasUserId]->joinChannel(channels["uWu"]);
    }
    else {
        //5ass 
    }
    return (0);
}