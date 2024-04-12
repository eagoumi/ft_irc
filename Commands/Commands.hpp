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
#include "../Logger/logger.hpp"
#include "../error_request.hpp"

enum OPTION
{
    NADA,
    RESET
};

enum token_type { 

    NONE, COMMA , PONG_CMD, PASS_CMD, NICK_CMD, USER_CMD, 
    PRIVMSG_CMD, JOIN_CMD, KICK_CMD, PART_CMD, 
    TOPIC_CMD, INVITE_CMD, MODE_CMD, LOGTIME_CMD, 
    WHOIS_CMD, LOCATION_CMD, CHANNEL, KEY, NICK, MSG, 
    TOPIC_MSG, COMMENT, MODE_STR, REASON, MODE_ARG, LOG_BEG, 
    LOG_END, PASS, USER, TARGET
};
struct token
{
    token_type type;
    std::string data;
};

class Database;

typedef struct s_comData
{
    int         fd;
    std::string line;
    std::string serverPass;

} cmdData;

class Commands
{
private:
    Database            *db;
    User                *currUser;
    Channel             *currChannel;
    std::string         newTopic;
    std::string         invitedNick;
    Logger&             _logger;
    std::list<token>    _tokensList;
    size_t              _paramCounter;
    unsigned long       fd;
    std::string         topicMsg;
    std::string         line;

    void                                                tokenize(std::string const&);
    bool                                                isEnoughParam(token_type const& cmd);
    bool                                                checkTokensListSyntax();
    token_type                                          determineToken(char sep, token_type cmdType);
    std::string                                         get42Token();
    std::pair<std::string, std::vector<std::string> >   getNextParam(OPTION option = NADA);

public:
    Commands();
    ~Commands();
    Commands(const Commands &obj);
    Commands&           operator=(const Commands &obj);

    void                executeCommand(cmdData dataCmd);
    void                join();
    void                kick();
    void                invite();
    void                topic();
    void                mode();
    void                part();
    void                logtime();
    void                location();
    void                whois();
    void                PRIVMSG();
    void                Authentication(std::string const& serverPass);
    void                WelcomeClient();
    void                sendResponse(int userfd, std::string msg);
    void                SendMessageToMembers(Channel *Channel_name, User *user_fds, std::string command);
    void                sendToClientsExisted(size_t reciver, User *sender, std::string Message);
    bool                check_connection(size_t user_fd);
    void                PrintLogsAfterJoined(std::string ClientChannel, User& Client, Channel& Channel);
    const std::string   getCommand() const;

    
 

};

#endif
