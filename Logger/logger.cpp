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
    if (!_currUser)
        return ;
    _currUser = currUser;
    _userFd = _currUser->getUserId();
    _nickname = _currUser->getNickName();
    _username = _currUser->getUserName();
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

//Send Client To Client
void Logger::CleintToClient(int id, std::string msg) {

    std::string mess = ": " + _nickname + "!~" + _username + "@" + _serverIp + " " + msg;

    IRCPrint(id, mess);
}
