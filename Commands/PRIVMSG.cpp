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
    std::string msg = ":" + sender->getNickName() + "!" + sender->getUserName() + "@" + _logger.getServerIP() + " " + Message + "\r\n";
    if (check_connection(reciver)) //check if the clients still connected then shend the message
        send(reciver, msg.c_str(), msg.length(), 0);
    msg.clear();
}

std::string eraseOpertorSymbole(std::string Channel_Name)
{
    std::string tmp;
    if (Channel_Name[0] == '@' && Channel_Name[1] == '%')
        Channel_Name.erase(0, 2);
    else if (Channel_Name[0] == '@' || Channel_Name[0] == '%')
    {
        Channel_Name.erase(0, 1);
    }
    return Channel_Name;
}

//Check with LimeChat
void Commands::PRIVMSG()
{
    std::vector<std::string> get_param = getNextParam().second;
    std::string Message = getNextParam().first;
    std::cout << Message << std::endl;
    for(size_t i = 0; i < get_param.size(); i++)
    {
        if ((get_param[i][0] == '@' || get_param[i][0] == '%'))
        {
            puts("ss");
            std::string good_str = eraseOpertorSymbole(get_param[i]);
            std::cout << good_str << std::endl;
            Channel *ch1 = db->getChannel(good_str);
            if(ch1 && good_str[0] == '#' && ch1->getMember(fd))
            {
            puts("ss");
                std::map<USER_ID, User *> operators = ch1->getOperators();
                std::map<USER_ID, User *>::iterator IT_OPER = operators.begin();
                for (; IT_OPER != operators.end() ; IT_OPER++)
                {
                    puts("ss11");
                    std::cout << "Operators = " << IT_OPER->second->getNickName() << std::endl;
                    _logger.CleintToClient(IT_OPER->first, Message); // check for message syntax
                }
                puts("ss3");
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
            if (ch && get_param[i][0] == '#')
            {
                if (ch->getMember(fd))
                    _logger.SendJoinedMembers(ch, "PRIVMSG " + ch->getChannelName() + " :" + Message); // check for message syntax
                else
                {
                    _logger.ServertoClient(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), get_param[i]));
                    return ;
                }
            }
            else
            {
                User *reciver_msg = db->existUser(get_param[i]);
                if (reciver_msg)
                {
                    // if (Message[0] != ':')
                    //     Message.insert(0, ":");
                    // std::cout << " ID = " << reciver_msg->getUserId() << std::endl;
                    // std::cout << " NICK = " << get_param[i] << std::endl;
                    sendToClientsExisted(reciver_msg->getUserId(), reciver_msg, "PRIVMSG " + reciver_msg->getNickName() + " :" + Message);
                }
                else
                {
                    // puts("i am here");
                    // std::cout << get_param[i] << std::endl;
                    _logger.ServertoClient(ERR_NOSUCHNICK(db->getUser(fd)->getNickName(),get_param[i]));
                    return ;
                }
            }
        }
    }
}