#ifndef __SERVER__HPP
#define __SERVER__HPP

#include <iostream>

class Server
{
    private:

    public:
        Server(const std::string &port, const std::string &password);
        ~Server();
};

#endif