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
    User *nickUser = db->existUser(nickName);
    if (reason == "")
        reason = ":" + currUser->getNickName(); // maybe hoka reason = ":" + currUser->getNickName();

    if (currChannel == NULL)
    {
        // sendResponse(fd, ":" + currUser->getNickName() + " " + channelName + " :No such channel\n");
        _logger.ServertoClient(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
        return;
    }
    else if (currChannel->isUserMember(currUser->getUserId()) == false)
    {
        _logger.ServertoClient(ERR_NOTONCHANNEL(nickName, channelName));
        return;
    }
    else if (currChannel->isUserOperator(currUser->getUserId()) == false) //Problem here
    {
        _logger.ServertoClient(ERR_CHANOPRIVSNEEDED(nickName, channelName));
        return;
    }
    if (currChannel->isNickExist(nickName) == false)
    {
        _logger.ServertoClient(ERR_USERNOTINCHANNEL(currUser->getNickName(), nickName, channelName));
        return;
    }
    else if (currChannel->isUserOperator(nickUser->getUserId()) == true)
    {
        _logger.ServertoClient(ERR_CANNOTKICKOP(currUser->getNickName(), channelName));
        return;
    }
    else
    {
        SendMessageToMembers(currChannel, currUser, "KICK " + currChannel->getChannelName() + " " + nickUser->getNickName() + " " + reason);
        db->getChannel(channelName)->deleteMember(nickUser);
        db->getChannel(channelName)->deleteInvited(nickUser);
        // SendMessageToMembers(currChannel, currUser, reason);
        // sendResponse(kickedUser->getUserId(), ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + reason + "\n");
        // :yousra!~0@oq7f3s33.btgd4imj.mjbqqhjk.ip KICK #hello sara :yousra
        // _logger.ServertoClient(":" + currUser->getNickName() + "!~@" + _logger.getServerIP() + " KICK " + channelName + " " + nickName + ":" + reason + "\n");
    }
}

// delete displayy members function