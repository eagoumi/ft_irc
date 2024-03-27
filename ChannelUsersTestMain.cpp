#include "./Channels/channel.hpp"
#include "./Users/user.hpp"
#include "Database/database.hpp"
#include <sys/signal.h>
# include <iostream>

# define NOT_FOUND NULL

/* Compile using c++ ChannelUsersTestMain.cpp Database/database.cpp Channels/channel.cpp Users/user.cpp */

int main(int argc, char **argv) {
    try {

        Database* db = Database::GetInstance();

        int EliasUserId = 1337;
        User *user = db->addNewUser(new User(EliasUserId));
        user->setUserName("ilias");
        std::cout << db->getUser(EliasUserId)->getUserName() << std::endl;

        int yUserId = 42;
        User *user2 = db->addNewUser(new User(yUserId));
        user->setUserName("youssra");
        std::cout << db->getUser(yUserId)->getUserName() << std::endl;


        //when `/join #uWu` command sent
        Channel* channel = db->getChannel("uWu");
        if (channel == NOT_FOUND) {
            std::cout << "channel not found, creating ...\n";
            // channel = user->createChannel("uWu");
            db->addNewChannel("uWu", user);
            if (db->getChannel("uWu") != NULL)
                std::cout << "Channel uWu has been created\n";
        }
        else {
            //ila kanet channel already kayna 4ay5ess user joini liha
            std::cout << "channel found, joining ...\n";
            channel->addMember(user);
        }

        channel = db->getChannel("uWu");
        if (channel == NOT_FOUND) {
            std::cout << "channel not found, creating ...\n";
            // channel = user->createChannel("uWu");
            db->addNewChannel("uWu", user);
            if (db->getChannel("uWu") != NULL)
                std::cout << "Channel uWu has been created\n";
        }
        else {
            //ila kanet channel already kayna 4ay5ess user joini liha
            std::cout << "channel found, joining ...\n";
            std::cout << user2->getUserName() << " joined the channel" << std::endl;
            channel->addMember(user2);
        }
    }
    catch (std::string errMsg) {
        std::cout << errMsg << std::endl;
    }
    return (0);
}