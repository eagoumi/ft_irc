#include "Commands.hpp"

void Commands::join(){
    User *user = new User(fd);
    Channel *Chan;
    std::map<std::string, std::string> channel;
    std::map<std::string, std::string>::iterator it;
    std::vector<Channel*> listChan;
    int i = 0;
    if (getChannel().find(',') != std::string::npos)
    {
        channel = splitInput(getChannel());
        for(it = channel.begin(); it != channel.end(); it++)
        {
            listChan[i] = db->addNewChannel(it->first, user);
            sendResponse(": Clinet joind seccefully\n");
            i++;
        }
    }
    else
    {
        db->addNewChannel(getChannel(), user);
        sendResponse(": Clinet joind seccefully\n");
    }
    
    // std::cout << "FD = "  << fd << std::endl;
    // std::string key = command[1];
    // getChannel();
}