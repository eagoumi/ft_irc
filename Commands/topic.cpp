#include "Commands.hpp"




void Commands::topic()
{
    std::string channelName = getNextParam().first;
    currChannel = db->getChannel(channelName);
    std::string theTopic = getNextParam().first;
    std::cout << "TOPIC1 = " << theTopic << std::endl;

    // if (command.size() < 2)
    // {
    //     sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
    //     return;
    // }

    if (db->getChannel(channelName) == NULL)
        currUser->ServertoClients(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No such channel\n");
    // else if (existMemberChannel(db->getUser(fd)->getNickName(), channelName) == false)
    else if (currChannel->isUserMember(currUser->getUserId()) == false) //checking if the member are on the channel use this "currChannel->getMember(currUser->getUserId())"
        currUser->ServertoClients(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), channelName));
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not on that channel\n");
    else if (theTopic == "")
    {
        puts("empty topic");
        if(db->getChannel(channelName)->getTopic() != "")
        {
            currUser->ServertoClients(RPL_TOPIC(db->getUser(fd)->getNickName(), channelName, db->getChannel(channelName)->getTopic()));
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + db->getChannel(channelName)->getTopic() + "\n");
        }
        else if (db->getChannel(channelName)->getTopic() == "")
            currUser->ServertoClients(RPL_NOTOPIC(db->getUser(fd)->getNickName(), channelName));
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No topic is set\n");
    }
    else if (theTopic == ":")
    {
        puts("clear topic");
        // if (existOperatorChannel(db->getUser(fd)->getNickName(), channelName) == false && getMode("t", channelName) == true)
        if (currChannel->isUserOperator(currUser->getUserId()) == false && currChannel->getMode('t') == true)
        {
            currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
        }   
        else{
            db->getChannel(channelName)->setTopic("");
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " " + "topic cleared successfully\n");
        }
    }
    else if (theTopic[0] == ':') //Problem Here if not :
    {
        // puts("set topic");
        if (currChannel->isUserOperator(currUser->getUserId()) == false && currChannel->getMode('t') == true)
        {
            currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
        }   
        else
        {
            db->getChannel(channelName)->setTopic(theTopic);
            currUser->IRCPrint(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + currUser->GetIpAddress() + " TOPIC " + channelName + " :" + db->getChannel(channelName)->getTopic());
            currUser->ServertoClients(RPL_TOPIC(db->getUser(fd)->getNickName(), channelName, db->getChannel(channelName)->getTopic()));
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " " + db->getChannel(channelName)->getTopic() + "\n");
        }
    }
}
