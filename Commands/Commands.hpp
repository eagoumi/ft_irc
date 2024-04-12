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

typedef std::pair<std::string, std::vector<std::string> > PARAM_PAIR;

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
    unsigned long		fd;
    Database			*db;
    Logger&				_logger;
    std::string			newTopic;
    std::string			topicMsg;
    User*				currUser;
    std::string			invitedNick;
    std::list<token>	_tokensList;
    Channel				*currChannel;
    size_t				_paramCounter;

    void				join();
    void				kick();
    void				mode();
    void				part();
    void				topic();
    void				whois();
    void				invite();
    void				logtime();
    void				PRIVMSG();
    void				location();
    std::string			get42Token();
    void				WelcomeClient();
    const std::string	getCommand() const;
    bool				checkTokensListSyntax();
    void				tokenize(std::string const&);
    bool				check_connection(size_t user_fd);
    PARAM_PAIR       	getNextParam(OPTION option = NADA);
    bool				isEnoughParam(token_type const& cmd);
    void				sendResponse(int userfd, std::string msg);
    token_type			determineToken(char sep, token_type cmdType);
    void				Authentication(std::string const& serverPass);
    void				sendToClientsExisted(size_t reciver, User *sender, std::string Message);
    void				SendMessageToMembers(Channel *Channel_name, User *user_fds, std::string command);
    void				PrintLogsAfterJoined(std::string ClientChannel, User& Client, Channel& Channel);

public:
    Commands();
    ~Commands();
    Commands(const Commands &obj);
    Commands& operator=(const Commands &obj);

    void	executeCommand(cmdData dataCmd);
};

#endif
