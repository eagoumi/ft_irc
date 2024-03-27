#include "Commands.hpp"

void Commands::mode()
{
    if (command.size() < 2)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
        return;
    }
    std::cout << "command = " <<  getCommand() << std::endl;
}