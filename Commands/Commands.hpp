#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <list>
#include <iostream>
#include <vector>
#include <cctype>
#include <map>
#include <sstream>
#include <utility>
#include <algorithm>
#include <unistd.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include "../Database/database.hpp"
#include "../Channels/channel.hpp"
#include "../Server/server.hpp"
#include "../Users/user.hpp"
#include "../error_request.hpp"

enum reset
{
    NANDA,
    RESET
};

enum token_type { NONE, COMMA , JOIN_CMD, KICK_CMD, PART_CMD, TOPIC_CMD, INVITE_CMD, MODE_CMD, LOGTIME_CMD, CHANNEL, KEY, NICK, TOPIC_MSG, COMMENT, MODE_STR, REASON, MODE_ARG, LOG_BEG, LOG_END};
struct token
{
    token_type type;
    std::string data;
};

// :yousra!~a@127.0.0.1 JOIN #HD
// :Pentagone.chat MODE #HD +t
// :Pentagone.chat 353 yousra @ #HD : @yousra
// :Pentagone.chat 366 yousra #HD :End of /NAMES list.
// :Pentagone.chat 332 yousra #HD :TOPIC Not set

class Database;

typedef struct s_comData
{
    std::string line;
    std::string nick;
    int fd;

} cmdData;

class Commands
{
private:
    Channel *currChannel;
    // std::map<std::string, bool> modes;
    static std::string newTopic;
    static std::string invitedNick;
    std::string modeStr;
    Database *db;

    int flag;
    std::list<token> _tokensList;
    size_t _paramCounter;
    void tokenize(std::string const&);
    std::pair<std::string, std::vector<std::string> > getNextParam(reset option = NANDA);
    void checkTokensListSyntax();
    token_type  determineToken(char sep, token_type cmdType);
    std::string get42Token();
    // std::string command;
    std::vector<std::string> command;
    std::vector<std::string>::iterator itV;
    unsigned long fd;
    User *currUser;
    std::string owner;
    std::string topicMsg;
    std::string line;

public:
    Commands();
    ~Commands();
    Commands(const Commands &obj);
    Commands &operator=(const Commands &obj);

    void CommandMapinit(cmdData dataCmd);

    void kick();
    void invite();
    void topic();
    void mode();
    void join();
    void logtime();

    void part();

    std::map<std::string, std::string> splitInput(std::string input);
    bool existMemberChannel(std::string member);
    bool existOperatorChannel(std::string nick);
    size_t existUser(std::string nick);


    std::string getNick();
    std::string getCommand() const;
    std::string getChannel();
    std::string getTopic();
    std::string getComment();
    std::string getHostName();
    std::string getModeString();

    // bool gettingModes(char toFind, std::string mode, std::map<std::string, bool> &modSeted);
    // void seTopic(std::string newTopic);
    void    sendResponse(int userfd, std::string msg);
    void    displayMember();
    bool    getMode(std::string letter);
    size_t  getLimitArg();


};

#endif

// pass yous
// nick yous
// user s s s s

// INVITE <nickname> <channel>
// JOIN <channel>{,<channel>} [<key>{,<key>}]
// KICK <channel> <user> *( "," <user> ) [<comment>]
// TOPIC <channel> [<topic>]
// MODE <target> [<modestring> [<mode arguments>...]]

// topic #ch
// topic #ch :
// topic #ch :channel's topic
