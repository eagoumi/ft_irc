#include "Commands.hpp"

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
                    currUser->CleintToClient( IT_OPER->first, Message); // check for message syntax
                }
                puts("ss3");
            }
            else
                currUser->ServertoClients(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), good_str));
        }
        else
        {
            Channel *ch = db->getChannel(get_param[i]);
            if (ch)
            {
                if (get_param[i][0] == '#' && ch->getMember(fd))
                    SendMessageToMembers(ch, currUser, Message); // check for message syntax
                else
                    currUser->ServertoClients(ERR_NOTONCHANNEL(db->getUser(fd)->getNickName(), get_param[i]));
            }
            else
            {
                User *reciver_msg = db->existUser(get_param[i]);
                if (reciver_msg)
                    currUser->CleintToClient(reciver_msg->getUserId(), Message);
                else
                {
                    puts("i am here");
                    currUser->ServertoClients(ERR_NOSUCHNICK(get_param[i]));
                }
            }
        }
    }
}