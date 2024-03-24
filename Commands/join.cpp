#include "Commands.hpp"

void Commands::join(){
    User *user = new User(fd);
    std::string key = command[1];
    getChannel();
    db->addNewChannel("agoumi", user);
}