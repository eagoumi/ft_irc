#include "Commands.hpp"

void Commands:: kick()
{
    // User *Uobj;
    // db = Database::GetInstance();
    // int flag  = -1;
// std::cout << db << std::endl;

    if (command.size() < 3)
    {
        sendResponse(":" + getClient() + " " + getCommand() +  " :Not enough parameters");
        return ;
    }
    if(db->getUser(fd) != NULL)
    {
        std::cout << "USER'S NICKNAME " << db->getUser(fd)->getUserName() << std::endl;
    }

    if(db->getChannel(this->getChannel()) == NULL)
    {
        sendResponse(":" + getClient() + " " + getChannel() + " :No such channel\n");
    }

    // if(db->ge)

    // for (itCh = range.first; itCh != range.second; itCh++)
    // {
    //     if (itCh->second == getClient() && itCh->second[0] == '@')
    //     {
    //         flag = 1;
    //         break;
    //     }
    //     // else if(itCh->second == getClient() && itCh->second[0] != '@')
    //         flag = 0;
    //     // else
    //     //     flag  = 3;
    // }
    // if (flag == 0)
    // {
    //     // std::cout << ":" << getClient() << " " << getChannel() << " :You're not channel operator" << std::endl;
    //     return;
    // }
    // else if(flag  == 3)
    //     // std::cout << ":" << getClient() << " " << getChannel() << " :You're not on that channel" << std::endl;


    // for (itCh = range.first; itCh != range.second; itCh++)
    // {
    //     if (itCh->second == get_nickName() && flag == 1)
    //     {
    //         channels.erase(itCh);
    //         // write(get_fd(), getComment().c_str(), std::strlen(getComment().c_str()));
    //         // write(get_fd(), "\n", 1);
    //         flag  = -1;
    //         return;
    //     }
    //     else
    //     {
    //         flag = 1;
    //     }
    // }
    // if (flag == 1)
    // {
    //     // std::cout << ":" << getClient() << " " << get_nickName() << " " << getChannel() << " :They aren't on that channel" << std::endl;
    //     return;
    // }
}
