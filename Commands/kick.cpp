#include "Commands.hpp"
#include "../error_request.hpp"

void Commands::displayMember(std::string channelName)
{
    std::map<size_t, User *> members;
    members = db->getChannel(channelName)->getMembers();
    for (std::map<size_t, User *>::iterator it = members.begin(); it != members.end(); it++)
    {
        std::cout << it->first << " nickname = " << it->second->getNickName() << std::endl;
    }
}

void Commands::kick()
{
    std::string channelName = getNextParam().first;
    std::string nickName = getNextParam().first;
    std::string reason = getNextParam().first;

    currChannel = db->getChannel(channelName);
    User* nickUser = db->existUser(nickName);
   
    if (currChannel == NULL)
    {    
        // sendResponse(fd, ":" + currUser->getNickName() + " " + channelName + " :No such channel\n");
        _logger.ServertoClient(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
        return ;
    }
    else if (currChannel->isUserMember(currUser->getUserId()) == false)
    {
        _logger.ServertoClient(ERR_NOTONCHANNEL(nickName, channelName));
        return ;
    }
    else if (currChannel->isUserOperator(currUser->getUserId()) == false)
    {
        _logger.ServertoClient(ERR_CHANOPRIVSNEEDED(nickName, channelName));
        return ;
    }
    if (currChannel->isNickExist(nickName) == false)
    {
        sendResponse(fd, ":" + currUser->getNickName() + " " + nickName /*client*/ + " " + channelName + " :They aren't on that channel\n");
        return ;
    }
    else if (currChannel->isUserOperator(nickUser->getUserId()) == true)
    {
        sendResponse(fd, ":" + currUser->getNickName() + " " + nickName /*client*/ + " " + channelName + " :You can't KICK the operator\n");
        return ;
    }
    else
    {
        User* kickedUser =db->existUser(nickName);
        displayMember(channelName);
        std::cout << std::endl;
        if (reason != "")
        {
            std::cout << "REASON = " << reason << std::endl;
            // SendMessageToMembers(currChannel, currUser, ERR_NOSUCHCHANNEL(nickName, channelName));
            SendMessageToMembers(currChannel, currUser, reason);

            // sendResponse(kickedFd, ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + ":" + reason + "\n");
            // sendResponse(fd, ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + " " + reason + "\n");
            sendResponse(fd, "user kicked succefully");
        }
        else
        {
            sendResponse(kickedUser->getUserId(), ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + "\n");
            sendResponse(fd, ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + "\n");
        }
        db->getChannel(channelName)->deleteMember(kickedUser);
        displayMember(channelName);
    }
}
