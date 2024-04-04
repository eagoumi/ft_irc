#include "Commands.hpp"


// size_t Commands::existUser(std::string nick)
// {
//     std::transform(nick.begin(), nick.end(), nick.begin(), ::toupper);
//     std::map<size_t, User *> users;
//     users = db->getUsers();
//     std::cout << "CHANEEEEL NAME = 2" << nick << std::endl;
//     for (std::map<size_t, User *>::iterator it = users.begin(); it != users.end(); it++)
//     {
//         std::string currUserNickCapitilized;
//         std::string currUserNick = it->second->getNickName();
//         std::transform(currUserNick.begin(), currUserNick.end(), currUserNickCapitilized.begin(), ::toupper);
//         if(currUserNickCapitilized == nick)
//             return it->first;
//     }
//     return 0;
// }

void Commands::invite()
{
    // std::cout << "NICK = " << nickName << std::endl;
    // if (command.size() < 3)
    // {
    //     sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
    //     return;
    // }
    std::string nickName = getNextParam().first;
    std::string channelName = getNextParam().first;
    std::cout << "CHANEEEEL NAME = " << channelName << std::endl;
    std::cout << "CHANEEEEL NAME = " << nickName << std::endl;
    User* nickUser = db->existUser(nickName);
    currChannel = db->getChannel(channelName);
    if (db->getChannel(channelName) == NULL)
    {
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No such channel\n");
        currUser->ServertoClients(ERR_NOSUCHCHANNEL(nickName, channelName));
    }
    // else if (existMemberChannel(db->getUser(fd)->getNickName(), channelName) == false)
    else if (currChannel->isUserMember(currUser->getUserId()) == false)
    {
        currUser->ServertoClients(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), channelName));
        return ;
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not on that channel\n");
    }
    // else if (existOperatorChannel(db->getUser(fd)->getNickName(), channelName) == false)
    else if (currChannel->isUserOperator(currUser->getUserId()) == false)
    {
        currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
        return ;
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
    }
    else if(currChannel->isUserMember(nickUser->getUserId()) == true)
    {
        currUser->ServertoClients(RPL_ALREADYONCHANNEL(db->getUser(fd)->getNickName(), nickName, channelName));
        return ;
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + nickName + " " + channelName + " :is already on channel\n");
    }
    else{
        // size_t inviteFd = existUser(nickName);
        User* invitedUser = db->existUser(nickName);
        if(invitedUser)
        {
            sendResponse(invitedUser->getUserId(), ":" + db->getUser(fd)->getNickName() + " " + nickName + " " + channelName + "\n"); //LOGS STILL HERE WITH ME
            // db->getChannel(channelName)->setInvitedNick(nickName);
            db->getChannel(channelName)->inviteUser(invitedUser);
            // std::cout << "INVITEDNICK = " << invitedNick << std::endl;
        }   
        else
            currUser->ServertoClients(RPL_NOUSERS(db->getUser(fd)->getNickName(), channelName));
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :User does not exist\n");
    }
}



