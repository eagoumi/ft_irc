#include "Commands.hpp"

bool Commands::check_connection(size_t user_fd)
{
    std::map<size_t, User *> user = db->getUsers();
    std::map<size_t, User *>::iterator it_user = user.begin();
    for (; it_user != user.end() ; it_user++)
    {
        if (it_user->first == user_fd)
            return true;
    }
    return false;
}

void Commands::sendToClientsExisted(size_t reciver, User *sender, std::string Message)
{
    (void)sender;
    std::string msg = ":" + _logger.PrefixLogs() + Message + "\r\n";
    if (check_connection(reciver)) //check if the clients still connected then shend the message
        send(reciver, msg.c_str(), msg.length(), 0);
    msg.clear();
}

std::string eraseOpertorSymbole(std::string Channel_Name)
{
    std::string tmp;
    if ((Channel_Name[0] == '@' && Channel_Name[1] == '%') || (Channel_Name[0] == '%' && Channel_Name[1] == '@'))
        Channel_Name.erase(0, 2);
    else if (Channel_Name[0] == '@' || Channel_Name[0] == '%')
        Channel_Name.erase(0, 1);
    return Channel_Name;
}

void Commands::PRIVMSG()
{
    std::vector<std::string> get_param = getNextParam().second;
    std::string Message = getNextParam().first;
    // std::cout << Message << std::endl;
    for(size_t i = 0; i < get_param.size(); i++)
    {
        if ((get_param[i][0] == '@' || get_param[i][0] == '%'))
        {
            std::string good_str = eraseOpertorSymbole(get_param[i]);
            // std::cout <<  "Good are: " << good_str << std::endl;
            Channel *ch1 = db->getChannel(good_str);
            if(ch1 && good_str[0] == '#' && ch1->getMember(fd))
            {
                std::map<USER_ID, User *> operators = ch1->getOperators();
                std::map<USER_ID, User *>::iterator IT_OPER = operators.begin();
                for (; IT_OPER != operators.end() ; IT_OPER++)
                {
                    std::cout << "Operators = " << IT_OPER->second->getNickName() << std::endl;
                    _logger.SendJoinedMembers(ch1, "PRIVMSG " + IT_OPER->second->getNickName() + " :" + Message);
                }
            }
            else
            {
                _logger.ServertoClient(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), good_str));
                return ;
            }
        }
        else
        {
            Channel *ch = db->getChannel(get_param[i]);
            if (get_param[i][0] == '#')
            {
                if (ch && ch->getMember(fd))
                    _logger.SendJoinedMembers(ch, "PRIVMSG " + ch->getChannelName() + " :" + Message);
                else
                {
                    _logger.ServertoClient(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), get_param[i]));
                    return ;
                }
            }
            else
            {
                User *reciver_msg = db->existUser(get_param[i]);
                // std::cout << "User Are: " << reciver_msg->getNickName() << std::endl;
                if (reciver_msg)
                    sendToClientsExisted(reciver_msg->getUserId(), currUser, "PRIVMSG " + reciver_msg->getNickName() + " :" + Message);
                else
                {
                    _logger.ServertoClient(ERR_NOSUCHNICK(db->getUser(fd)->getNickName(),get_param[i]));
                    return ;
                }
            }
        }
    }
}