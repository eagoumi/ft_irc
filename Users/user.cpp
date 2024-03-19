# include "./user.hpp"

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
    _nickname = name;
}

NICK_NAME const & User::getNickName() {
    return (_nickname);
}

User::~User() {
    //here I think I'll have to ... nothing srsly
}
