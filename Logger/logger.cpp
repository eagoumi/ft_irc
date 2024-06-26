#include "logger.hpp"

Logger::Logger() {
}

Logger::~Logger() {
}

Logger& Logger::GetInstance() {
    static Logger logger_;
    return logger_;
}

std::string Logger::getServerIP() {
    return _serverIp;
}

void Logger::setCurrUser(User* currUser) {
    _currUser = currUser;
    _userFd = _currUser->getUserId();
    _nickname = _currUser->getNickName();
    _username = _currUser->getUserName();
}

std::string Logger::GetUserName()
{
    return _username;
}

std::string Logger::GetNickName()
{
    return _nickname;
}

void Logger::setServerIp(std::string const& serverIp) {
    
    _serverIp = serverIp;
}

std::string Logger::PrefixLogs()
{
    return _nickname + "!" + _username + "@" + getServerIP() + " ";
}

void Logger::IRCPrint(std::string string) 
{
    std::string buffer = string + "\r\n";
    //catch erro
    if (send(_userFd, buffer.c_str(), buffer.length(), 0) < 0)
        throw std::runtime_error("Error On Sending a Message to the Client.\n");
}

void Logger::ServertoClient(std::string string) 
{
    IRCPrint(":" + _serverIp + " " + string);
}

//Send Client To Client
void Logger::CleintToClient(std::string msg) 
{
    IRCPrint(":" + PrefixLogs() + msg);
}

void Logger::SendJoinedMembers(Channel *Channel_name, std::string command)
{
    //getNickname() + "!" + getUsername() + "@" + getHostIp() + " "
    std::map<USER_ID, User *> checkUsers = Channel_name->getMembers();
    std::map<USER_ID, User *>::iterator iter_map = checkUsers.begin();
    std::string msg = ":" + PrefixLogs() + command + "\r\n";
    for(; iter_map != checkUsers.end(); iter_map++)
    {
        if (iter_map->first != _currUser->getUserId())
            send(iter_map->first, msg.c_str(), msg.length(), 0);
    }
    // Database::GetInstance()->getChannel()
}

