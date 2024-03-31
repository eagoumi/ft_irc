#include "Commands.hpp"




void Commands::topic()
{
    std::string channelName = getNextParam().first;
    std::string nickName = getNextParam().first;
    std::string theTopic = getNextParam().first;
    std::cout << "TOPIC1 = " << theTopic << std::endl;

    // if (command.size() < 2)
    // {
    //     sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
    //     return;
    // }

    if (db->getChannel(channelName) == NULL)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No such channel\n");

    else if (existMemberChannel(db->getUser(fd)->getNickName(), channelName) == false)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not on that channel\n");

    else if (theTopic == "")
    {
        puts("empty topic");
        if(db->getChannel(channelName)->getTopic() != "")
        {
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + db->getChannel(channelName)->getTopic() + "\n");
        }
        else if (db->getChannel(channelName)->getTopic() == "")
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No topic is set\n");
    }
    else if (theTopic == ":")
    {
        puts("clear topic");
        std::cout << "existOperqtor = " << existOperatorChannel(db->getUser(fd)->getNickName(), channelName) << " gettingModes = " << getMode("t", channelName) << std::endl;
        if (existOperatorChannel(db->getUser(fd)->getNickName(), channelName) == false && getMode("t", channelName) == true)
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
        else{
            db->getChannel(channelName)->setTopic("");
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " " + "topic cleared successfully\n");
        }
    }
    else if (theTopic[0] == ':')
    {
        puts("set topic");
        if (existOperatorChannel(db->getUser(fd)->getNickName(), channelName) == false && this->getMode("t", channelName) == true)
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
        else
        {
            db->getChannel(channelName)->setTopic(theTopic);
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " " + db->getChannel(channelName)->getTopic() + "\n");
        }
    }
}
