#include "Commands.hpp"




void Commands::topic()
{
    std::string channelName = getNextParam().first;
    currChannel = db->getChannel(channelName);
    std::string theTopic = getNextParam().first;
    currChannel = db->getChannel(channelName);


    if (db->getChannel(channelName) == NULL)
        _logger.ServertoClient(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
    else if (currChannel->isUserMember(currUser->getUserId()) == false) //checking if the member are on the channel use this "currChannel->getMember(currUser->getUserId())"
        _logger.ServertoClient(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), channelName));
    else if (_paramCounter == 2)
    {
        if(db->getChannel(channelName)->getTopic() != "")
        {
            _logger.ServertoClient(RPL_TOPIC(db->getUser(fd)->getNickName(), channelName, db->getChannel(channelName)->getTopic())); // Problem Here A Yousra
        }
        else if (db->getChannel(channelName)->getTopic() == "")
            _logger.ServertoClient(RPL_NOTOPIC(db->getUser(fd)->getNickName(), channelName)); // Problem Here A Yousra
    }
    else if (theTopic == "" && _paramCounter == 3)
    {
        if (currChannel->isUserOperator(currUser->getUserId()) == false && currChannel->getMode('t') == true)
        {
            _logger.ServertoClient(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
        }   
        else{
            db->getChannel(channelName)->setTopic("");
            SendMessageToMembers(currChannel, currUser, "TOPIC " + channelName + " :");
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " " + "topic cleared successfully\n");
        }
    }
    else// if (theTopic[0] == ':')
    {
        if (currChannel->isUserOperator(currUser->getUserId()) == false && currChannel->getMode('t') == true)
        {
            _logger.ServertoClient(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
        }   
        else
        {
            db->getChannel(channelName)->setTopic(theTopic);
            SendMessageToMembers(currChannel, currUser, "TOPIC " + channelName + " " + db->getChannel(channelName)->getTopic());
            _logger.ServertoClient(RPL_TOPIC(db->getUser(fd)->getNickName(), channelName, db->getChannel(channelName)->getTopic()));
            // _logger.IRCPrint(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + _logger.getServerIP() + " TOPIC " + channelName + " :" + db->getChannel(channelName)->getTopic());
        }
    }
}
