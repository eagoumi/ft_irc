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
    // if (!_currUser)
    //     return ;
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

void Logger::IRCPrint(size_t fd, std::string string) {

    std::string buffer = string + "\r\n";
    //catch erro
    send(fd, buffer.c_str(), buffer.length(), 0);
}

void Logger::ServertoClient(std::string string) {

    IRCPrint(_userFd, ":" + _serverIp + " " + string);
}

void Logger::SendJoinedMembers(Channel *Channel_name, std::string command)
{
    //getNickname() + "!" + getUsername() + "@" + getHostIp() + " "
    std::map<USER_ID, User *> checkUsers = Channel_name->getMembers();
    std::map<USER_ID, User *>::iterator iter_map = checkUsers.begin();
    std::string msg = ":" + GetUserName() + "!" + GetUserName() + "@" + getServerIP() + " " + command + "\r\n";
    for(; iter_map != checkUsers.end(); iter_map++)
    {
        if (iter_map->first != _currUser->getUserId())
            send(_userFd, msg.c_str(), msg.length(), 0);
    }
    // Database::GetInstance()->getChannel()
}

//Send Client To Client
void Logger::CleintToClient(int id, std::string msg) {

    std::string mess = ": " + _nickname + "!~" + _username + "@" + _serverIp + " " + msg;

    IRCPrint(id, mess);
}
