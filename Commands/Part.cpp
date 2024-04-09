#include "Commands.hpp"
#include <map>
#include "../Users/user.hpp"



void Commands::SendMessageToMembers(Channel *Channel_name, User *user_fds, std::string command)
{
    (void)user_fds;
    std::map<USER_ID, User *> checkUsers = Channel_name->getMembers();
    std::map<USER_ID, User *>::iterator iter_map = checkUsers.begin();
    std::string message = ":" + _logger.PrefixLogs() + command + "\r\n";
    for(; iter_map != checkUsers.end(); iter_map++)
        send(iter_map->first, message.c_str(), message.length(), 0);
}

void Commands::part()
{
    std::vector<std::string> All_channels = getNextParam().second;

    for (size_t i = 0; i < All_channels.size() ; i++)
    {
        User *userParted = db->existUser(db->getUser(fd)->getNickName());
        currChannel = db->getChannel(All_channels[i]);
        if (currChannel == NULL)
        {
            _logger.ServertoClient(ERR_NOSUCHCHANNEL(db->getUser(fd)->getNickName(), "PART"));
            return ;
        }
        else if (currChannel->getMember(fd) == NULL)
        {
            _logger.ServertoClient(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), "PART"));
            return ;
        }
        SendMessageToMembers(currChannel, currUser, "PART " + All_channels[i]);
        currChannel->deleteMember(userParted);
        if(currChannel->isUserOperator(userParted->getUserId()) == true)
            currChannel->deleteOperator(userParted);
        if(currChannel->getMembers().size() == 0)
        {
            db->deleteChannel(All_channels[i]);
        }
    }
}

