#include "./Channels/channel.hpp"
#include "./Users/user.hpp"
#include "Database/database.hpp"
#include <cstddef>
#include <sys/signal.h>
#include <list>
#include <utility>
#include <vector>
# include <iostream>

# define NOT_FOUND NULL

/* Compile using c++ cmdTokenizeTest.cpp Database/database.cpp Channels/channel.cpp Users/user.cpp Commands/Commands.cpp */

enum type { NONE, COMMA , JOIN_CMD, KICK_CMD, TOPIC_CMD, INVITE_CMD, MODE_CMD, CHANNEL, KEY, NICK, TOPIC_MSG, COMMENT, MODE_STR, MODE_ARG };
struct token
{
	type		type;
	std::string	data;
};

/* just for debug */
char arr[14][20] = {
    "NONE",
    "COMMA",
    "JOIN_CMD",
    "KICK_CMD",
    "TOPIC_CMD",
    "INVITE_CMD",
    "MODE_CMD",
    "CHANNEL",
    "KEY",
    "NICK",
    "TOPIC_MSG",
    "COMMENT",
    "MODE_STR",
    "MODE_ARG"
};


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

std::list<token> tokenize(std::string const& cmdLine) {

    /* token types: CMD - CHANNEL - KEY - USER - TOPIC - COMMENT - MODE - MODEARG */
    std::list<token>    tokensList;
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
					tokensList.push_back(tokenNode);
					word.clear();
                    tokenCounter++;
				}

                if (cmdLine[i] == ',')
				{
					tokenType = COMMA;
					tokenNode.data = cmdLine[i];
					tokenNode.type = tokenType;
					tokensList.push_back(tokenNode);
				}
                else if (isspace(cmdLine[i]))
                    parameterCounter++;

                // determine next token type
				if (isspace(cmdLine[i]) || cmdLine[i] == ',')
                    tokenType = determineToken(cmdLine[i], tokensList.front().type, parameterCounter);
                break ;
            }
            word += cmdLine[i++];
            // std::cout << word << " " << arr[tokenType] << std::endl;
        }
    }
    return tokensList;
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

int main(int argc, char **argv) {

    std::list<token> tokensList = tokenize(argv[1]);

    getNextParam(tokensList);

    //JOIN
    std::vector<std::string> channelsNames = getNextParam(tokensList);
    
    std::cout << "[\t";
    for (int i = 0; i < channelsNames.size(); i++)
        std::cout << channelsNames[i] << "\t";
    std::cout << "]\n";
    
    std::vector<std::string> channelsKeys = getNextParam(tokensList); 
    std::cout << "[\t";
    for (int i = 0; i < channelsKeys.size(); i++)
        std::cout << channelsKeys[i] << "\t";
    std::cout << "]\n";





    // while (true) {
    //     std::vector<std::string> param = getNextParam(tokensList);
    //     if (param.size() == 0)
    //         break ;
    //     std::cout << "[\t";
    //     for (int i = 0; i < param.size(); i++)
    //         std::cout << param[i] << "\t";
    //     std::cout << "]\n";
    // }

    



    // std::list<token>::iterator it = tokensList.begin();
    // while (it != tokensList.end())
	// {
    //     std::cout << "[\n\t" << (*it).data << "\n\t" << arr[(*it).type] << "\n]\n\n";
    //     // if ((*it).type == CHANNEL)

	// 	it++;
	// }


    return (0);
}