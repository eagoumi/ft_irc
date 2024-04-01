#include "Commands.hpp"
#include <map>
#include "../Users/user.hpp"



void SendMessageToMembers(Channel *Channel_name, User *user_fds, std::string command)
{
    std::map<USER_ID, User *> checkUsers = Channel_name->getMembers();
    std::map<USER_ID, User *>::iterator iter_map = checkUsers.begin();
    std::string mess = ": " + user_fds->getNickName() + "!" + user_fds->getUserName() + "@" + "127.0.0.1" + " " + command + " " + "\r\n";

    for(; iter_map != checkUsers.end(); iter_map++)
    {
        if(send(iter_map->first, mess.c_str(), mess.length(), 0) < 0)
            throw std::runtime_error("Error On Sending a Message to the Client.\n");
    }
}

void Commands::part()
{
    std::vector<std::string> All_channels = getNextParam().second;

    for (size_t i = 0; i < All_channels.size() ; i++)
    {
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
        Store_channel->deleteMember(db->getUser(fd)->getNickName());
    }
    // std::cout << "2 " << All_channels[0] << std::endl;
}


// void send_message_all_in_channel(channel, user, cmd)
// {
//     int i = 0;
//     map<USER_ID, USER *>users = getMembers() or map();
//     std::string msg = ":"( + ServertoClientst() + cmd + "\r\n";
//     while (i < users.size())
//     {
//         send(users.socketfdclients[i], msg, msg.len, 0);
//         i++;
//     }
// }

// void Commands::part()
// {
//     if (args.size < 2)
//     {
//         send Not enough parameter; using ERR_NEEDMOREPARAMS
//         return;
//     }

//     channel x << getchanel() from database and take name channel as parameter

//     if (checking if the channel are exist other way are not NULL x == NULL)
//     {
//         send ERR no such Channel; using ERR_NOSUCHCHANNEL
//         return;
//     }
//     if (checking if tthe user are axite on the channel)
//     {
//         send this user are not in the channel using ERR_NOTONCHANNEL;
//         return;
//     }
//     first send the message to all channel that the member are lefting the channel
//     using send_message_all_in_channel(x, user, "PART " + channelname);
//     than final remove this user on this channel; using deleteMember();

// }