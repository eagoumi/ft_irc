#include "./Commands.hpp"
#include <cctype>
#include <cstddef>
#include <vector>


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

token_type    determine_cmd(std::string token) {

    if      (token == "JOIN")       return JOIN_CMD;
    else if (token == "TOPIC")      return TOPIC_CMD;
    else if (token == "KICK")       return KICK_CMD;
    else if (token == "INVITE")     return INVITE_CMD;
    else if (token == "MODE")       return MODE_CMD;
    else if (token == "LOGTIME")    return LOGTIME_CMD;           
    return NONE;
}

token_type    determineToken(char sep, token_type cmdType, size_t paramCounter) {

    /* Command: JOIN   Parameters: <channel>{,<channel>}    [<key>{,<key>}]                         */
    /* Command: KICK   Parameters: <channel>                <user> *( "," <user> ) [<comment>]      */
    /* Command: TOPIC  Parameters: <channel>                [<topic>]                               */
    /* Command: INVITE Parameters: <nickname>               <channel>                               */
    /* Command: MODE   Parameters: <target>                 [<modestring> [<mode arguments>...]]    */
    token_type tokenType(NONE);
    if (sep == ',') {
        if      (cmdType == JOIN_CMD)       paramCounter == 1 ? tokenType = CHANNEL : (paramCounter == 2 ? tokenType = KEY  : tokenType = NONE);
        else if (cmdType == KICK_CMD)       paramCounter == 1 ? tokenType = NONE    : (paramCounter == 2 ? tokenType = NICK : tokenType = NONE);
        else if (cmdType == TOPIC_CMD)      tokenType = NONE;
        else if (cmdType == INVITE_CMD)     tokenType = NONE;
        else if (cmdType == MODE_CMD)       tokenType = NONE;
        else if (cmdType == LOGTIME_CMD)    tokenType = NONE;
    }
    else if (isspace(sep)) {
        if      (cmdType == JOIN_CMD)   paramCounter == 1 ? tokenType = CHANNEL : (paramCounter == 2 ? tokenType = KEY       : tokenType = NONE);
        else if (cmdType == KICK_CMD)   paramCounter == 1 ? tokenType = CHANNEL : (paramCounter == 2 ? tokenType = NICK      : tokenType = NONE);
        else if (cmdType == TOPIC_CMD)  paramCounter == 1 ? tokenType = CHANNEL : (paramCounter == 2 ? tokenType = TOPIC_MSG : tokenType = NONE);
        else if (cmdType == INVITE_CMD) paramCounter == 1 ? tokenType = NICK    : (paramCounter == 2 ? tokenType = CHANNEL   : tokenType = NONE);
        else if (cmdType == MODE_CMD)   paramCounter == 1 ? tokenType = CHANNEL : (paramCounter == 2 ? tokenType = MODE_STR  : tokenType = NONE);
        else if (cmdType == LOGTIME_CMD) paramCounter == 1 ? tokenType = NICK : tokenType = NONE;
    }
    return (tokenType);
}

//call it when you are sure from the command syntax
std::vector<std::string> Commands::getNextParam(reset option) {

    static token_type tokenType;
    static bool firstTime = true;
    static std::list<token>::iterator it;
    std::vector<std::string> paramData;

    if (option == RESET) { firstTime = true; return std::vector<std::string>(); }
    if (firstTime == true) {
        it = ++_tokensList.begin();
        tokenType = (*it).type;
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

void   Commands::tokenize(std::string const& cmdLine) {
    
    token               tokenNode;
	token_type          tokenType(NONE);
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
                    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
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
    getNextParam(RESET);
}

void Commands::checkTokensListSyntax()
{
	std::list<token>::iterator ListIt = _tokensList.begin();
    token_type cmd = _tokensList.front().type;

    _tokensList.size() == 0 ? throw std::string("TokenList is empty => cmdLine is empty") : NULL;
    if (cmd == NONE) sendResponse(fd, ERR_UNKNOWNCOMMAND(currUser->getNickName(), getCommand()));
	while (ListIt != _tokensList.end())
	{
        if ((*ListIt).type == CHANNEL)
            if ((*ListIt).data[0] != '#') (*ListIt).data.insert(0, 1, '#');
        ListIt++;
	}
}

void Commands::CommandMapinit(cmdData dataCmd)
{
    // std::cout << "command : " << getCommand() << std::endl;
    client = dataCmd.nick;
    fd = dataCmd.fd;
    currUser = db->getUser(fd);

    /***********************************************************************************/
    tokenize(dataCmd.line);
    try {
        checkTokensListSyntax();
    }
    catch(std::string err) {
        std::cout << err << std::endl;
        return ;
    }
    /* Here I need to verfiy of tokens List has a true sytnax, If not I'll print error */
    /* SO YOU WILL NEED TO ENTER COMMANDS THAT ARE WELL SYNTAXED FOR NOW */
    /***********************************************************************************/

    // std::vector<std::string> tokenParam;
    // std::vector<std::string>::iterator it;
    // std::list<token> input = tokenize(dataCmd.line);
    // tokenParam = getNextParam(input);
    // for(it = tokenParam.begin(); it != tokenParam.end(); it++)
    //     std::cout << "[" << *it << "]" << std::endl;
    // Channel cObj;

    // std::cout << line << std::endl;

// std::cout << "what" << std::endl;
    // std::string token;
    // std::istringstream iss(dataCmd.line);

    // while (iss >> token)
    // {
    //     command.push_back(token);
    //     // std::cout << "[" << token << "]" << std::endl;
    // }

    // for (itV = command.begin(); itV != command.end(); itV++)
    //     std::cout << *itV << " ";
    // std::cout << std::endl;
    // if (currUser == NULL)   
    //     std::cout << "user is null within commandMapINit()" << std::endl;
    // User *currUser = db->getUser(fd);
    std::string cmd = getCommand();
    if (cmd == "JOIN")
        join();
    else if (cmd == "KICK")
        kick();
    else if (cmd == "LOGTIME")
        logtime();
    else if(cmd == "INVITE")
        invite();
    else if(cmd == "TOPIC")
        topic();
    else if(cmd == "MODE")
        mode();
    // else
    //     sendResponse(fd, ERR_UNKNOWNCOMMAND(currUser->getNickName(), cmd));

    // if (getCommand() == "JOIN" || getCommand() == "join") 
    //     join();
    // else if (getCommand() == "KICK" || getCommand() == "kick")
    //     kick();
    // else if (getCommand() == "LOGTIME")
    //     logtime();
    // else if(getCommand() == "INVITE" || getCommand() == "invite")
    //     invite();
    // else if(getCommand() == "TOPIC" || getCommand() == "topic")
    //     topic();
    // else if(getCommand() == "MODE" || getCommand() == "mode")
    //     mode();
    // else
    //     sendResponse(fd, ":" + getClient() + " " + getCommand() + " :Unknown command\n");
}

void Commands::sendResponse(int userfd, std::string message)
{
    send(userfd, message.c_str(), message.length(), 0);
}

std::string Commands::getNick()
{
    if(getCommand() == "invite" || getCommand() == "INVITE")
        return command[1];
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
    // if (command[1] == "KICK")
    //     return command[1];
    // return command[0];
    return _tokensList.front().data;
}

std::string Commands::getCommentTopic(){
    // std::cout << "WHY " <<command[2] << std::endl;
    if(command[2] != "")
        return command[2];
    else
        return "";
}

void Commands::seTopic(std::string newTopic)
{
    command[2] = newTopic;
}

// std::string Commands::geTopic()
// {
//     if(command[2])
// }
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


std::string Commands::getChannel()
{
    if(getCommand() == "mode" || getCommand() == "MODE")
        return command[1];
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
