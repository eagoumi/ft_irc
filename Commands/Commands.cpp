#include "Commands.hpp"


Commands::Commands()
{
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
    //complete this
    return *this;
}

void Commands::CommandMapinit(cmdData dataCmd)
{
    Channel cObj;

    // std::cout << line << std::endl;
    db = Database::GetInstance();
    
    // _db->displayChannels();
    std::string token;
    std::istringstream iss(dataCmd.line);
    client = dataCmd.nick;
    fd = dataCmd.fd;
    while(iss >> token)
    {
        command.push_back(token);
    }

    for (itV = command.begin(); itV != command.end(); itV++)
        std::cout << *itV << " ";
    std::cout << std::endl;


    // channels.insert(std::make_pair(fd, getChannel()));
    if(getCommand() == "JOIN")
        join();
    else if(getCommand() == "KICK")
        kick();
    // else if(getCommand() == "INVITE")
    //     invite();
    // else if(getCommand() == "MODE")
    //     mode();
    // else if(getCommand() == "TOPIC")
    //     topic();
    else
        // std::cout << ":" << get_nickName() << " " << getCommand() << " :Unknown command" << std::endl;
        sendResponse(":" + getClient() + " " + getCommand() + " :Unknown command\n");
}


// void Commands::parsCommands(std::string str, std::string owner, std::string host, int fd){
//     (void)host;
//     this->fd = fd;
//     this->owner = owner;

//     // std::cout << "host " << host << std::endl;
//     // std::cout << "fd " << fd << std::endl;
//     // this->command = ft_trim(str);
//     fillMultimap();
//     this->command = ft_split(str);
//     if(get_command() == "kick" || get_command() == "KICK")
//         kick();
//     else if(get_command() == "invite" || get_command() == "INVITE")
//         invite();
//     else if(get_command() == "topic" || get_command() == "TOPIC")
//         topic();
//     else if(get_command() == "mode" || get_command() == "MODE")
//         mode();
//     else
//         std::cout << ":" << get_nickName() << " " << get_command() << " :Unknown command" << std::endl;
//     for(this->itCh = channels.begin(); this->itCh != channels.end(); this->itCh++)
//         std::cout << itCh->first << " " << itCh->second << std::endl;

// }

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


std::string Commands::getNick() {
     for(itV = command.begin() ; itV != command.end(); itV++)
    {
        if(itV->find('#', 0) != std::string::npos)
        {
            itV++;
            return *itV;
        }
    }
    return "";
}
std::string Commands::getClient() const{
    return client;
}
std::string Commands::getCommand() const{
    if(command[1] == "KICK")
        return command[1];
    return command[0];

}
std::string Commands::getChannel() {
    for(itV = command.begin() ; itV != command.end(); itV++)
    {
        if(itV->find('#', 0) != std::string::npos)
        {
            return *itV;
        }
    }
    return "";
}

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
