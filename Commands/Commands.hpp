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
#include <sys/socket.h>
#include "../Database/database.hpp"
#include "../Channels/channel.hpp"
#include "../Users/user.hpp"
#include "../error_request.hpp"

enum reset { NANDA, RESET };

enum token_type { NONE, COMMA , JOIN_CMD, KICK_CMD, TOPIC_CMD, INVITE_CMD, MODE_CMD, LOGTIME_CMD, CHANNEL, KEY, NICK, TOPIC_MSG, COMMENT, MODE_STR, MODE_ARG };
struct token
{
	token_type		type;
	std::string	    data;
};

// :yousra!~a@127.0.0.1 JOIN #HD
// :Pentagone.chat MODE #HD +t
// :Pentagone.chat 353 yousra @ #HD : @yousra
// :Pentagone.chat 366 yousra #HD :End of /NAMES list.
// :Pentagone.chat 332 yousra #HD :TOPIC Not set

typedef struct s_comData{
    std::string line;
    std::string nick;
    int fd;

}cmdData;


class Commands{
private:
    static std::string newTopic;

    Database* db;

    int flag;
    std::list<token> _tokensList;
    void tokenize(std::string const&);
    std::vector<std::string> getNextParam(reset option = NANDA);
    // std::string command;
    std::vector<std::string> command;
    std::vector<std::string>::iterator itV;
    std::string::iterator it;
    // std::multimap<std::string, std::string> channels;
    // std::map<std::string, std::string> channels;
    // std::map<std::string, std::string>::iterator itCh;
    unsigned long fd;
    User *currUser;
    std::string owner;
    std::string topicMsg;

    std::pair<
        std::multimap<std::string, std::string>::iterator,
        std::multimap<std::string, std::string>::iterator>
        range;

    std::string line;
    std::string client;


public:
    Commands();
    ~Commands();
    Commands(const Commands& obj);
    Commands& operator=(const Commands& obj);

    void parsCommands(std::string str, std::string owner, std::string host, int fd);

    std::string ft_trim(std::string str);

    void ft_split(std::string str);

    void kick();

    int searchChannel();

    void fillMultimap();

    void invite();

    void topic();

    void mode();

    void join();

    void logtime();

    std::map<std::string, std::string> splitInput(std::string input);

    bool existMemberChannel(std::string member);
    bool existOperatorChannel(std::string nick);
    size_t existUser(std::string nick);

    void CommandMapinit(cmdData dataCmd);
    // void CommandMapinit(std::string &line);


    std::string getNick();
    std::string getClient() const;
    std::string getCommand() const;
    std::string getChannel();
    std::string getCommentTopic();
    std::string getHostName();
    void seTopic(std::string newTopic);
    // std::string geTopic();

    void sendResponse(int userfd, std::string msg);

    void displayMember();

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