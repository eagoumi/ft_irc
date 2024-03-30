#include "Commands.hpp"




void Commands::topic()
{
    std::cout << "TOPIC1 = " << getTopic() << std::endl;

    if (command.size() < 2)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
        return;
    }

    if (db->getChannel(this->getChannel()) == NULL)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " :No such channel\n");

    else if (existMemberChannel(db->getUser(fd)->getNickName()) == false)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not on that channel\n");

    else if (getTopic() == "")
    {
        puts("empty topic");
        if(db->getChannel(this->getChannel())->getTopic() != "")
        {
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + db->getChannel(this->getChannel())->getTopic() + "\n");
        }
        else if (db->getChannel(this->getChannel())->getTopic() == "")
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " :No topic is set\n");
    }
    else if (getTopic() == ":")
    {
        puts("clear topic");
        std::cout << "existOperqtor = " << existOperatorChannel(db->getUser(fd)->getNickName()) << " gettingModes = " << getMode("t") << std::endl;
        if (existOperatorChannel(db->getUser(fd)->getNickName()) == false && getMode("t") == true)
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not channel operator\n");
        else{
            db->getChannel(this->getChannel())->setTopic("");
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " " + "topic cleared successfully\n");
        }
    }
    else if (getTopic()[0] == ':')
    {
        puts("set topic");
        if (existOperatorChannel(db->getUser(fd)->getNickName()) == false && this->getMode("t") == true)
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not channel operator\n");
        else
        {
            db->getChannel(this->getChannel())->setTopic(getTopic());
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " " + db->getChannel(this->getChannel())->getTopic() + "\n");
        }
    }
}
