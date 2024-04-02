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

// bool Commands::existMemberChannel(std::string member, std::string channelName)
// {
//     std::map<size_t, User *> members;
//     members = db->getChannel(channelName)->getMembers();
//     for (std::map<size_t, User *>::iterator it = members.begin(); it != members.end(); it++)
//     {
//         if (member == it->second->getNickName())
//             return true;
//     }
//     return false;
// }
// bool Commands::existOperatorChannel(std::string nick, std::string channelName)
// {
//     std::map<size_t, User *> operators;
//     // int flag;
//     operators = db->getChannel(channelName)->getOperators();
//     for (std::map<size_t, User *>::iterator it = operators.begin(); it != operators.end(); it++)
//     {
//         std::cout << "OPERATORS = " << it->second->getNickName() << "NICK ENTRED = " << nick << std::endl;
//         if (nick == it->second->getNickName())
//             return true;
//     }
//     return false;
// }

void Commands::kick()
{
    std::string channelName = getNextParam().first;
    std::string nickName = getNextParam().first;
    std::string reason = getNextParam().first;
    std::cout << "REASON = " << reason << std::endl; 
    if(reason[0] == ':')
        reason.erase(0, 1);
    currChannel = db->getChannel(channelName);
    User* nickUser = db->existUser(nickName);
    // if (command.size() < 3)
    // {
    //     sendResponse(fd, ERR_NEEDMOREPARAMS(currUser->getNickName(), getCommand()) + "\n");
    //     // sendResponse(fd, ":" + currUser->getNickName() + " " + getCommand() + " :Not enough parameters\n");
    //     return;
    // }
    if (db->getChannel(channelName) == NULL)
        // sendResponse(fd, ":" + currUser->getNickName() + " " + channelName + " :No such channel\n");
            currUser->ServertoClients(ERR_NOSUCHCHANNEL(nickName, channelName));
    // else if (existMemberChannel(currUser->getNickName(), channelName) == false)
    else if (currChannel->isUserMember(currUser->getUserId()) == false)
    {
        currUser->ServertoClients(ERR_NOTONCHANNEL(nickName, channelName));
        // sendResponse(fd, ":" + currUser->getNickName() /*client*/ + " " + channelName + " :You're not on that channel\n");
    }

    else if (currChannel->isUserOperator(currUser->getUserId()) == false)
    {
        currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(nickName, channelName));
        // sendResponse(fd, ":" + currUser->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
        // sendResponse(fd, ERR_CHANOPRIVSNEEDED(nickName, channelName) + "\n");
    }
    // else if (existMemberChannel(nickName, channelName) == false)
    else if (currChannel->isUserMember(nickUser->getUserId()) == false)
    {
        sendResponse(fd, ":" + currUser->getNickName() + " " + nickName /*client*/ + " " + channelName + " :They aren't on that channel\n");
    }

    // else if (existOperatorChannel(nickName, channelName) == true)
    else if (currChannel->isUserOperator(nickUser->getUserId()) == true)
    {
        sendResponse(fd, ":" + currUser->getNickName() + " " + nickName /*client*/ + " " + channelName + " :You can't KICK the operator\n");
    }

    else
    {
        User* kickedUser =db->existUser(nickName);
        displayMember(channelName);
        std::cout << std::endl;
        if (reason != "")
        {
            std::cout << "REASON = " << reason << std::endl;
            // SendMessageToMembers(currChannel, currUser, ERR_NOSUCHCHANNEL(nickName, channelName));
            SendMessageToMembers(currChannel, currUser, reason);

            // sendResponse(kickedFd, ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + ":" + reason + "\n");
            // sendResponse(fd, ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + " " + reason + "\n");
            sendResponse(fd, "user kicked succefully");
        }
        else
        {
            sendResponse(kickedUser->getUserId(), ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + "\n");
            sendResponse(fd, ":" + currUser->getNickName() + " KICK " + channelName + " " + nickName + "\n");
        }
        db->getChannel(channelName)->deleteMember(kickedUser);
        displayMember(channelName);
    }
}
