#include "./database.hpp"

typedef std::map<USER_ID, User *>::const_iterator         UserIter;
typedef std::map<std::string, Channel *>::const_iterator ChanIter;



void    Database::debug() {

    std::ofstream DbDebugFile("DATABASE_DEBUG.csv");

    DbDebugFile << "*Server Channels* :" << std::endl << std::endl;

    for (ChanIter chIt = _channels.begin(); chIt != _channels.end(); chIt++) {
        DbDebugFile << chIt->first + ": [" << std::endl;
        Channel *currChannel = chIt->second;
        for (UserIter userIt = currChannel->getMembers().begin(); userIt != currChannel->getMembers().end(); userIt++) {
            DbDebugFile << userIt->second->getNickName() + ", ";
        }
        DbDebugFile << std::endl << "]" << std::endl << std::endl;
    }

    DbDebugFile << "=================================================================" << std::endl << std::endl;

    DbDebugFile << "*Server Users* :" << std::endl;

    for (UserIter userIt = _users.begin(); userIt != _users.end(); userIt++) {
            DbDebugFile << userIt->second->getNickName() + ", ";
    }

    DbDebugFile.close();
}