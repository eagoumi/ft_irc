#include "Commands.hpp"



void Commands::SendMessageToMembers(Channel *Channel_name, User *user_fds, std::string command)
{
    std::map<USER_ID, User *>checkUsers = Channel_name->getMembers();
    std::string mess = ":" + user_fds->getNickName() + "!" + user_fds->getUserName() + "@" + user_fds->getServerIP() + " " + command + "\r\n";

    for(size_t i = 0; i < checkUsers.size(); i++)
        if(send(user_fds->getUserId(), mess.c_str(), mess.length(), 0) < 0)
            throw std::runtime_error("Error On Sending a Message to the Client.\n");
}

void Commands::part()
{
    // if (command.size() < 2)
    // {
    //     currUser->ServertoClients(ERR_NEEDMOREPARAMS(currUser->getNickName(), "PART"));
    //     return ;
    // }

    // if (db->getChannel(this->getChannel()) == NULL)
    // {
    //     currUser->ServertoClients(ERR_NOSUCHCHANNEL(currUser->getNickName(), "PART"));
    //     return ;
    // }
    // else if (existMemberChannel(db->getUser(fd)->getNickName()) == false)
    // {
    //     currUser->ServertoClients(ERR_NOTONCHANNEL(currUser->getNickName(), "PART"));
    //     return ;
    // }
    // SendMessageToMembers(db->getChannel(this->getChannel()), currUser, "PART" + db->getUser(fd)->getNickName());
    // db->getChannel(this->getChannel())->deleteMember(db->getUser(fd)->getNickName());
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