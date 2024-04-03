#include "Commands.hpp"
#include <vector>

#define NOT_FOUND NULL

std::string Commands::invitedNick;

void Commands::join()
{
	// _db->getChannel(".");

    // std::cout << "join() : beg" << std::endl;
    std::vector<std::string> channelNamesList = getNextParam().second;
    // std::cout << "join() : line 2" << std::endl;
    std::vector<std::string> channelkeysList = getNextParam().second;
    for (size_t channelIndex = 0; channelIndex < channelNamesList.size(); channelIndex++)
    {
        std::cout << "join() : loop " << channelNamesList[channelIndex] << std::endl;
        std::cout << "join() : user nickname: " + currUser->getNickName() << std::endl;

        currChannel = db->getChannel(channelNamesList[channelIndex]);
        if (currChannel == NOT_FOUND)
        {
            std::cout << "channel not found, creating by " << currUser->getNickName() << " ...\n";
            db->addNewChannel(channelNamesList[channelIndex], currUser);
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + currUser->GetIpAddress() + " JOIN " + channelNamesList[channelIndex] + "\n");
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + getHostName() + " MODE +nt" + channelNamesList[channelIndex] + "\n");//MODE #blahmeow +nt
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + getHostName() + " MODE +nt " + channelNamesList[channelIndex]+ '\n');
            
            // sendResponse(fd, ":" + currUser->GetIpAddress() + " MODE " + channelNamesList[channelIndex] + " +t\n");
            currUser->ServertoClients(RPL_NAMREPLY(db->getUser(fd)->getNickName(),channelNamesList[channelIndex],"@" + db->getUser(fd)->getNickName()));
            currUser->ServertoClients(RPL_ENDOFNAMES(db->getUser(fd)->getNickName(), channelNamesList[channelIndex]));

            // sendResponse(fd, ":" + currUser->GetIpAddress() + " 353 " + db->getUser(fd)->getNickName() + " @ " + channelNamesList[channelIndex] + " :" + "@tofa7A, agoumi, yousra\n");
            // sendResponse(fd, ":" + currUser->GetIpAddress() + " 366 " + db->getUser(fd)->getNickName() + channelNamesList[channelIndex] + " :End of /NAMES list.\n");
            // sendResponse(fd, ":" + currUser->GetIpAddress() + " 332 " + db->getUser(fd)->getNickName() + " " + channelNamesList[channelIndex] + " :" + db->getChannel(channelNamesList[channelIndex])->getTopic() + "\n");
            // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + currUser->GetIpAddress() + "  has changed mode: +s " + channelNamesList[channelIndex] + "\n");
            // currUser->ServertoClients( ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + getHostName() + " JOIN " + channelNamesList[channelIndex] + "\n");
            // currUser->ServertoClients(currUser->getNickName() + " has changed mode: +s\n");
        }
        else
        {
            // std::cout << "getMode = " << getMode("i") << " getInvited = " << currChannel->getInvitedNick(currUser->getNickName()) << std::endl;
            if (currChannel->getMember(fd) != NULL)
            {
                sendResponse(fd, "User already in channel\n");
                continue;
            }
            else if (this->getMode("i", channelNamesList[channelIndex]) == true && currChannel->isUserInvited(currUser->getUserId()) == false)
            {
                currUser->ServertoClients(ERR_INVITEONLYCHAN(db->getUser(fd)->getNickName(),channelNamesList[channelIndex]));
                // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelNamesList[channelIndex] + " :Cannot join channel (+i)\n");
            }
            else if(getMode("l", channelNamesList[channelIndex]) == true && currChannel->getLimit() <= currChannel->getMembers().size())
            {
                currUser->ServertoClients(ERR_CHANNELISFULL(db->getUser(fd)->getNickName(),channelNamesList[channelIndex]));
                // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelNamesList[channelIndex] + " :Cannot join channel (+l)\n");
            }
            else
            {
                // std::cout << "TEST = " << currChannel->getChannelName() << std::endl;
                currChannel->addMember(currUser);

                std::map<USER_ID, User *> Members = currChannel->getMembers();
                std::map<USER_ID, User *>::iterator It_Members = Members.begin();
                std::string MemberStr;
                int i = 0;
                for (; It_Members != Members.end(); ++It_Members)
                {
                    if (currChannel->isUserOperator(It_Members->first))
                    {
                        MemberStr += "@";
                    }
                    MemberStr += It_Members->second->getNickName() + " ";
                    // if (!MemberStr.empty())
                    //     MemberStr.pop_back();
                }
                SendMessageToMembers(currChannel, currUser, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + currUser->GetIpAddress() + " JOIN :" + channelNamesList[channelIndex]);
                // sendResponse(fd, ":" + MemberStr + "!~" + db->getUser(fd)->getUserName() + "@" + currUser->GetIpAddress() + " JOIN :" + channelNamesList[channelIndex] + "\n");
                currUser->ServertoClients(RPL_NAMREPLY(db->getUser(fd)->getNickName(),channelNamesList[channelIndex],MemberStr)); //how to pranting all list of members on the channel 
                // sendResponse(fd, db->getUser(fd)->getNickName() + " Joined successfully\n");
            }
        }
    }
}

// void Commands::join()
// {
//     flag = 0;
//     // User *user = new User(fd);
//     User *user = db->getUser(fd);
//     std::map<std::string, Channel *> listChan;
//     std::map<std::string, std::string>::iterator it;
//     std::map<std::string, std::string> channel;
//     // std::map<USER_ID, User *> members;
//     // std::map<USER_ID, User *> operators;

//     if (channelName.find(',') != std::string::npos)
//         channel = splitInput(channelName);
//     else
//         channel[channelName] = "";

//     for (it = channel.begin(); it != channel.end(); it++)
//     {
//         if (Channel *existingChannel = db->getChannel(it->first))
//         {
//             if (listChan.find(it->first) == listChan.end())
//             {
//                 listChan[it->first] = existingChannel;
//             }
// 		// ft_send(var->fd, ":" + var->cit[var->fd].getNick() + "!~" + var->cit[var->fd].getUser() + "@" + var->cit[var->fd].getHost() + " JOIN " + Ch_Name);
//             if (existingChannel->getMember(fd))
//             {
//                 //later just ignore it there is no such error in join
//                 sendResponse(fd, "User already in channel\n");
//             }
//             else
//             {
//                 existingChannel->addMember(user);
//                 sendResponse(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + getHostName() + " JOIN " + it->first + "\n");
//                 sendResponse(fd, "db->getUser(fd)->getNickName() Joined successfully\n");
//             }
//         }
//         else
//         {
//             listChan[it->first] = db->addNewChannel(it->first, user);
//             sendResponse(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + getHostName() + " JOIN " + it->first + "\n");
//             sendResponse(fd, ": db->getUser(fd)->getNickName() Created The Channel successfully\n");
//         }
//             // members = db->getChannel(it->first)->getMembers();
//             // operators = db->getChannel(it->first)->getOperators();
//             // std::cout << "members size = " << members.size() << std::endl;
//             // std::cout << "operator size = " << operators.size() << std::endl;
//             // std::cout << "db->getUser(fd)->getNickName() = " << db->getUser(fd)->getNickName() << std::endl;
//     }
// }
