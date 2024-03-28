#include "Commands.hpp"
#include "../error_request.hpp"

void Commands::displayMember()
{
    std::map<size_t, User *> members;
    members = db->getChannel(this->getChannel())->getMembers();
    for (std::map<size_t, User *>::iterator it = members.begin(); it != members.end(); it++)
    {
        std::cout << it->first << " nickname = " << it->second->getNickName() << std::endl;
    }
}

bool Commands::existMemberChannel(std::string member)
{
    std::map<size_t, User *> members;
    members = db->getChannel(this->getChannel())->getMembers();
    for (std::map<size_t, User *>::iterator it = members.begin(); it != members.end(); it++)
    {
        if (member == it->second->getNickName())
            return true;
    }
    return false;
}
bool Commands::existOperatorChannel(std::string nick)
{
    std::map<size_t, User *> operators;
    // int flag;
    operators = db->getChannel(this->getChannel())->getOperators();
    for (std::map<size_t, User *>::iterator it = operators.begin(); it != operators.end(); it++)
    {
        if (nick == it->second->getNickName())
            return true;
    }
    return false;
}

void Commands::kick()
{

    if (command.size() < 3)
    {
        sendResponse(fd, ERR_NEEDMOREPARAMS(db->getUser(fd)->getNickName(), getCommand()));
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
        return;
    }

    if (db->getChannel(this->getChannel()) == NULL)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " :No such channel\n");

    else if (existMemberChannel(db->getUser(fd)->getNickName()) == false)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not on that channel\n");

    else if (existOperatorChannel(db->getUser(fd)->getNickName()) == false)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not channel operator\n");

    else if (existMemberChannel(this->getNick()) == false)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getNick() /*client*/ + " " + getChannel() + " :They aren't on that channel\n");

    else if (existOperatorChannel(getNick()) == true)
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getNick() /*client*/ + " " + getChannel() + " :You can't KICK the operator\n");

    else
    {
        // displayMember();
        db->getChannel(this->getChannel())->deleteMember(getNick());
        // displayMember();
        if (getCommentTopic() != "") 
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " KICK " + getChannel() + " " + getNick() + ":" + getCommentTopic() + "\n");
        else
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " KICK " + getChannel() + " " + getNick() + "\n");
    }
}