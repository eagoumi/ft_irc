#include "Commands.hpp"

void Commands::invite()
{
    std::string nickName = getNextParam().first;
    std::string channelName = getNextParam().first;
    // std::cout << "CHANEEEEL NAME = " << channelName << std::endl;
    // std::cout << "CHANEEEEL NAME = " << nickName << std::endl;
    User* nickUser = db->existUser(nickName);
    currChannel = db->getChannel(channelName);
    if (db->getChannel(channelName) == NULL)
    {
        _logger.ServertoClient(ERR_NOSUCHCHANNEL(nickName, channelName));
    }
    else if (currChannel->isUserMember(currUser->getUserId()) == false)
    {
        _logger.ServertoClient(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), channelName));
        return ;
    }
    else if (currChannel->isUserOperator(currUser->getUserId()) == false)
    {
        _logger.ServertoClient(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
        return ;
    }
    else if(currChannel->isUserMember(nickUser->getUserId()) == true)
    {
        _logger.ServertoClient(RPL_ALREADYONCHANNEL(db->getUser(fd)->getNickName(), nickName, channelName));
        return ;
    }
    else{
        User* invitedUser = db->existUser(nickName);
        if(invitedUser)
        {
            sendResponse(invitedUser->getUserId(), ":" + db->getUser(fd)->getNickName() + " " + nickName + " " + channelName + "\n"); //LOGS STILL HERE WITH ME
            db->getChannel(channelName)->inviteUser(invitedUser);
        }   
        else
            _logger.ServertoClient(RPL_NOUSERS(db->getUser(fd)->getNickName(), channelName));
    }
}



