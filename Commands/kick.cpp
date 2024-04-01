#include "Commands.hpp"
#include "../error_request.hpp"

void Commands::displayMember(std::string channelName)
{
    std::map<size_t, User *> members;
    members = db->getChannel(channelName)->getMembers();
    for (std::map<size_t, User *>::iterator it = members.begin(); it != members.end(); it++)
    {
        std::cout << it->first << " nickname = " << it->second->getNickName() << std::endl;
    }
}

bool Commands::existMemberChannel(std::string member, std::string channelName)
{
    std::map<size_t, User *> members;
    members = db->getChannel(channelName)->getMembers();
    for (std::map<size_t, User *>::iterator it = members.begin(); it != members.end(); it++)
    {
        if (member == it->second->getNickName())
            return true;
    }
    return false;
}
bool Commands::existOperatorChannel(std::string nick, std::string channelName)
{
    std::map<size_t, User *> operators;
    // int flag;
    operators = db->getChannel(channelName)->getOperators();
    for (std::map<size_t, User *>::iterator it = operators.begin(); it != operators.end(); it++)
    {
        if (nick == it->second->getNickName())
            return true;
    }
    return false;
}

void Commands::kick()
{
    std::string channelName = getNextParam().first;
    std::string nickName = getNextParam().first;
    // if (command.size() < 3)
    // {
    //     sendResponse(fd, ERR_NEEDMOREPARAMS(currUser->getNickName(), getCommand()) + "\n");
    //     // sendResponse(fd, ":" + currUser->getNickName() + " " + getCommand() + " :Not enough parameters\n");
    //     return;
    // }
    if (db->getChannel(channelName) == NULL)
        sendResponse(fd, ":" + currUser->getNickName() + " " + channelName + " :No such channel\n");

    else if (existMemberChannel(currUser->getNickName(), channelName) == false)
    {
        sendResponse(fd, ":" + currUser->getNickName() /*client*/ + " " + channelName + " :You're not on that channel\n");
    }

    else if (existOperatorChannel(currUser->getNickName(), channelName) == false)
    {
        sendResponse(fd, ":" + currUser->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
    }
    else if (existMemberChannel(nickName, channelName) == false)
    {
        sendResponse(fd, ":" + currUser->getNickName() + " " + nickName /*client*/ + " " + channelName + " :They aren't on that channel\n");
    }

    else if (existOperatorChannel(nickName, channelName) == true)
    {
        sendResponse(fd, ":" + currUser->getNickName() + " " + nickName /*client*/ + " " + channelName + " :You can't KICK the operator\n");
    }

    else
    {
        size_t kickedFd = existUser(nickName);
        puts("here is the segv2");
        displayMember(channelName);
        std::cout << "\n"
                  << std::endl;
        db->getChannel(channelName)->deleteMember(nickName);
        puts("here is the segv3");
        displayMember(channelName);
        // if (getComment() != "")
        // {
        //     sendResponse(kickedFd, ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + ":" + getComment() + "\n");
        //     sendResponse(fd, ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + ":" + getComment() + "\n");
        // }
        // else
        // {
        sendResponse(kickedFd, ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + "\n");
        sendResponse(fd, ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + "\n");
        // }
    }
}
