#include "Commands.hpp"

void Commands::join(){
    std::cout << "FD = "  << fd << std::endl;
    User *user = new User(fd);
    std::string key = command[1];
    getChannel();
    db->addNewChannel("agoumi", user);
}