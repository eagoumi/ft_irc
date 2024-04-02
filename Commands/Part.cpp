#include "Commands.hpp"
#include <map>
#include "../Users/user.hpp"



void Commands::SendMessageToMembers(Channel *Channel_name, User *user_fds, std::string command)
{
    std::map<USER_ID, User *> checkUsers = Channel_name->getMembers();
    std::map<USER_ID, User *>::iterator iter_map = checkUsers.begin();
    for(; iter_map != checkUsers.end(); iter_map++)
    {
        user_fds->IRCPrint(iter_map->first, command);
    }
}

void Commands::part()
{
    std::vector<std::string> All_channels = getNextParam().second;

    for (size_t i = 0; i < All_channels.size() ; i++)
    {
        User *userParted = db->existUser(db->getUser(fd)->getNickName());
        Channel *Store_channel = db->getChannel(All_channels[i]);
        // std::cout << All_channels[i] << std::endl;
        if (Store_channel == NULL)
        {
            currUser->ServertoClients(ERR_NOSUCHCHANNEL(db->getUser(fd)->getNickName(), "PART"));
            return ;
        }
        else if (Store_channel->getMember(fd) == NULL)
        {
            currUser->ServertoClients(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), "PART"));
            return ;
        }
        SendMessageToMembers(Store_channel, currUser, "PART " + db->getUser(fd)->getNickName() + " " + All_channels[i]);
        Store_channel->deleteMember(userParted);
        if(Store_channel->getMembers().size() == 0)
        {
            db->deleteChannel(All_channels[i]);
        }
    }
    // std::cout << "2 " << All_channels[0] << std::endl;
}

