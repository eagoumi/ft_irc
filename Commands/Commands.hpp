#ifndef COMMANDS_HPP
#define COMMANDS_HPP

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



//USER ID == FD

typedef struct s_comData{
    std::string line;
    std::string nick;
    int fd;
}cmdData;


class Commands{
private:
    Database db;
    // std::string command;
    std::vector<std::string> command;
    std::vector<std::string>::iterator itV;
    std::string::iterator it;
    // std::multimap<std::string, std::string> channels;
    // std::map<std::string, std::string> channels;
    // std::map<std::string, std::string>::iterator itCh;
    int fd;
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


    void CommandMapinit(cmdData dataCmd);
    // void CommandMapinit(std::string &line);


    std::string getNick();
    std::string getClient() const;
    std::string getCommand() const;
    std::string getChannel();
    std::string get_comment();
    int get_fd();
    std::string get_owner();
    std::string getHostName();
    std::string getKey();

    void sendResponse( std::string msg );



};

#endif

// pass yous
// nick yous
// user s s s s
