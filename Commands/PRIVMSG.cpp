#include "Commands.hpp"

//Check with LimeChat
void Commands::PRIVMSG()
{
    std::vector<std::string> get_param = getNextParam().second;
    std::string Message = getNextParam().first;
    for(size_t i = 0; i < get_param.size(); i++)
    {
        Channel *Current_ch = db->getChannel(get_param[i]);
        if (!Current_ch)
        {
            if (get_param[i][0] == '#' && Current_ch->getMember(fd))
                SendMessageToMembers(Current_ch, currUser, Message); // check for message syntax
            else
                currUser->ServertoClients(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), get_param[i]));
        }
        else
        {
            User *reciver_msg = db->existUser(get_param[i]);
            if (reciver_msg)
                currUser->CleintToClient(reciver_msg->getUserId(), Message);
            else
                currUser->ServertoClients(ERR_NOSUCHNICK(get_param[i]));
        }
    }
}