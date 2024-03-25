#include "Commands.hpp"
#include <cctype>
#include <cstddef>


Commands::Commands()
{
    db = Database::GetInstance();
}

Commands::~Commands()
{
}

Commands::Commands(const Commands &obj)
{
    *this = obj;
}

Commands &Commands::operator=(const Commands &obj)
{
    (void)obj;
    // complete this
    return *this;
}

type    determine_cmd(std::string token) {

    if      (token == "JOIN")   return JOIN_CMD;
    else if (token == "TOPIC")  return TOPIC_CMD;
    else if (token == "KICK")   return KICK_CMD;
    else if (token == "INVITE") return INVITE_CMD;
    else if (token == "MODE")   return MODE_CMD;
                                return NONE;
}

type    determineToken(char sep, type cmdType, size_t paramCounter) {

    /* Command: JOIN   Parameters: <channel>{,<channel>}    [<key>{,<key>}]                         */
    /* Command: KICK   Parameters: <channel>                <user> *( "," <user> ) [<comment>]      */
    /* Command: TOPIC  Parameters: <channel>                [<topic>]                               */
    /* Command: INVITE Parameters: <nickname>               <channel>                               */
    /* Command: MODE   Parameters: <target>                 [<modestring> [<mode arguments>...]]    */
    type tokenType(NONE);
    if (sep == ',') {
        if      (cmdType == JOIN_CMD)   paramCounter == 1 ? tokenType = CHANNEL : (paramCounter == 2 ? tokenType = KEY  : tokenType = NONE);
        else if (cmdType == KICK_CMD)   paramCounter == 1 ? tokenType = NONE    : (paramCounter == 2 ? tokenType = NICK : tokenType = NONE);
        else if (cmdType == TOPIC_CMD)  tokenType = NONE;
        else if (cmdType == INVITE_CMD) tokenType = NONE;
        else if (cmdType == MODE_CMD)   tokenType = NONE;
    }
    else if (isspace(sep)) {
        if      (cmdType == JOIN_CMD)   paramCounter == 1 ? tokenType = CHANNEL : (paramCounter == 2 ? tokenType = KEY       : tokenType = NONE);
        else if (cmdType == KICK_CMD)   paramCounter == 1 ? tokenType = CHANNEL : (paramCounter == 2 ? tokenType = NICK      : tokenType = NONE);
        else if (cmdType == TOPIC_CMD)  paramCounter == 1 ? tokenType = CHANNEL : (paramCounter == 2 ? tokenType = TOPIC_MSG : tokenType = NONE);
        else if (cmdType == INVITE_CMD) paramCounter == 1 ? tokenType = NICK    : (paramCounter == 2 ? tokenType = CHANNEL   : tokenType = NONE);
        else if (cmdType == MODE_CMD)   paramCounter == 1 ? tokenType = CHANNEL : (paramCounter == 2 ? tokenType = MODE_STR  : tokenType = NONE);
    }
    return (tokenType);
}

std::vector<std::string> Commands::getNextParam(/*std::list<token>& tokensList*/) {

    static type tokenType;
    static bool firstTime = true;
    /*static std::list<token>* tokensListAddr = NULL;*/
    static std::list<token>::iterator it;
    std::vector<std::string> paramData;

    if (firstTime == true /*|| tokensListAddr != &tokensList*/) {
        it = _tokensList.begin();
        tokenType = (*it).type;
        /*tokensListAddr = &tokensList;*/
        firstTime = false;
    }
    if (it == _tokensList.end()) firstTime = true;

    while (it != _tokensList.end()) {
        if ((*it).type != tokenType && (*it).type != COMMA) { tokenType = (*it).type; break; }
        if ((it)->type != COMMA) paramData.push_back((*it).data);
        it++;
    }

    return (paramData);
}

void    Commands::tokenize(std::string const& cmdLine) {
    
    token               tokenNode;
	type                tokenType(NONE);
    std::string         word;
    size_t              tokenCounter = 0;
    size_t              parameterCounter = 0;

    for (size_t i = 0; i < cmdLine.length(); i++) {

        /************************************/
		/*			skip spaces				*/
		/************************************/
        while (isspace(cmdLine[i]) == true) {
            i++;
        }

        while (i <= cmdLine.length()) {
            /*********************************************************/
			/* so here if I encounter spaces or Comma store the word */
			/*********************************************************/
            if (isspace(cmdLine[i]) || cmdLine[i] == ',' || cmdLine[i] == '\0') {
                /*********************************************************************************/
				/* store word if it is not empty, it can be empty if this is the first iteration */
				/*********************************************************************************/
                if (!(word.empty()))
				{
                    tokenCounter == 0 ? tokenType = determine_cmd(word) : 0;
					tokenNode.data = word;
					tokenNode.type = tokenType;
					_tokensList.push_back(tokenNode);
					word.clear();
                    tokenCounter++;
				}

                if (cmdLine[i] == ',')
				{
					tokenType = COMMA;
					tokenNode.data = cmdLine[i];
					tokenNode.type = tokenType;
					_tokensList.push_back(tokenNode);
				}
                else if (isspace(cmdLine[i]))
                    parameterCounter++;

                // determine next token type
				if (isspace(cmdLine[i]) || cmdLine[i] == ',')
                    tokenType = determineToken(cmdLine[i], _tokensList.front().type, parameterCounter);
                break ;
            }
            word += cmdLine[i++];
        }
    }

}

void Commands::CommandMapinit(cmdData dataCmd)
{
    // Channel cObj;

    // std::cout << line << std::endl;

    std::string token;
    std::istringstream iss(dataCmd.line);
    // client = dataCmd.nick;
    fd = dataCmd.fd;

    while (iss >> token)
    {
        command.push_back(token);
        std::cout << "[" << token << "]" << std::endl;
    }

    // for (itV = command.begin(); itV != command.end(); itV++)
    //     std::cout << *itV << " ";
    // std::cout << std::endl;

    if (getCommand() == "JOIN")
        join();
    else if (getCommand() == "KICK")
        kick();
    // else if(getCommand() == "INVITE")
    //     invite();
    // else if(getCommand() == "MODE")
    //     mode();
    // else if(getCommand() == "TOPIC")
    //     topic();
    else
        sendResponse(":" + getClient() + " " + getCommand() + " :Unknown command\n");
    // std::cout << db << std::endl;
}

void Commands::sendResponse(std::string message)
{
    send(fd, message.c_str(), message.length(), 0);
}

std::string Commands::getNick()
{
    for (itV = command.begin(); itV != command.end(); itV++)
    {
        if (itV->find('#', 0) != std::string::npos)
        {
            itV++;
            return *itV;
        }
    }
    return "";
}
std::string Commands::getClient() const
{
    return client;
}

std::string Commands::getCommand() const
{
    if (command[1] == "KICK")
        return command[1];
    return command[0];
}

std::map<std::string, std::string> Commands::splitInput(std::string input)
{

    std::map<std::string, std::string> sChannels;
    std::map<std::string, std::string>::iterator it;

    std::istringstream str(input);
    std::string token;
    while (getline(str, token, ','))
    {
        sChannels.insert(std::make_pair(token, ""));
    }
    if (command[0] == "JOIN" && command[2] != "")
    {
        it = sChannels.begin();
        std::istringstream str2(command[2]);
        while (getline(str2, token, ','))
        {
            it->second = token;
            it++;
        }
    }
    // for (it = sChannels.begin(); it != sChannels.end(); it++)
    // {
    //     std::cout << it->first << "      " << it->second << std::endl;
    // }

    return sChannels;
}

void Commands::parsCommands()
{
    //    if(getChannel().find(',') != std::string::npos)
    //     puts("it's here");
    //     else
    //     puts("nothere");
    // else if(getCommand() == "KICK")
    //     parsKick();
}

std::string Commands::getChannel()
{
    for (itV = command.begin(); itV != command.end(); itV++)
    {
        if (itV->find('#', 0) != std::string::npos)
        {
            return *itV;
        }
    }
    return "";
}


std::string Commands::getHostName()
{
    char hostName[256];
    if (gethostname(hostName, sizeof(hostName)) != 0)
        return "";
    return hostName;
}

// std::string Commands::getKey(){

// }

// std::string Commands::get_comment() const{
//     if(command[3] != "\0")
//         return command[3];
//     else
//         return " ";
// }
// int Commands::get_fd() const{
//     return fd;
// }
// std::string Commands::get_owner() const{
//     return this->owner;
// }
