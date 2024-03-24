# include "./user.hpp"

static bool isStrContains(std::string const& str, std::string const& charSet) {

    for (int i = 0; charSet[i]; i++)
        if (str.find(charSet[i]) != std::string::npos) return true;
    return false;
}

static bool isStrStartWith(std::string const& str, std::string const& charSet) {

    for (int i = 0; charSet[i]; i++)
        if (str.at(0) == charSet[i]) return true;
    return false;
}

User::User(USER_ID const& Id) : _Id(Id) {
}

USER_ID const& User::getUserId() {
    return (this->_Id);
}

void User::setUserName(USER_NAME const& name) {
    _username = name;
}

USER_NAME const & User::getUserName() {
    return (_username);
}

void User::setNickName(NICK_NAME const& name) {
    //welp I think I'll have to check if name is empty, I need to test it
    isStrStartWith(name, "$:#&+~%") == true ? throw  std::string("uWu nickname is wrong") : NULL;
    isStrContains(name, " ,*?!@.") == true ? throw std::string("uWu nickname is wrong") : NULL;
    _nickname = name;
}

NICK_NAME const & User::getNickName() {
    return (_nickname);
}

User::~User() {
    //here I think I'll have to ... nothing srsly
}
