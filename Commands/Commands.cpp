#include "Commands.hpp"

Commands::Commands()
{
    db = Database::GetInstance();
}

Commands::~Commands()
{
}

Commands::Commands(const Commands &obj)
{
    *this = obj;
}

Commands &Commands::operator=(const Commands &obj)
{
    (void)obj;
    // complete this
    return *this;
}

void Commands::CommandMapinit(cmdData dataCmd)
{
    // Channel cObj;

    // std::cout << line << std::endl;

    std::string token;
    std::istringstream iss(dataCmd.line);
    client = dataCmd.nick;
    fd = dataCmd.fd;

    while (iss >> token)
    {
        command.push_back(token);
    }

    for (itV = command.begin(); itV != command.end(); itV++)
        std::cout << *itV << " ";
    std::cout << std::endl;

    // std::cout << "FD = " << fd << std::endl;
    // std::cout << db << std::endl;
    parsCommands();
    // channels.insert(std::make_pair(fd, getChannel()));
    if (getCommand() == "JOIN")
        join();
    else if (getCommand() == "KICK")
        kick();
    // else if(getCommand() == "INVITE")
    //     invite();
    // else if(getCommand() == "MODE")
    //     mode();
    // else if(getCommand() == "TOPIC")
    //     topic();
    else
        sendResponse(":" + getClient() + " " + getCommand() + " :Unknown command\n");
    // std::cout << db << std::endl;
}

// void Commands::fillMultimap(){
//     // channels.insert(std::make_pair("friend", "@yousra"));
//     // channels.insert(std::make_pair("friend", "meryem"));
//     // channels.insert(std::make_pair("friend", "asmaa"));
//     // channels.insert(std::make_pair("friend", "hiba"));

//     channels.insert(std::make_pair("#family", "@amine"));
//     channels.insert(std::make_pair("#family", "sara"));
//     channels.insert(std::make_pair("#family", "anas"));
//     channels.insert(std::make_pair("#family", "yousra"));

//     // channels.insert(std::make_pair("parents", "@latifa"));
//     // channels.insert(std::make_pair("parents", "mehdi"));
//     // channels.insert(std::make_pair("parents", "yousra"));

// }

void Commands::sendResponse(std::string message)
{
    send(fd, message.c_str(), message.length(), 0);
}

std::string Commands::getNick()
{
    for (itV = command.begin(); itV != command.end(); itV++)
    {
        if (itV->find('#', 0) != std::string::npos)
        {
            itV++;
            return *itV;
        }
    }
    return "";
}
std::string Commands::getClient() const
{
    return client;
}

std::string Commands::getCommand() const
{
    if (command[1] == "KICK")
        return command[1];
    return command[0];
}

std::map<std::string, std::string> Commands::parsJoin()
{
    std::map<std::string, std::string> sChannels;
    std::map<std::string, std::string>::iterator it;
    
    std::istringstream str(getChannel());
    std::string token;
    while (getline(str, token, ','))
    {
        sChannels.insert(std::make_pair(token, ""));
    }
    if(command[2] != "")
    {   
        it = sChannels.begin();
        std::istringstream str2(command[2]);
        while(getline(str2, token, ','))
        {
            it->second = token;
            it++;
        }
    }
    for(it = sChannels.begin(); it != sChannels.end(); it++)
    {
        std::cout << it->first << "      " << it->second << std::endl;
    }
    return sChannels;
}

void Commands::parsCommands()
{
    if (getCommand() == "JOIN")
        parsJoin();
    // else if(getCommand() == "KICK")
    //     parsKick();
    else
        std::cout << "faiiiiler" << std::endl;
}

std::string Commands::getChannel()
{
    for (itV = command.begin(); itV != command.end(); itV++)
    {
        if (itV->find('#', 0) != std::string::npos)
        {
            return *itV;
        }
    }
    return "";
}

// std::string Commands::getChannel() {
//     // return command[0];
//     std::string token;
//     std::string keey;
//     for(itV = command.begin() ; itV != command.end(); itV++)
//     {
//         if(itV->find('#', 0) != std::string::npos)
//         {
//             std::istringstream str(*itV);
//             while(getline(str, token, ','))
//             {
//                 std::cout << "TOKEN = " << token << std::endl;
//             }
//             break;
//             // return *itV;
//         }
//     }
//     itV++;
//     std::istringstream key(*itV);
//     while(getline(key, keey, ','))
//     {
//         std::cout << "TOKEN = " << keey << std::endl;
//     }
//     return "";
// }

std::string Commands::getHostName()
{
    char hostName[256];
    if (gethostname(hostName, sizeof(hostName)) != 0)
        return "";
    return hostName;
}

// std::string Commands::getKey(){

// }

// std::string Commands::get_comment() const{
//     if(command[3] != "\0")
//         return command[3];
//     else
//         return " ";
// }
// int Commands::get_fd() const{
//     return fd;
// }
// std::string Commands::get_owner() const{
//     return this->owner;
// }