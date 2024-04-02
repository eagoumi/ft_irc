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
#include "../error_request.hpp"

enum OPTION
{
    NANDA,
    RESET
};

enum token_type { NONE, COMMA , PRIVMSG_CMD, JOIN_CMD, KICK_CMD, PART_CMD, TOPIC_CMD, INVITE_CMD, MODE_CMD, LOGTIME_CMD, WHOIS_CMD, LOCATION_CMD, CHANNEL, KEY, NICK, MSG, TOPIC_MSG, COMMENT, MODE_STR, REASON, MODE_ARG, LOG_BEG, LOG_END};
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
    // std::string channelName;
    // std::string nickName;
    // std::vector<std::string> channelNamesList;

    int flag;
    std::list<token> _tokensList;
    size_t _paramCounter;
    void tokenize(std::string const&);
    std::pair<std::string, std::vector<std::string> > getNextParam(OPTION option = NANDA);
    void checkTokensListSyntax();
    token_type  determineToken(char sep, token_type cmdType);
    std::string get42Token();
    // std::string command;
    // std::vector<std::string> command;
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
    void location();
    void whois();

    void part();

    std::map<std::string, std::string> splitInput(std::string input);
    // bool existMemberChannel(std::string member, std::string channelName);
    // bool existOperatorChannel(std::string nick, std::string channelName);
    // size_t existUser(std::string nick);


    std::string const getCommand() const;
    std::string const getHostName();
    // std::string getNick();
    // std::string getChannel();
    // std::string getTopic();
    // std::string getComment();
    // std::string getModeString();

    // bool gettingModes(char toFind, std::string mode, std::map<std::string, bool> &modSeted);
    // void seTopic(std::string newTopic);
    void    sendResponse(int userfd, std::string msg);
    void    displayMember(std::string channelName);
    bool    getMode(std::string letter, std::string channelName);
    void SendMessageToMembers(Channel *Channel_name, User *user_fds, std::string command);


};

#endif


// JOIN <channel>{,<channel>} [<key>{,<key>}]
// part

// INVITE <nickname> <channel>
// KICK <channel> <user> *( "," <user> ) [<comment>]

// TOPIC <channel> [<topic>]
// MODE <channel> [<modestring> [<mode arguments>...]]


