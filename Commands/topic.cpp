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
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No such channel\n");
            currUser->ServertoClients(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
    // else if (existMemberChannel(db->getUser(fd)->getNickName(), channelName) == false)
    else if (currChannel->isUserMember(currUser->getUserId()) == false)
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
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No topic is set\n");
            currUser->ServertoClients(RPL_NOTOPIC(db->getUser(fd)->getNickName(), channelName));
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
    else if (theTopic[0] == ':')
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
            currUser->ServertoClients(RPL_TOPIC(db->getUser(fd)->getNickName(), channelName, db->getChannel(channelName)->getTopic()));
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " " + db->getChannel(channelName)->getTopic() + "\n");
        }
    }
}
