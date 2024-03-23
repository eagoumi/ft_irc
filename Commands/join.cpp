#include "Commands.hpp"

void Commands::join(){
    User *user;
    db = Database::GetInstance();
    
    std::string key = command[2];
    db->addNewChannel(getChannel(), user);
    // std::cout << "KEY = " << command[2] << "  " << getChannel() << std::endl;
    db->displayChannels();
}