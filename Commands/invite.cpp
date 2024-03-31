#include "Commands.hpp"


size_t Commands::existUser(std::string nick)
{
    std::map<size_t, User *> users;
    users = db->getUsers();
    for (std::map<size_t, User *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if(nick == it->second->getNickName())
            return it->first;
    }
    return 0;
}

void Commands::invite()
{
    // std::cout << "NICK = " << getNick() << std::endl;
    if (command.size() < 3)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
        return;
    }
    else if (db->getChannel(this->getChannel()) == NULL)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " :No such channel\n");
    }
    else if (existMemberChannel(db->getUser(fd)->getNickName()) == false)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not on that channel\n");
    }
    else if (existOperatorChannel(db->getUser(fd)->getNickName()) == false)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not channel operator\n");
    }
    else if(existMemberChannel(getNick()) == true)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getNick() + " " + getChannel() + " :is already on channel\n");
    }
    else{
        size_t inviteFd = existUser(getNick());
        if(inviteFd != 0)
        {
            sendResponse(inviteFd, ":" + db->getUser(fd)->getNickName() + " " + getNick() + " " + getChannel() + "\n");
            db->getChannel(this->getChannel())->setInvitedNick(getNick());
            // std::cout << "INVITEDNICK = " << invitedNick << std::endl;
        }   
        else
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " :User does not exist\n");
    }
}



