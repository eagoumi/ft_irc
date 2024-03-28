#include "./Channels/channel.hpp"
#include "./Users/user.hpp"
#include "Database/database.hpp"
#include <sys/signal.h>
# include <iostream>
#include <vector>

# define NOT_FOUND NULL

/* Compile using c++ ChannelUsersTestMain.cpp Database/database.cpp Channels/channel.cpp Users/user.cpp */

void join(std::string chanName, int userFd) {
    
    Database* db = Database::GetInstance();
    User *currUser = db->getUser(userFd);

    //when `/join #uWu` command sent
    Channel* channel = db->getChannel(chanName);
    if (channel == NOT_FOUND) {

        std::cout << "channel not found, creating by " << currUser->getUserName() << " ...\n";
        db->addNewChannel(chanName, currUser);
        //Check if channel exists or not
        if (db->getChannel(chanName) != NULL)
            std::cout << "Channel " << chanName << " has been created\n\n";
    }
    else {
        if (channel->getMember(userFd) != NULL)
        {
            std::cout << "user " << currUser->getUserName() << " already member in " << chanName << std::endl << std::endl;
            return ;
        }
        //ila kanet channel already kayna 4ay5ess user joini liha
        std::cout << "channel found, joining ...\n";
        channel->addMember(currUser);

        //jut debug msg to check if
        std::cout << "user " << channel->getMember(userFd)->getUserName() << " has joined to " << chanName << std::endl << std::endl;
    }
}


int main(int argc, char **argv) {
    try {

        Database* db = Database::GetInstance();

        User *elias = new User(3);
        elias->setUserName("elias");
        db->addNewUser(elias);

        User *agoumi = new User(4); 
        agoumi->setUserName("agoumi");
        db->addNewUser(agoumi);

        User *youssra = new User(5); 
        youssra->setUserName("youssra");
        db->addNewUser(youssra);

        join("irc", elias->getUserId());
        join("irc", elias->getUserId());
        join("42", agoumi->getUserId());
        
    }
    catch (std::string errMsg) {
        std::cout << "ERR EXCEPTION : " << errMsg << std::endl;
    }
    return (0);
}