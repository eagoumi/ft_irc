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
		std::istringstream GetCmd(cmd);
		GetCmd >> Command;
        if (!Command.empty() && _IsAuth == false && (Command == "pass" || Command == "PASS" || Command == "user" || Command == "nick"))
        {
            if (_correct_pass == false && (Command == "pass" || Command == "PASS"))
            {
                std::string pass;
		        GetCmd >> pass;
                if (pass == _Password)
                {
                    _correct_pass = true;
                    std::string PassMsg = "Your password are correct, You can Register Now!\n";
                    send(_Storeusersfd[index].fd, PassMsg.c_str(), PassMsg.length(), 0);
                }
                
            }
            else if (_NickCheck == false && _correct_pass == true && (Command == "nick" || Command == "NICK"))
            {
                std::string nickname;
            	GetCmd >> nickname;
            	// SetClientNickName(_Storeusersfd[index].fd, nickname);
            	std::string WlcmClientMsg = "Welcome " + nickname + "!\n";
            	send(_Storeusersfd[index].fd, WlcmClientMsg.c_str(), WlcmClientMsg.length(), 0);
                _NickCheck = true;
            }
            else if (_UserCheck == true && _correct_pass == true && (Command == "user" || Command == "USER"))
            {
                std::string user;
		        GetCmd >> user;
                // setUserName(user);
            	std::string UserMsg = "Your User are " + user + "!\n";
                send(_Storeusersfd[index].fd, UserMsg.c_str(), UserMsg.length(), 0);
                _UserCheck = true;
                //limechat
            }
            else if (_UserCheck == false && _NickCheck == true && _correct_pass == true)
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