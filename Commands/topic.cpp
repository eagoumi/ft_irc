#include "Commands.hpp"

std::string Commands::newTopic = "";

void Commands::topic()
{
    // std::cout << "FLAG = " << flag << std::endl;

    if(flag  == 0)
    {
        newTopic = getCommentTopic();
        // std::cout << "TOPIC1 = " << newTopic << std::endl;
        flag = 1;
    }
    // std::cout << "TOPIC1 = " << newTopic << std::endl;

    if (command.size() < 2)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
        return;
    }
    if (db->getChannel(this->getChannel()) == NULL)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " :No such channel\n");
    else if (existMemberChannel(db->getUser(fd)->getNickName()) == false)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not on that channel\n");
    else if (existOperatorChannel(db->getUser(fd)->getNickName()) == false)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not channel operator\n");
    else if(getCommentTopic() == "")
    {
        // std::cout << "TOPIC2 = " << newTopic << std::endl;
        if(newTopic == "")
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " :No topic is set\n");
        else{    
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + newTopic + "\n");
        }
    }
    else if(getCommentTopic() == ":")
    {
        // puts("hhhhhh");
        seTopic("");
        newTopic.clear();
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " " + getCommentTopic() + "\n");
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " " + "topic cleared successfully\n");
    }
    else if(getCommentTopic()[0] == ':')
    {
        newTopic = getCommentTopic();
        // std::cout << "TOPIC3 = " << newTopic << std::endl;
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " " + getCommentTopic() + "\n");
    }

}