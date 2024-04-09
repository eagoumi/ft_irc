#include "Commands.hpp"

void Commands::invite()
{
    std::string nickName = getNextParam().first;
    std::string channelName = getNextParam().first;
    currChannel = db->getChannel(channelName);
    if (currChannel == NULL)
    {
        _logger.ServertoClient(ERR_NOSUCHCHANNEL(nickName, channelName));
        return;
    }
    else if (currChannel->isUserMember(currUser->getUserId()) == false)
    {
        _logger.ServertoClient(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), channelName));
        return;
    }
    else if (currChannel->isUserOperator(currUser->getUserId()) == false && currChannel->getMode('i') == true)
    {
        _logger.ServertoClient(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
        return;
    }
    if (currChannel->isNickExist(nickName) == true)
    {
        _logger.ServertoClient(RPL_ALREADYONCHANNEL(db->getUser(fd)->getNickName(), nickName, channelName));
        return;
    }
    else
    {
        User *invitedUser = db->existUser(nickName);
        if (!invitedUser)
            _logger.ServertoClient(ERR_NOUSERS(db->getUser(fd)->getNickName(), channelName));
        else
        {
            currChannel->inviteUser(invitedUser);
            _logger.ServertoClient(RPL_INVITING(db->getUser(fd)->getNickName(), invitedUser->getNickName(), currChannel->getChannelName()));
            _logger.ServertoClient("NOTICE @"+currChannel->getChannelName() + " :" + db->getUser(fd)->getNickName() + " invited " + invitedUser->getNickName() + " into channel " + currChannel->getChannelName());
        }
    }
}
