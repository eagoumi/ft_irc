#include "Commands.hpp"
#include "../error_request.hpp"

void Commands::kick()
{
    std::string channelName = getNextParam().first;
    std::string nickName = getNextParam().first;
    std::string reason = getNextParam().first;
    currChannel = db->getChannel(channelName);
    
    User *nickUser = db->existUser(nickName);
    if (reason == "")
        reason = ":" + currUser->getNickName();

    if (currChannel == NULL)
    {
        _logger.ServertoClient(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
        return;
    }
    else if (currChannel->isUserMember(currUser->getUserId()) == false)
    {
        _logger.ServertoClient(ERR_NOTONCHANNEL(nickName, channelName));
        return;
    }
    else if (currChannel->isUserOperator(currUser->getUserId()) == false)
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
        currChannel->deleteMember(nickUser);
        currChannel->deleteInvited(nickUser);
    }
}

