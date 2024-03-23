#include "commands.hpp"

void Commands::invite()
{
    bool owner = false;
    bool onChannel = false;
    if (command.size() < 3)
    {
        std::cout << ":" << getClient() << " " << getCommand() << " :Not enough parameters" << std::endl;
        return;
    }
    range = channels.equal_range(command[2]);
    if(range.first == range.second)
    {
        std::cout << ":" << getClient() << " " << getChannel() << " :No such channel" << std::endl;
        return ;
    }
    for (itCh = range.first; itCh != range.second; itCh++)
    {
        //don't forget to add the set_only mode for joining the channel
        if (getClient() == itCh->second)
        {
            owner = true;
           
        }
        if(itCh->second == command[1])
        {
            onChannel = true;
            
        }
    }
    if(owner == true && onChannel == false)
    {
        channels.insert(std::pair<std::string, std::string>(command[2], command[1]));
        return;
    }
    else if(owner == false)
    {
        std::cout << ":" << getClient() << " " << getChannel() << " :You're not on that channel" << std::endl;
        return;
    } 
    else if(onChannel == true)
    {
        std::cout << ":" << getClient() << " " << command[1] << " " << command[2] << " :is already on channel" << std::endl;
        return ;
    }

}