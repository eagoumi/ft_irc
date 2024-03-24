#include "Commands.hpp"

void Commands::join(){
    User *user = NULL;
    // db = Database::GetInstance();
    std::string key = command[2];
    db->addNewChannel(getChannel(), user);
}