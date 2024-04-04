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

token_type determine_cmd(std::string token)
{

    std::transform(token.begin(), token.end(), token.begin(), ::toupper);
    if (token == "JOIN")
        return JOIN_CMD;
    else if (token == "PRIVMSG")
        return PRIVMSG_CMD;
    else if (token == "TOPIC")
        return TOPIC_CMD;
    else if (token == "KICK")
        return KICK_CMD;
    else if (token == "PART")
        return PART_CMD;
    else if (token == "INVITE")
        return INVITE_CMD;
    else if (token == "MODE")
        return MODE_CMD;
    else if (token == "LOGTIME")
        return LOGTIME_CMD;
    else if (token == "WHOIS")
        return WHOIS_CMD;
    else if (token == "LOCATION")
        return LOCATION_CMD;
    return NONE;
}

token_type    Commands::determineToken(char sep, token_type cmdType) {

    /* Command:             PRIVMSG         Parameters: <target>{,<target>}             <text to be sent>                       */
    /* Command:             JOIN            Parameters: <channel>{,<channel>}           [<key>{,<key>}]                         */
    /* Command:             KICK            Parameters: <channel>                       <user> *( "," <user> ) [<comment>]      */
    /* Command:             PART            Parameters: <channel>{,<channel>}           [<reason>]                              */
    /* Command:             TOPIC           Parameters: <channel>                       [<topic>]                               */
    /* Command:             INVITE          Parameters: <nickname>                      <channel>                               */
    /* Command:             MODE            Parameters: <target>                        [<modestring> [<mode arguments>...]]    */
    token_type tokenType(NONE);
    if (sep == ',') {
        if      (cmdType == JOIN_CMD)       _paramCounter == 1 ? tokenType = NICK    : tokenType = NONE;
        else if (cmdType == PRIVMSG_CMD)    _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = KEY  : tokenType = NONE);
        else if (cmdType == PART_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = KEY  : tokenType = NONE);
        else if (cmdType == KICK_CMD)       _paramCounter == 1 ? tokenType = NONE    : (_paramCounter == 2 ? tokenType = NICK : tokenType = NONE);
        else if (cmdType == TOPIC_CMD)      tokenType = NONE;
        else if (cmdType == INVITE_CMD)     tokenType = NONE;
        else if (cmdType == MODE_CMD)       tokenType = NONE;
        else if (cmdType == LOGTIME_CMD)    tokenType = NONE;
    }
    else if (isspace(sep)) {
        if      (cmdType == JOIN_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = KEY       : tokenType = NONE);
        else if (cmdType == PRIVMSG_CMD)    _paramCounter == 1 ? tokenType = NICK    : (_paramCounter == 2 ? tokenType = MSG       : tokenType = NONE);
        else if (cmdType == PART_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = REASON    : tokenType = NONE);
        else if (cmdType == KICK_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = NICK      : (_paramCounter == 3 ? tokenType = COMMENT  : tokenType = NONE));
        else if (cmdType == TOPIC_CMD)      _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = TOPIC_MSG : tokenType = NONE);
        else if (cmdType == INVITE_CMD)     _paramCounter == 1 ? tokenType = NICK    : (_paramCounter == 2 ? tokenType = CHANNEL   : tokenType = NONE);
        else if (cmdType == MODE_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = MODE_STR  : tokenType = MODE_ARG);
        else if (cmdType == LOGTIME_CMD)    _paramCounter == 1 ? tokenType = NICK    : (_paramCounter == 2 ? tokenType = LOG_BEG   : (_paramCounter == 3 ? tokenType = LOG_END  : tokenType = NONE));
        else if (cmdType == WHOIS_CMD)      _paramCounter == 1 ? tokenType = NICK    : tokenType = NONE;
        else if (cmdType == LOCATION_CMD)   _paramCounter == 1 ? tokenType = NICK    : tokenType = NONE;
    }
    return (tokenType);
}

//call it when you are sure from the command syntax
std::pair<std::string, std::vector<std::string> > Commands::getNextParam(OPTION option) {

    static token_type tokenType;
    static bool firstTime = true;
    static std::list<token>::iterator it;
    std::pair<std::string, std::vector<std::string> > paramData("", std::vector<std::string>());

    if (option == RESET) { firstTime = true; return paramData; }
    if (firstTime == true) {
        it = ++_tokensList.begin();
        tokenType = (*it).type;
        firstTime = false;
    }
    if (it == _tokensList.end())
        firstTime = true;

    while (it != _tokensList.end()) {
        if ((*it).type != tokenType && (*it).type != COMMA) { tokenType = (*it).type; break; }
        if ((it)->type != COMMA) paramData.second.push_back((*it).data);
        it++;
    }

    if (!paramData.second.empty()) paramData.first = paramData.second[0];

    return (paramData);
}

void   Commands::tokenize(std::string const& cmdLine) {
    
    token               tokenNode;
	token_type          tokenType(NONE);
    std::string         word;
    size_t              tokenCounter = 0;

    this->_paramCounter = 0;
    for (size_t i = 0; i < cmdLine.length(); i++) {
        /************************************/
        /*			skip spaces				*/
        /************************************/
        while (isspace(cmdLine[i]) == true)
            i++;
        while (i <= cmdLine.length()) {
            /*********************************************************/
            /*  store word to the end of line if tokenType is a MSG  */
            /*********************************************************/
            if (tokenType == COMMENT || tokenType == TOPIC_MSG || tokenType == REASON || tokenType == MSG) {

                word += cmdLine.substr(i); i = cmdLine.length();
                if (word.back() == '\n') word.pop_back();
                if (word.back() == '\r') word.pop_back();
                tokenNode.data = word;
                tokenNode.type = tokenType;
                _tokensList.push_back(tokenNode);
                word.clear();
                break ;
            }

            /*********************************************************/
            /* so here if I encounter spaces or Comma store the word */
            /*********************************************************/
            if (isspace(cmdLine[i]) || cmdLine[i] == ',' || cmdLine[i] == '\0')
            {
                /*********************************************************************************/
                /* store word if it is not empty, it can be empty if this is the first iteration */
                /*********************************************************************************/
                if (!(word.empty())) {
                    // if (tokenType != MODE_STR)
                    //     std::transform(word.begin(), word.end(), word.begin(), ::toupper);
                    tokenCounter == 0 ? tokenType = determine_cmd(word) : 0;
                    tokenNode.data = word;
                    tokenNode.type = tokenType;
                    _tokensList.push_back(tokenNode);
                    word.clear();
                    tokenCounter++;
                }

                if (cmdLine[i] == ',') {

                    tokenType = COMMA;
                    tokenNode.data = cmdLine[i];
                    tokenNode.type = tokenType;
                    _tokensList.push_back(tokenNode);
                }
                else if (isspace(cmdLine[i]))
                    _paramCounter++;

                // determine next token type
				if (isspace(cmdLine[i]) || cmdLine[i] == ',')
                    tokenType = determineToken(cmdLine[i], _tokensList.front().type);

                break ;
            }
            word += cmdLine[i++];
        }
    }
    getNextParam(RESET);
}

bool Commands::isEnoughParam(token_type cmd) {
    
    if (cmd == JOIN_CMD)
        if (_paramCounter < 2) return false;
    return true;
}

void Commands::checkTokensListSyntax()
{
	std::list<token>::iterator ListIt = _tokensList.begin();
    token_type cmd = _tokensList.front().type;
    /**************************************************** THIS LINE WILL BE REMOVED ****************************************************/
    char justFordebug[42][42] = { "NONE", "COMMA ", "PRIVMSG", "JOIN_CMD", "KICK_CMD", "PART_CMD", "TOPIC_CMD", "INVITE_CMD", "MODE_CMD", "LOGTIME_CMD", "WHOIS_CMD", "LOCATION_CMD", "CHANNEL", "KEY", "NICK", "MSG", "TOPIC_MSG", "COMMENT", "MODE_STR", "REASON", "MODE_ARG", "LOG_BEG", "LOG_END"};
    /***********************************************************************************************************************************/
    _tokensList.size() == 0 ? throw std::string("TokenList is empty => cmdLine is empty") : NULL;
    // if (cmd == NONE) sendResponse(fd, ERR_UNKNOWNCOMMAND(currUser->getNickName(), getCommand()) + "\n");
    //ServertoClients(std::string string)
	while (ListIt != _tokensList.end())
	{
        if ((*ListIt).type == CHANNEL)
            if ((*ListIt).data[0] != '#') (*ListIt).data.insert(0, 1, '#');
        
        std::cout << "[" + (*ListIt).data + "]" + " : [" + justFordebug[(*ListIt).type] + "]" << std::endl;
        ListIt++;
	}
}

void Commands::CommandMapinit(cmdData dataCmd)
{
    // std::cout << "command : " << getCommand() << std::endl;
    fd = dataCmd.fd;
    currUser = db->getUser(fd);
    // std::cout << "LINE = " << dataCmd.line << std::endl;
    /***********************************************************************************/
    line = dataCmd.line;
    tokenize(dataCmd.line);
    try {
        checkTokensListSyntax();
    }
    catch(std::string err) {
        std::cout << err << std::endl;
        return ;
    }
    // puts("1");
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

    // std::cout << line << dataCmd.line << std::endl;
    // std::cout << "what" << std::endl;
    // std::string token;
    // std::istringstream iss(dataCmd.line);
    // std::cout << "LIIINE = " << dataCmd.line;
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
    if (cmd == "NICK")
    {}
    else if (cmd == "PASS")
    {}
    else if (cmd == "USER")
    {}
    else if (cmd == "PONG")
    {}
    else if (cmd == "JOIN")
        join();
    else if (cmd == "KICK")
        kick();
    else if (cmd == "LOGTIME")
        logtime();
    else if (cmd == "INVITE")
        invite();
    else if (cmd == "TOPIC")
        topic();
    else if (cmd == "MODE")
        mode();
    else if (cmd == "LOCATION")
        location();
    else if (cmd == "WHOIS")
        whois();
    else if (cmd == "PART")
        part();
    else if (cmd == "PRIVMSG")
        PRIVMSG();
    else
        sendResponse(fd, ERR_UNKNOWNCOMMAND(currUser->getNickName(), cmd) + "\n");
}

void Commands::sendResponse(int userfd, std::string message)
{
    send(userfd, message.c_str(), message.length(), 0);
}

std::string const Commands::getCommand() const
{
    std::string cmd = _tokensList.front().data;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
    return cmd;
}

std::string const Commands::getHostName()
{
    char hostName[256];
    if (gethostname(hostName, sizeof(hostName)) != 0)
        return "";
    return hostName;
}
