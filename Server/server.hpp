#ifndef __SERVER__HPP
#define __SERVER__HPP

# include <map>
# include <iostream>
# include "../Database/database.hpp"

class Server
{
    private:
        Database *database;

    public:
        Server(const std::string &port, const std::string &password);
        ~Server();
};

#endif

//add to server `Command` Class with `execute` method