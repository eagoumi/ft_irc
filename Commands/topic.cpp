#include "Commands.hpp"




void Commands::topic()
{
    std::string channelName = getNextParam().first;
    currChannel = db->getChannel(channelName);
    std::string theTopic = getNextParam().first;
    currChannel = db->getChannel(channelName);


    if (db->getChannel(channelName) == NULL)
        currUser->ServertoClients(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
    else if (currChannel->isUserMember(currUser->getUserId()) == false) //checking if the member are on the channel use this "currChannel->getMember(currUser->getUserId())"
        currUser->ServertoClients(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), channelName));
    else if (theTopic == "")
    {
        if(db->getChannel(channelName)->getTopic() != "")
        {
            currUser->ServertoClients(RPL_TOPIC(db->getUser(fd)->getNickName(), channelName, db->getChannel(channelName)->getTopic()));
        }
        else if (db->getChannel(channelName)->getTopic() == "")
            currUser->ServertoClients(RPL_NOTOPIC(db->getUser(fd)->getNickName(), channelName));
    }
    else if (theTopic == ":")
    {
        puts("clear topic");
        if (currChannel->isUserOperator(currUser->getUserId()) == false && currChannel->getMode('t') == true)
        {
            currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
        }   
        else{
            db->getChannel(channelName)->setTopic("");
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " " + "topic cleared successfully\n");
        }
    }
    else// if (theTopic[0] == ':')
    {
        if (currChannel->isUserOperator(currUser->getUserId()) == false && currChannel->getMode('t') == true)
        {
            currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
        }   
        else
        {
            db->getChannel(channelName)->setTopic(theTopic);
            currUser->IRCPrint(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + currUser->GetIpAddress() + " TOPIC " + channelName + " :" + db->getChannel(channelName)->getTopic());
            currUser->ServertoClients(RPL_TOPIC(db->getUser(fd)->getNickName(), channelName, db->getChannel(channelName)->getTopic()));
        }
    }
}
