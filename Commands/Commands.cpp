#include "./Commands.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <vector>

Commands::Commands() : _logger(Logger::GetInstance())
{
    db = Database::GetInstance();
}

Commands::~Commands()
{
}

Commands::Commands(const Commands &obj) : _logger(Logger::GetInstance())
{
    *this = obj;
}

Commands &Commands::operator=(const Commands &obj)
{
    (void)obj;
    // complete this
    return *this;
}

token_type determine_cmd(std::string token) {

    std::transform(token.begin(), token.end(), token.begin(), ::toupper);
    if (token == "JOIN")
        return JOIN_CMD;
    else if (token == "PRIVMSG")
        return PRIVMSG_CMD;
    else if (token == "PASS")
        return PASS_CMD;
    else if (token == "NICK")
        return NICK_CMD;
    else if (token == "USER")
        return USER_CMD;
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

    /* Command:             JOIN            Parameters: <channel>{,<channel>}           [<key>{,<key>}]                         */
    /* Command:             MODE            Parameters: <target>                        [<modestring> [<mode arguments>...]]    */
    /* Command:             PART            Parameters: <channel>{,<channel>}           [<reason>]                              */
    /* Command:             KICK            Parameters: <channel>                       <user> *( "," <user> ) [<comment>]      */
    /* Command:             PRIVMSG         Parameters: <target>{,<target>}             <text to be sent>                       */
    /* Command:             TOPIC           Parameters: <channel>                       [<topic>]                               */
    /* Command:             INVITE          Parameters: <nickname>                      <channel>                               */

    token_type tokenType(NONE);
    if (sep == ',') {
        if      (cmdType == JOIN_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : tokenType = NONE;
        else if (cmdType == PART_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = KEY  : tokenType = NONE);
        else if (cmdType == KICK_CMD)       _paramCounter == 1 ? tokenType = NONE    : (_paramCounter == 2 ? tokenType = NICK : tokenType = NONE);
        else if (cmdType == PRIVMSG_CMD)    _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = KEY  : tokenType = NONE);
        else if (cmdType == MODE_CMD)       tokenType = NONE;
        else if (cmdType == TOPIC_CMD)      tokenType = NONE;
        else if (cmdType == INVITE_CMD)     tokenType = NONE;
        else if (cmdType == LOGTIME_CMD)    tokenType = NONE;
    }
    else if (isspace(sep)) {
        if      (cmdType == JOIN_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = KEY       : tokenType = NONE);
        else if (cmdType == MODE_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = MODE_STR  : tokenType = MODE_ARG);
        else if (cmdType == PART_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = REASON    : tokenType = NONE);
        else if (cmdType == KICK_CMD)       _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = NICK      : (_paramCounter == 3 ? tokenType = COMMENT  : tokenType = NONE));
        else if (cmdType == TOPIC_CMD)      _paramCounter == 1 ? tokenType = CHANNEL : (_paramCounter == 2 ? tokenType = TOPIC_MSG : tokenType = NONE);
        else if (cmdType == INVITE_CMD)     _paramCounter == 1 ? tokenType = NICK    : (_paramCounter == 2 ? tokenType = CHANNEL   : tokenType = NONE);
        else if (cmdType == PRIVMSG_CMD)    _paramCounter == 1 ? tokenType = NICK    : (_paramCounter == 2 ? tokenType = MSG       : tokenType = NONE);
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
    _tokensList.clear();
    getNextParam(RESET);
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
                
                size_t spacePos = cmdLine.find(' ', i);
                cmdLine[i] == ':' ? word += cmdLine.substr(i + 1) : word += cmdLine.substr(i, spacePos - i);
                tokenNode.data = word;
                tokenNode.type = tokenType;
                _tokensList.push_back(tokenNode);
                word.clear();
                return ;
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
                else if (isspace(cmdLine[i]) || cmdLine[i] == '\0')
                    _paramCounter++;

                // determine next token type
				if (isspace(cmdLine[i]) || cmdLine[i] == ',')
                    tokenType = determineToken(cmdLine[i], _tokensList.front().type);

                break ;
            }
            word += cmdLine[i++];
        }
    }
    
}

bool Commands::isEnoughParam(token_type const& cmd) {

    if      (cmd == JOIN_CMD     && _paramCounter < 2) return false;
    else if (cmd == MODE_CMD     && _paramCounter < 2) return false;
    else if (cmd == PART_CMD     && _paramCounter < 2) return false;
    else if (cmd == KICK_CMD     && _paramCounter < 3) return false;
    else if (cmd == TOPIC_CMD    && _paramCounter < 2) return false;
    else if (cmd == WHOIS_CMD    && _paramCounter < 2) return false;
    else if (cmd == INVITE_CMD   && _paramCounter < 3) return false;
    else if (cmd == PRIVMSG_CMD  && _paramCounter < 2) return false;
    else if (cmd == LOGTIME_CMD  && _paramCounter < 2) return false;
    else if (cmd == LOCATION_CMD && _paramCounter < 2) return false;

    return true;
}

bool Commands::checkTokensListSyntax()
{
    token_type cmd = _tokensList.front().type;
	std::list<token>::iterator ListIt = _tokensList.begin();

    /**************************************************** -DEBUG- THOSE LINES WILL BE REMOVED -DEBUG- ****************************************************/
    char justFordebug[42][42] = { "NONE", "COMMA", "PASS_CMD", "NICK_CMD", "USER_CMD", "PRIVMSG_CMD", "JOIN_CMD", "KICK_CMD", "PART_CMD", "TOPIC_CMD", "INVITE_CMD", "MODE_CMD", "LOGTIME_CMD", "WHOIS_CMD", "LOCATION_CMD", "CHANNEL", "KEY", "NICK", "MSG", "TOPIC_MSG", "COMMENT", "MODE_STR", "REASON", "MODE_ARG", "LOG_BEG", "LOG_END", "PASS", "USER"};
    std::cout << "▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒" << std::endl;
    std::cout << "▒▒▒▒▒▒▒▒▒▒▒▒▒▒CMD TOKEN LIST▒▒▒▒▒▒▒▒▒▒▒▒▒▒" << std::endl;
    std::cout << "▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒" << std::endl;
    for (std::list<token>::iterator it = _tokensList.begin(); it != _tokensList.end(); it++)
        std::cout << "▒ " << "[" + (*it).data + "]" + " : [" + justFordebug[(*it).type] + "]" << std::endl;
    std::cout << "▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒" << std::endl << std::endl;
    /***********************************************************************************************************************************/
    
    if (_tokensList.size() == 0) { return false; }
    if (cmd == NONE) { _logger.ServertoClient(ERR_UNKNOWNCOMMAND(currUser->getNickName(), getCommand()) + "\n"); return false; }
    if (isEnoughParam(cmd) == false) { _logger.ServertoClient(ERR_NEEDMOREPARAMS(currUser->getNickName()) + "\n"); return false; }

	while (ListIt != _tokensList.end())
	{
        if ((*ListIt).type == CHANNEL)
            if ((*ListIt).data[0] != '#') (*ListIt).data.insert(0, 1, '#');
        ListIt++;
	}
    return true;
}

void Commands::CommandMapinit(cmdData dataCmd)
{
    fd = dataCmd.fd;
    currUser = db->getUser(fd);
    line = dataCmd.line;

    tokenize(dataCmd.line);
    if (checkTokensListSyntax() == false)
        return ;

    std::string cmd = getCommand();
    if ((cmd == "PASS" || cmd == "NICK" || cmd == "USER") && currUser->isAuthenticated() == false)
        Authentication(dataCmd.serverPass);
    // else if (cmd == "PASS")
    // {}
    // else if (cmd == "USER")
    // {}
    // else if (cmd == "PONG")
    // {}
    if (currUser->isAuthenticated() == true)
    {
        if (cmd == "JOIN")
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
    }
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
