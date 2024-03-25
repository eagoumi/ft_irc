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
    _isAuthenticated = false;
    _Correct_Password = false;
    _UserCheck = false;
    _NickCheck = false;
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


//by agoumi
void User::SetAuthenticated()
{
    if (_UserCheck == true && _NickCheck == true && _Correct_Password == true)
        _isAuthenticated = true;
}

bool User::isAuthenticated()
{
    return (_isAuthenticated);
}

//checking if the pass word are entred and correct
void User::Correct_Password()
{
    _Correct_Password = true;
}

bool User::isCorrect_Password()
{
    return (_Correct_Password);
}

//checking fo entring NickName
void User::NickCheck()
{
    _NickCheck = true;
}

bool User::isNickCheck()
{
    return (_NickCheck);
}

//checking fo entring NickName
void User::UserCheck()
{
    _UserCheck = true;
}

bool User::isUserCheck()
{
    return (_UserCheck);
}


User::~User() {
    //here I think I'll have to ... nothing srsly
}
