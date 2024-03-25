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
    //complete this
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

std::vector<std::string> getNextParam(std::list<token>& tokensList) {

    static type tokenType;
    static bool firstTime = true;
    static std::list<token>* tokensListAddr = NULL; 
    static std::list<token>::iterator it;
    std::vector<std::string> paramData;

    if (firstTime == true || tokensListAddr != &tokensList) {
        it = tokensList.begin();
        tokenType = (*it).type;
        tokensListAddr = &tokensList;
        firstTime = false;
    }
    if (it == tokensList.end()) firstTime = true;

    while (it != tokensList.end()) {
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
    client = dataCmd.nick;
    fd = dataCmd.fd;
    std::cout << "FD = " << fd << std::endl;
	std::cout << db << std::endl;

    while(iss >> token)
    {
        command.push_back(token);
        std::cout << "[" << token << "]" << std::endl;
    }

    for (itV = command.begin(); itV != command.end(); itV++)
        std::cout << *itV << " ";
    std::cout << std::endl;


    // channels.insert(std::make_pair(fd, getChannel()));
    if(getCommand() == "JOIN")
        join();
    else if(getCommand() == "KICK")
        kick();
    // else if(getCommand() == "INVITE")
    //     invite();
    // else if(getCommand() == "MODE")
    //     mode();
    // else if(getCommand() == "TOPIC")
    //     topic();
    else
        // std::cout << ":" << get_nickName() << " " << getCommand() << " :Unknown command" << std::endl;
        sendResponse(":" + getClient() + " " + getCommand() + " :Unknown command\n");
	std::cout << db << std::endl;
    
}


// void Commands::parsCommands(std::string str, std::string owner, std::string host, int fd){
//     (void)host;
//     this->fd = fd;
//     this->owner = owner;

//     // std::cout << "host " << host << std::endl;
//     // std::cout << "fd " << fd << std::endl;
//     // this->command = ft_trim(str);
//     fillMultimap();
//     this->command = ft_split(str);
//     if(get_command() == "kick" || get_command() == "KICK")
//         kick();
//     else if(get_command() == "invite" || get_command() == "INVITE")
//         invite();
//     else if(get_command() == "topic" || get_command() == "TOPIC")
//         topic();
//     else if(get_command() == "mode" || get_command() == "MODE")
//         mode();
//     else
//         std::cout << ":" << get_nickName() << " " << get_command() << " :Unknown command" << std::endl;
//     for(this->itCh = channels.begin(); this->itCh != channels.end(); this->itCh++)
//         std::cout << itCh->first << " " << itCh->second << std::endl;

// }

// void Commands::fillMultimap(){
//     // channels.insert(std::make_pair("friend", "@yousra"));
//     // channels.insert(std::make_pair("friend", "meryem"));
//     // channels.insert(std::make_pair("friend", "asmaa"));
//     // channels.insert(std::make_pair("friend", "hiba"));

//     channels.insert(std::make_pair("#family", "@amine"));
//     channels.insert(std::make_pair("#family", "sara"));
//     channels.insert(std::make_pair("#family", "anas"));
//     channels.insert(std::make_pair("#family", "yousra"));

//     // channels.insert(std::make_pair("parents", "@latifa"));
//     // channels.insert(std::make_pair("parents", "mehdi"));
//     // channels.insert(std::make_pair("parents", "yousra"));

// }

void Commands::sendResponse(std::string message)
{
	send(fd, message.c_str(), message.length(), 0);
}


std::string Commands::getNick() {
     for(itV = command.begin() ; itV != command.end(); itV++)
    {
        if(itV->find('#', 0) != std::string::npos)
        {
            itV++;
            return *itV;
        }
    }
    return "";
}
std::string Commands::getClient() const{
    return client;
}
std::string Commands::getCommand() const{
    if(command[1] == "KICK")
        return command[1];
    return command[0];

}
std::string Commands::getChannel() {
    for(itV = command.begin() ; itV != command.end(); itV++)
    {
        if(itV->find('#', 0) != std::string::npos)
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
