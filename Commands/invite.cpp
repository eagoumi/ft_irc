#include "Commands.hpp"


size_t Commands::existUser(std::string nick)
{
    std::transform(nick.begin(), nick.end(), nick.begin(), ::toupper);
    std::map<size_t, User *> users;
    users = db->getUsers();
    std::cout << "CHANEEEEL NAME = 2" << nick << std::endl;
    for (std::map<size_t, User *>::iterator it = users.begin(); it != users.end(); it++)
    {
        std::cout << "WHYYYYYYYY = " << it->second->getNickName() << std::endl;
        if(nick == it->second->getNickName())
            return it->first;
    }
    return 0;
}

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

    if (db->getChannel(channelName) == NULL)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No such channel\n");
    }
    else if (existMemberChannel(db->getUser(fd)->getNickName(), channelName) == false)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not on that channel\n");
    }
    else if (existOperatorChannel(db->getUser(fd)->getNickName(), channelName) == false)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
    }
    else if(existMemberChannel(nickName, channelName) == true)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + nickName + " " + channelName + " :is already on channel\n");
    }
    else{
        size_t inviteFd = existUser(nickName);
        if(inviteFd != 0)
        {
            sendResponse(inviteFd, ":" + db->getUser(fd)->getNickName() + " " + nickName + " " + channelName + "\n");
            db->getChannel(channelName)->setInvitedNick(nickName);
            // std::cout << "INVITEDNICK = " << invitedNick << std::endl;
        }   
        else
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :User does not exist\n");
    }
}



