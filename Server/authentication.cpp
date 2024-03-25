#include "server.hpp"

void Server::Authentication(int index, const char* data)//, bool& _IsAuth, bool& _correct_pass, bool& _NickCheck, bool& _UserCheck)
{
    std::string dataStr(data);
        // bool                            _IsAuth        =     false;
		bool                            _correct_pass  =     false;
		bool                            _NickCheck     =     false;
		bool                            _UserCheck     =     false;

    //check for the newline when applying command
    size_t CmdNewLine = dataStr.find('\n');
    if (CmdNewLine != std::string::npos)
    {
        std::cout << "user to create : " << _Storeusersfd[index].fd << std::endl;
        User *Add_User = _db->getUser(_Storeusersfd[index].fd);
        std::string Command;
        std::string cmd = dataStr.substr(0, CmdNewLine);
        // std::cout << cmd << std::endl;
        size_t cmddoubledot = cmd.find(':');
        if (cmddoubledot != std::string::npos)
            cmd.erase(cmd.begin() + cmddoubledot);
        std::istringstream GetCmd(cmd);
        GetCmd >> Command;

        // std::cout << _correct_pass << std::endl;
        if (!Command.empty()) //&& Add_User->isAuthenticated() == false) // && (Command == "pass" || Command == "PASS" || Command == "user" || Command == "nick" || Command == "USER" || Command == "NICK"))
        {
            if (Command == "pass" || Command == "PASS")
            {
                if (Add_User->isCorrect_Password() == false)
                {
                    std::string pass;
                    GetCmd >> pass;
                    // std::cout << pass << std::endl;
                    if (pass == _Password)
                    {
                        Add_User->Correct_Password();
                        std::string PassMsg = "Your password is correct. Please enter your nickname.\n";
                        send(_Storeusersfd[index].fd, PassMsg.c_str(), PassMsg.length(), 0);
                    }
                    else
                    {
                        std::string PassErrMsg = "Incorrect password. Please try again.\n";
                        send(_Storeusersfd[index].fd, PassErrMsg.c_str(), PassErrMsg.length(), 0);
                    }
                }
                else
                {
                    std::string PassErrMsg = "You have already entered the password.\n";
                    send(_Storeusersfd[index].fd, PassErrMsg.c_str(), PassErrMsg.length(), 0);

                }
            }
            else if (Command == "nick" || Command == "NICK")
            {
                if (!Add_User->isCorrect_Password())
                {
                    std::string NickErrMsg = "Please enter the password to connect to the server first.\n";
                    send(_Storeusersfd[index].fd, NickErrMsg.c_str(), NickErrMsg.length(), 0);
                }
                else if (Add_User->isNickCheck() == false)
                {
                    std::string nickname;
                    GetCmd >> nickname;
                    Add_User->setNickName(nickname);
                    // std::cout << cmd << std::endl;
                    // std::cout << nickname << std::endl;
                    std::string WlcmClientMsg = "Welcome " + nickname + "!\n";
                    send(_Storeusersfd[index].fd, WlcmClientMsg.c_str(), WlcmClientMsg.length(), 0);
                    Add_User->NickCheck();
                }
                else
                {
                    std::string NickErrMsg = "You have already entered the nickname.\n";
                    send(_Storeusersfd[index].fd, NickErrMsg.c_str(), NickErrMsg.length(), 0);
                }
            }
            else if (Command == "user" || Command == "USER")
            {
                if (!Add_User->isCorrect_Password())
                {
                    std::string UserErrMsg = "Please enter the password to connect to the server first.\n";
                    send(_Storeusersfd[index].fd, UserErrMsg.c_str(), UserErrMsg.length(), 0);
                }
                else if (!Add_User->isNickCheck())
                {
                    std::string UserErrMsg = "Please enter your nickname first.\n";
                    send(_Storeusersfd[index].fd, UserErrMsg.c_str(), UserErrMsg.length(), 0);
                }
                else if (Add_User->isUserCheck() == false)
                {
                    std::string user;
                    GetCmd >> user;
                    Add_User->setUserName(user);
                    // std::cout << user << std::endl;
                    // std::cout << cmd << std::endl;
                    std::string UserMsg = "Your username is " + user + ". You are now registered.\n";
                    send(_Storeusersfd[index].fd, UserMsg.c_str(), UserMsg.length(), 0);
                    Add_User->UserCheck();
                }
                else
                {
                    std::string UserErrMsg = "You are already registered.\n";
                    send(_Storeusersfd[index].fd, UserErrMsg.c_str(), UserErrMsg.length(), 0);
                }
                Add_User->SetAuthenticated();
            }
            else
            {
                std::string ErrorMsg = "Invalid command or sequence.\n";
                send(_Storeusersfd[index].fd, ErrorMsg.c_str(), ErrorMsg.length(), 0);
            }
        }
    }
}