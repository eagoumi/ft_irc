#include "server.hpp"
#include "../Commands/Commands.hpp"

// void Server::getregestred(int index, std::string data)
// {

// }

// void Server::Authentication(int index, const char* data, bool _IsAuth, bool _correct_pass, bool _NickCheck, bool _UserCheck)
// {
//     bool                            _IsAuth        =     false;
//     bool                            _correct_pass  =     false;
//     bool                            _NickCheck     =     false;
//     bool                            _UserCheck     =     false;
//     std::string dataStr(data);

// 	//check fo the /n when applaying command
// 	size_t CmdNewLine = dataStr.find('\n');
// 	if (CmdNewLine != std::string::npos)
// 	{
// 		std::string Command;
// 		std::string cmd = dataStr.substr(0, CmdNewLine);
//         std::cout << cmd << std::endl;
//         size_t cmddoubledot = cmd.find(':');
//         if (cmddoubledot != std::string::npos)
//             cmd.erase(cmd.begin() + cmddoubledot);
// 		std::istringstream GetCmd(cmd);
// 		GetCmd >> Command;
        
//         std::cout << _correct_pass << std::endl;
//         if (!Command.empty() && _IsAuth == false && (Command == "pass" || Command == "PASS" || Command == "user" || Command == "nick" || Command == "USER" || Command == "NICK"))
//         {
//             if (_correct_pass == false && (Command == "pass" || Command == "PASS"))
//             {
//                 std::string pass;
// 		        GetCmd >> pass;
//                 std::cout << pass << std::endl;
//                 if (pass == _Password)
//                 {
//                     puts("ss");
//                     _correct_pass = true;
//                     std::string PassMsg = "Your password are correct, You can Register Now!\n";
//                     send(_Storeusersfd[index].fd, PassMsg.c_str(), PassMsg.length(), 0);
//                 }
                
//             }
//             else if (_NickCheck == false && _correct_pass == true && (Command == "nick" || Command == "NICK"))
//             {
//                 std::string nickname;
//             	GetCmd >> nickname;
//                 // puts ("sss");
//                 std::cout << cmd << std::endl;
//                 std::cout << nickname << std::endl;
//             	// SetClientNickName(_Storeusersfd[index].fd, nickname);
//             	std::string WlcmClientMsg = "Welcome " + nickname + "!\n";
//             	send(_Storeusersfd[index].fd, WlcmClientMsg.c_str(), WlcmClientMsg.length(), 0);
//                 _NickCheck = true;
//             }
//             else if (_UserCheck == false && _correct_pass == true && (Command == "user" || Command == "USER"))
//             {
//                 std::string user;
// 		        GetCmd >> user;
//                 std::cout << user << std::endl;
//                 std::cout << cmd << std::endl;
//                 // setUserName(user);
//             	std::string UserMsg = "Your User are " + user + "!\n";
//                 send(_Storeusersfd[index].fd, UserMsg.c_str(), UserMsg.length(), 0);
//                 _UserCheck = true;
//                 //limechat
//             }
//             else if (_UserCheck == true && _NickCheck == true && _correct_pass == true)
//             {
//                 _IsAuth = true;
//             	std::string ErrorMsg = "Your are already Registed.\n";
//                 send(_Storeusersfd[index].fd, ErrorMsg.c_str(), ErrorMsg.length(), 0);
//             }
//             else
//             {
//             	std::string ErrorMsg = "Your Should to Register.\n";
//                 send(_Storeusersfd[index].fd, ErrorMsg.c_str(), ErrorMsg.length(), 0);
//             }
//         }
//     }
// }

void Server::Authentication(int index, const char* data, bool& _IsAuth, bool& _correct_pass, bool& _NickCheck, bool& _UserCheck)
{
    std::string dataStr(data);
    cmdData dataCmd;
    Commands obj;
	//check fo the /n when applaying command
	size_t CmdNewLine = dataStr.find('\n');
	if (CmdNewLine != std::string::npos)
	{
		std::string Command;
		std::string cmd = dataStr.substr(0, CmdNewLine);
        dataCmd.line = cmd;
        // std::cout << cmd << std::endl;
        size_t cmddoubledot = cmd.find(':');
        if (cmddoubledot != std::string::npos)
            cmd.erase(cmd.begin() + cmddoubledot);
		std::istringstream GetCmd(cmd);
		GetCmd >> Command;
        
        // std::cout << Command << std::endl;
        dataCmd.fd = _Storeusersfd[index].fd;
        if (!Command.empty() && _IsAuth == false && (Command == "pass" || Command == "PASS" || Command == "user" || Command == "nick" || Command == "USER" || Command == "NICK"))
        {
            if (Command == "pass" || Command == "PASS")
            {
                if (_correct_pass == false)
                {
                    std::string pass;
                    GetCmd >> pass;
                    // std::cout << pass << std::endl;
                    if (pass == _Password)
                    {
                        _correct_pass = true;
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
            else if (_NickCheck == false && _correct_pass == true && (Command == "nick" || Command == "NICK"))
            {
                std::string nickname;
            	GetCmd >> nickname;
                // puts ("sss");
                std::cout << cmd << std::endl;
                std::cout << nickname << std::endl;
            	// SetClientNickName(_Storeusersfd[index].fd, nickname);
            	std::string WlcmClientMsg = "Welcome " + nickname + "!\n";
            	send(_Storeusersfd[index].fd, WlcmClientMsg.c_str(), WlcmClientMsg.length(), 0);
                _NickCheck = true;
                dataCmd.nick = nickname;
            }
            else if (Command == "nick" || Command == "NICK")
            {
                if (!_correct_pass)
                {
                    std::string NickErrMsg = "Please enter the password to connect to the server first.\n";
                    send(_Storeusersfd[index].fd, NickErrMsg.c_str(), NickErrMsg.length(), 0);
                }
                else if (_NickCheck == false)
                {
                    std::string nickname;
                    GetCmd >> nickname;
                    // std::cout << cmd << std::endl;
                    // std::cout << nickname << std::endl;
                    std::string WlcmClientMsg = "Welcome " + nickname + "!\n";
                    send(_Storeusersfd[index].fd, WlcmClientMsg.c_str(), WlcmClientMsg.length(), 0);
                    _NickCheck = true;
                }
                else
                {
                    std::string NickErrMsg = "You have already entered the nickname.\n";
                    send(_Storeusersfd[index].fd, NickErrMsg.c_str(), NickErrMsg.length(), 0);
                }
            }
            else if (Command == "user" || Command == "USER")
            {
                _IsAuth = true;
            	std::string ErrorMsg = "Your are already Registed.\n";
                send(_Storeusersfd[index].fd, ErrorMsg.c_str(), ErrorMsg.length(), 0);
            }
            else
            {
                std::string ErrorMsg = "Invalid command or sequence.\n";
                send(_Storeusersfd[index].fd, ErrorMsg.c_str(), ErrorMsg.length(), 0);
            }
        }
        // else if(_IsAuth == true)
        // {
            obj.CommandMapinit(dataCmd);
        // }

    }
}