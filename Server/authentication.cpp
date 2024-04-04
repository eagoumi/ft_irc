#include "server.hpp"
#include <sstream>

std::string Server::skipSpace(std::string string)
{
    std::string skiip;
    int i= 0;
    while (string[i] == ' ')
        i++;
    skiip = string.substr(i, string.length());
    return skiip;
}

size_t Parameter_number(std::string data)
{
    size_t len = 0;
    std::string param;
    std::istringstream string(data);

    while (string >> param)
        len++;
    return len;
}

void Server::Authentication(int index, const char* data)//, bool& _IsAuth, bool& _correct_pass, bool& _NickCheck, bool& _UserCheck)
{
    std::string dataStr(data);

    //check for the newline when applying command
    size_t CmdNewLine = dataStr.find_first_of("\r\n");
    if (CmdNewLine != std::string::npos)
    {
        User *currUser = _db->getUser(_Storeusersfd[index].fd);
        std::string Command;
        std::string cmd = dataStr.substr(0, CmdNewLine);
        size_t len_param_command = Parameter_number(cmd);
        // std::cout << "param len: " << len_param_command << std::endl;
        size_t cmddoubledot = cmd.find(':');
        if (cmddoubledot != std::string::npos)
            cmd.erase(cmd.begin() + cmddoubledot);
        std::istringstream GetCmd(cmd);
        GetCmd >> Command;

        // std::cout << Command << std::endl;
        if (!Command.empty()) //&& currUser->isAuthenticated() == false) // && (Command == "pass" || Command == "PASS" || Command == "user" || Command == "nick" || Command == "USER" || Command == "NICK"))
        {
            if (Command == "pass" || Command == "PASS")
            {
                if (currUser->hasInsertedPass() == false)
                {
                    std::string pass;
                    pass = skipSpace(cmd.substr(Command.length(), CmdNewLine));
                    // GetCmd >> pass;
                    // std::cout << "here'" << _Password << "'here" << std::endl;
                    // std::cout << "here'" << pass << "'here" << std::endl;
                    // std::cout << _Password << std::endl;
                    if (pass == skipSpace(_Password)) //&& len_param_command == 2)
                    {
                        currUser->insertedPassSuccessfully();
                        // _logger.ServertoClient(ERR_CORRECTPASS(std::string("*")));
                    }
                    else if (pass.empty() || len_param_command == 1)
                        _logger.ServertoClient(ERR_NEEDMOREPARAMS(std::string("*")));
                    else 
                        _logger.ServertoClient(ERR_PASSWDMISMATCH(std::string("*")));
                }
                else
                    _logger.ServertoClient(ERR_NOTREGISTERED(std::string("*")));
            }
            else if (Command == "nick" || Command == "NICK")
            {
                if (!currUser->hasInsertedPass())
                    _logger.ServertoClient(ERR_MISSPASS(std::string("*")));
                else if (currUser->hasInsertedNick() == false)
                {
                    std::string nickname;
                    GetCmd >> nickname;
                    // std::cout << nickname << std::endl;
                    if (_db->isNicknameUsed(nickname) == true)
                        _logger.ServertoClient(ERR_NICKNAMEINUSE(nickname));
                    else if (!nickname.empty() && !currUser->isStrContains(nickname, " ,*?!@.") \
                            && !currUser->isStrStartWith(nickname, "$:#&+~%") && len_param_command == 2)
                        currUser->setNickName(nickname);
                    else if (nickname.empty() || len_param_command == 1)
                        _logger.ServertoClient(ERR_NONICKNAMEGIVEN(std::string("*")));
                    else
                        _logger.ServertoClient(ERR_ERRONEUSNICKNAME(nickname));
                }
                else
                {
                    std::string NickErrMsg = "You have already entered the nickname.\n";
                    send(_Storeusersfd[index].fd, NickErrMsg.c_str(), NickErrMsg.length(), 0);
                }
            }
            else if (Command == "user" || Command == "USER")
            {
                if (!currUser->hasInsertedPass())
                    _logger.ServertoClient(ERR_MISSPASS(std::string("*")));
                else if (!currUser->hasInsertedNick())
                {
                    std::string UserErrMsg = "Please enter your nickname first.\n";
                    send(_Storeusersfd[index].fd, UserErrMsg.c_str(), UserErrMsg.length(), 0);
                }
                else if (currUser->hasInsertedUsername() == false)
                {
                    std::string user;
                    GetCmd >> user;
                    if (!user.empty() || len_param_command == 5 || len_param_command == 2)
                    {
                        currUser->setUserName(user);
                        std::string UserMsg = "Your username is " + user + ". You are now registered.\n";
                        send(_Storeusersfd[index].fd, UserMsg.c_str(), UserMsg.length(), 0);
                    }
                }
                else
                {
                    std::string UserErrMsg = "You are already registered.\n";
                    send(_Storeusersfd[index].fd, UserErrMsg.c_str(), UserErrMsg.length(), 0);
                }
                WelcomeClient(currUser);
            }
            else
                _logger.ServertoClient(ERR_UNKNOWNCOMMAND(std::string("*"), Command));
        }
    }
}

void Server::WelcomeClient(User *currUser)
{

    // User *currUser = _db->getUser(fd);
    if (!currUser->getNickName().empty() && !currUser->getUserName().empty())
    {
        std::cout << currUser->getNickName() << " Is Logged in!" << std::endl;
        _logger.ServertoClient(RPL_WELCOME(currUser->getNickName(), "IRC",currUser->getUserName(), _IPHostAdress));
        _logger.ServertoClient(RPL_YOURHOST(currUser->getNickName(), _IPHostAdress));
        _logger.ServertoClient(RPL_MOTDSTART(currUser->getNickName()));
        _logger.ServertoClient(RPL_MOTD(currUser->getNickName(), " ██╗██████╗  █████╗ ███████╗██╗      █████╗ ██╗    ██╗███╗   ██╗    ███╗   ██╗    ██╗██████╗  ██████╗  "));
        _logger.ServertoClient(RPL_MOTD(currUser->getNickName(), " ██║██╔══██╗██╔══██╗╚════██║██║     ██╔══██╗██║    ██║████╗  ██║    ████╗  ██║    ██║██╔══██╗██╔════╝  "));
        _logger.ServertoClient(RPL_MOTD(currUser->getNickName(), " ██║██████╔╝███████║    ██╔╝██║     ███████║██║ █╗ ██║██╔██╗ ██║    ██╔██╗ ██║    ██║██████╔╝██║       "));
        _logger.ServertoClient(RPL_MOTD(currUser->getNickName(), " ██║██╔══██╗██╔══██║   ██╔╝ ██║     ██╔══██║██║███╗██║██║╚██╗██║    ██║╚██╗██║    ██║██╔══██╗██║       "));
        _logger.ServertoClient(RPL_MOTD(currUser->getNickName(), " ██║██████╔╝██║  ██║   ██║  ███████╗██║  ██║╚███╔███╔╝██║ ╚████║    ██║ ╚████║    ██║██║  ██║╚██████╗  "));
        _logger.ServertoClient(RPL_MOTD(currUser->getNickName(), " ╚═╝╚═════╝ ╚═╝  ╚═╝   ╚═╝  ╚══════╝╚═╝  ╚═╝ ╚══╝╚══╝ ╚═╝  ╚═══╝    ╚═╝  ╚═══╝    ╚═╝╚═╝  ╚═╝ ╚═════╝  "));
        _logger.ServertoClient(RPL_MOTD(currUser->getNickName(), " "));
        _logger.ServertoClient(RPL_ENDOFMOTD(currUser->getNickName()));
        _logger.ServertoClient(RPL_UMODEIS(currUser->getNickName(), "+w"));

    }
}
