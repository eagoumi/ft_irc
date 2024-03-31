#include "Commands.hpp"
#include <vector>

# define NOT_FOUND NULL

std::string Commands::invitedNick;

void Commands::join()
{

std::cout << "join() : beg" << std::endl;
    std::vector<std::string> channelNamesList = getNextParam().second;
std::cout << "join() : line 2" << std::endl;
    std::vector<std::string> channelkeysList = getNextParam().second;
    for (size_t channelIndex = 0; channelIndex < channelNamesList.size(); channelIndex++)
    {
std::cout << "join() : loop " << channelNamesList[channelIndex] << std::endl;
std::cout << "join() : user nickname: " + currUser->getNickName() << std::endl;

        currChannel = db->getChannel(channelNamesList[channelIndex]);
        if (currChannel == NOT_FOUND) {

            std::cout << "channel not found, creating by " << currUser->getUserName() << " ...\n";
            db->addNewChannel(channelNamesList[channelIndex], currUser);
            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + getHostName() + " JOIN " + channelNamesList[channelIndex] + "\n");
            sendResponse(fd, ":"+  db->getUser(fd)->getNickName() + " Created The Channel successfully\n");
        }
        else {
            std::cout << "getMode = " << getMode("i") << " getInvited = " << currChannel->getInvitedNick(currUser->getNickName()) << std::endl;
            if (currChannel->getMember(fd) != NULL)
            {
                sendResponse(fd, "User already in channel\n");
                continue ;
            }
            else if(this->getMode("i") == true && currChannel->getInvitedNick(currUser->getNickName()) == false )            
            {
                sendResponse(fd, ":"+  db->getUser(fd)->getNickName() + getChannel() + " :Cannot join channel (+i)\n");
            }
            // else if(getMode("l") == true && db->get)
            else
            {
                // std::cout << "TEST = " << currChannel->getChannelName() << std::endl;
                currChannel->addMember(currUser);
                sendResponse(fd, ":" + db->getUser(fd)->getNickName() + "!~" + db->getUser(fd)->getUserName() + "@" + getHostName() + " JOIN " + channelNamesList[channelIndex] + "\n");
                sendResponse(fd, db->getUser(fd)->getNickName() + " Joined successfully\n");
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

//     if (getChannel().find(',') != std::string::npos)
//         channel = splitInput(getChannel());
//     else
//         channel[getChannel()] = "";

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
