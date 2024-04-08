#include "Commands.hpp"
#include <sstream>

void Commands::Authentication(std::string const& serverPass) {

    std::string cmd = getCommand();
    if (cmd == "PASS")
    {
        if (currUser->hasInsertedPass() == false)
        {
            std::string pass = getNextParam().first;    
            if (pass == serverPass)
            {
                currUser->insertedPassSuccessfully();
                _logger.ServertoClient(RPL_CORRECTPASS(std::string("*")));
            }
            else if (pass.empty() || _paramCounter == 1)
                _logger.ServertoClient(ERR_NEEDMOREPARAMS(std::string("*")));
            else 
                _logger.ServertoClient(ERR_PASSWDMISMATCH(std::string("*")));
        }
        else
            _logger.ServertoClient(ERR_NOTREGISTERED(std::string("*")));
    }
    else if (cmd == "NICK")
    {
        if (!currUser->hasInsertedPass())
            _logger.ServertoClient(ERR_MISSPASS(std::string("*")));
        else if (currUser->hasInsertedNick() == false)
        {
            std::string nickname = getNextParam().first;
            if (db->isNicknameUsed(nickname) == true)
                _logger.ServertoClient(ERR_NICKNAMEINUSE(nickname));
            else if (!nickname.empty() && !currUser->isStrContains(nickname, " ,*?!@.") \
                    && !currUser->isStrStartWith(nickname, "$:#&+~%") && _paramCounter == 2)
                currUser->setNickName(nickname);
            else if (nickname.empty() || _paramCounter == 1)
                _logger.ServertoClient(ERR_NONICKNAMEGIVEN(std::string("*")));
            else
                _logger.ServertoClient(ERR_ERRONEUSNICKNAME(nickname));
        }
        else
        {
            std::string NickErrMsg = "You have already entered the nickname.\n";
            send(fd, NickErrMsg.c_str(), NickErrMsg.length(), 0);
        }
    }
    else if (cmd == "USER")
    {
        if (!currUser->hasInsertedPass())
            _logger.ServertoClient(ERR_MISSPASS(std::string("*")));
        else if (!currUser->hasInsertedNick())
        {
            std::string UserErrMsg = "Please enter your nickname first.\n";
            send(fd, UserErrMsg.c_str(), UserErrMsg.length(), 0);
        }
        else if (currUser->hasInsertedUsername() == false)
        {
            std::string user = getNextParam().first;
            if (!user.empty() || _paramCounter == 5 || _paramCounter == 2)
            {
                currUser->setUserName(user);
                std::string UserMsg = "Your username is " + user + ". You are now registered.\n";
                send(fd, UserMsg.c_str(), UserMsg.length(), 0);
            }
        }
        else
        {
            std::string UserErrMsg = "You are already registered.\n";
            send(fd, UserErrMsg.c_str(), UserErrMsg.length(), 0);
        }
        WelcomeClient();
    }
}

void Commands::WelcomeClient() {

    if (!currUser->getNickName().empty() && !currUser->getUserName().empty())
    {
        std::cout << currUser->getNickName() << " Is Logged in!" << std::endl;
        _logger.ServertoClient(RPL_WELCOME(currUser->getNickName(), "IRC",currUser->getUserName(), _logger.getServerIP()));
        _logger.ServertoClient(RPL_YOURHOST(currUser->getNickName(), _logger.getServerIP()));
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
