#include "Commands.hpp"

void Commands::kick()
{
    int flag  = -1;
    if (command.size() < 3)
    {
        std::cout << ":" << get_owner() << " " << getCommand() << " " << " :Not enough parameters" << std::endl;
        return ;
    }

    range = channels.equal_range(getChannel());
    if(range.first == range.second)
    {
        std::cout << ":" << get_owner() << " " << getChannel() << " :No such channel" << std::endl;
        return ;
    }
    for (itCh = range.first; itCh != range.second; itCh++)
    {
        if (itCh->second == get_owner() && itCh->second[0] == '@')
        {
            flag = 1;
            break;
        }
        else if(itCh->second == get_owner() && itCh->second[0] != '@')
            flag = 0;
        else
            flag  = 3;
    }
    if (flag == 0)
    {
        std::cout << ":" << get_owner() << " " << getChannel() << " :You're not channel operator" << std::endl;
        return;
    }
    else if(flag  == 3)
        std::cout << ":" << get_owner() << " " << getChannel() << " :You're not on that channel" << std::endl;


    for (itCh = range.first; itCh != range.second; itCh++)
    {
        if (itCh->second == get_nickName() && flag == 1)
        {
            channels.erase(itCh);
            // write(get_fd(), getComment().c_str(), std::strlen(getComment().c_str()));
            write(get_fd(), "\n", 1);
            flag  = -1;
            return;
        }
        else
        {
            flag = 1;
        }
    }
    if (flag == 1)
    {
        std::cout << ":" << get_owner() << " " << get_nickName() << " " << getChannel() << " :They aren't on that channel" << std::endl;
        return;
    }
}
