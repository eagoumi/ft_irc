#include "Commands.hpp"

void Commands::topic()
{
    std::string channelName = getNextParam().first;
    std::string theTopic = getNextParam().first;
    currChannel = db->getChannel(channelName);

    if (currChannel == NULL)
        _logger.ServertoClient(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
    else if (currChannel->isUserMember(currUser->getUserId()) == false)
        _logger.ServertoClient(ERR_NOTONCHANNEL(currUser->getNickName(), channelName));
    else if (_paramCounter == 2)
    {
        if (currChannel->getTopic() != "")
        {
            _logger.ServertoClient(RPL_TOPIC(currUser->getNickName(), channelName, currChannel->getTopic()));
        }
        else if (currChannel->getTopic() == "")
            _logger.ServertoClient(RPL_NOTOPIC(currUser->getNickName(), channelName));
    }
    else if (theTopic == "" && _paramCounter == 3)
    {
        if (currChannel->isUserOperator(currUser->getUserId()) == false && currChannel->getMode('t') == true)
        {
            _logger.ServertoClient(ERR_CHANOPRIVSNEEDED(currUser->getNickName(), channelName));
        }
        else
        {
            currChannel->setTopic("");
            SendMessageToMembers(currChannel, currUser, "TOPIC " + channelName + " :");
        }
    }
    else
    {
        if (currChannel->isUserOperator(currUser->getUserId()) == false && currChannel->getMode('t') == true)
        {
            _logger.ServertoClient(ERR_CHANOPRIVSNEEDED(currUser->getNickName(), channelName));
        }
        else
        {
            currChannel->setTopic(theTopic);
            SendMessageToMembers(currChannel, currUser, "TOPIC " + channelName + " " + currChannel->getTopic());
            _logger.ServertoClient(RPL_TOPIC(currUser->getNickName(), channelName, currChannel->getTopic()));
        }
    }
}
