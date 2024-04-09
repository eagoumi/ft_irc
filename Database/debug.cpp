#include "./database.hpp"

typedef std::map<USER_ID, User *>::const_iterator         UserIter;
typedef std::map<std::string, Channel *>::const_iterator ChanIter;

void    Database::debug() {

    std::ofstream DbDebugFile("DATABASE_DEBUG.csv");

    DbDebugFile << "*Server Channels* :" << std::endl << std::endl;

    for (ChanIter chIt = _channels.begin(); chIt != _channels.end(); chIt++) {

        DbDebugFile << "\t" << chIt->first + ": [ " << chIt->second->getModeStr() << ", TOPIC:\"" + chIt->second->getTopic() + "\"" << std::endl << "\t\t";

        Channel *currChannel = chIt->second;
    
        DbDebugFile << "Operaters: ";
        for (UserIter userIt = currChannel->getOperators().begin(); userIt != currChannel->getOperators().end(); userIt++) {
            DbDebugFile << userIt->second->getNickName() + ", ";
        }

        DbDebugFile << "\n\t\tMembers: ";
        for (UserIter userIt = currChannel->getMembers().begin(); userIt != currChannel->getMembers().end(); userIt++) {
            DbDebugFile << userIt->second->getNickName() + ", ";
        }
        DbDebugFile << std::endl << "\t]" << std::endl << std::endl;
    }

    DbDebugFile << "=================================================================" << std::endl << std::endl;

    DbDebugFile << "*Server Users* :" << std::endl << "\t";

    for (UserIter userIt = _users.begin(); userIt != _users.end(); userIt++) {
            if (userIt->second->getNickName().empty() == false)
                DbDebugFile << userIt->second->getNickName() + ", ";
    }

    DbDebugFile.close();
}