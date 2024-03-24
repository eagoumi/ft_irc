#include "server.hpp"

void Server::getregestred(int index, std::string data)
{

}

void Server::Authentication(int index, const char* data)
{
    std::string dataStr(data);

	//check fo the /n when applaying command
	size_t CmdNewLine = dataStr.find('\n');
	if (CmdNewLine != std::string::npos)
	{
		std::string Command;
		std::string cmd = dataStr.substr(0, CmdNewLine);
        std::cout << cmd << std::endl;
        size_t cmddoubledot = cmd.find(':');
        if (cmddoubledot != std::string::npos)
            cmd.erase(cmd.begin() + cmddoubledot);
		std::istringstream GetCmd(cmd);
		GetCmd >> Command;
        
        std::cout << Command << std::endl;
        if (!Command.empty() && _IsAuth == false && (Command == "pass" || Command == "PASS" || Command == "user" || Command == "nick" || Command == "USER" || Command == "NICK"))
        {
            if (_correct_pass == false && (Command == "pass" || Command == "PASS"))
            {
                std::string pass;
		        GetCmd >> pass;
                std::cout << pass << std::endl;
                if (pass == _Password)
                {
                    puts("ss");
                    _correct_pass = true;
                    std::string PassMsg = "Your password are correct, You can Register Now!\n";
                    send(_Storeusersfd[index].fd, PassMsg.c_str(), PassMsg.length(), 0);
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
            }
            else if (_UserCheck == false && _correct_pass == true && (Command == "user" || Command == "USER"))
            {
                std::string user;
		        GetCmd >> user;
                std::cout << user << std::endl;
                std::cout << cmd << std::endl;
                // setUserName(user);
            	std::string UserMsg = "Your User are " + user + "!\n";
                send(_Storeusersfd[index].fd, UserMsg.c_str(), UserMsg.length(), 0);
                _UserCheck = true;
                //limechat
            }
            else if (_UserCheck == true && _NickCheck == true && _correct_pass == true)
            {
            	std::string ErrorMsg = "Your are already Registed.\n";
                send(_Storeusersfd[index].fd, ErrorMsg.c_str(), ErrorMsg.length(), 0);
            }
            else
            {
            	std::string ErrorMsg = "Your Should to Register.\n";
                send(_Storeusersfd[index].fd, ErrorMsg.c_str(), ErrorMsg.length(), 0);
            }
        }
    }
}