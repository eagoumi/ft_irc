#include "Commands.hpp"

void Commands::mode()
{
    std::string targetChannel;
    std::string targetNick;

    if (command.size() < 2)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
        return;
    }
    if(command[1][0] == '#')
        targetChannel = command[1];
    else
        targetNick = command[1];
    
    
    std::cout << "command = " << targetChannel << std::endl;
    std::cout << "command = " << targetNick << std::endl;
}