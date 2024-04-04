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
        currUser->ServertoClients(ERR_NOSUCHCHANNEL(nickName, channelName));
    }
    else if (currChannel->isUserMember(currUser->getUserId()) == false)
    {
        currUser->ServertoClients(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), channelName));
        return ;
    }
    else if (currChannel->isUserOperator(currUser->getUserId()) == false)
    {
        currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
        return ;
    }
    else if(currChannel->isUserMember(nickUser->getUserId()) == true)
    {
        currUser->ServertoClients(RPL_ALREADYONCHANNEL(db->getUser(fd)->getNickName(), nickName, channelName));
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
            currUser->ServertoClients(RPL_NOUSERS(db->getUser(fd)->getNickName(), channelName));
    }
}



