# include "./user.hpp"
#include <sys/socket.h>

bool User::isStrContains(std::string const& str, std::string const& charSet) {

    for (int i = 0; charSet[i]; i++)
        if (str.find(charSet[i]) != std::string::npos) return true;
    return false;
}

bool User::isStrStartWith(std::string const& str, std::string const& charSet) {

    for (int i = 0; charSet[i]; i++)
        if (str.at(0) == charSet[i]) return true;
    return false;
}

User::User(){}

User::User(USER_ID const& Id) : _Id(Id) {
    // _isAuthenticated = false;
    _isPasswordInserted = false;
    _isUserNameInserted = false;
    _isNickInserted = false;
}

USER_ID const& User::getUserId() {
    return (this->_Id);
}

void User::setUserName(USER_NAME const& name) {
    _username = name;
    _isUserNameInserted = true;
}

USER_NAME const & User::getUserName() {
    return (_username);
}

void User::setNickName(NICK_NAME const& name) {
    //welp I think I'll have to check if name is empty, I need to test it
    isStrStartWith(name, "$:#&+~%") == true ? throw  std::string("uWu nickname is wrong") : NULL;
    isStrContains(name, " ,*?!@.") == true ? throw std::string("uWu nickname is wrong") : NULL;
    _nickname = name;
    _isNickInserted = true;
}

NICK_NAME const & User::getNickName() {
    return (_nickname);
}


//by agoumi
// void User::SetAuthenticated()
// {
//     if (_UserCheck == true && _NickCheck == true && _Correct_Password == true)
//         _isAuthenticated = true;
// }

bool User::isAuthenticated()
{
    return (_isPasswordInserted == true && _isNickInserted == true && _isUserNameInserted == true);
}

//checking if the pass word are entred and correct
void User::insertedPassSuccessfully()
{
    _isPasswordInserted = true;
}

bool User::hasInsertedPass()
{
    return (_isPasswordInserted);
}

//checking fo entring NickName
// void User::NickCheck()
// {
//     _NickCheck = true;
// }

bool User::hasInsertedNick()
{
    return (_isNickInserted);
}

//checking fo entring NickName
// void User::UserCheck()
// {
//     _UserCheck = true;
// }

bool User::hasInsertedUsername()
{
    return (_isUserNameInserted);
}


void User::setServerIP(std::string const &ServerIP)
{
    std::cout << _IPServer << std::endl;
    _IPServer = ServerIP;
}

std::string const &User::getServerIP()
{
    std::cout << _IPServer << std::endl;
    return (_IPServer);
}


void User::IRCPrint(std::string string)
{
    std::string buffer = string + "\n";
    //catch erro
    if (send(_Id, buffer.c_str(), buffer.length(), 0) < 0)
        throw std::runtime_error("Error On Sending a Message to the Client.\n");
}

void User::ServertoClients(std::string string)
{
    std::cout << _IPServer << std::endl;
    std::string ipserv = getServerIP();
    IRCPrint(":" + ipserv + " " + string);
}

User::~User() {
    //here I think I'll have to ... nothing srsly
}
