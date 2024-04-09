#include "Server/server.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

int main(int ac, char **av)
{
    try{
        if (ac != 3)
        {
            std::cout << "Too few Arguments" << std::endl;
            return 1;
        }
        signal(SIGPIPE, SIG_IGN);
        // signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        size_t port = std::atoi(av[1]);
        if (port < 1024 || port > 65535)
        {
            std::cerr << "You are Not On the Right RAnge Of Allowed Port" << std::endl;
            return 1;
        }
        Server sr(port, av[2]);
        sr.ServerStarting();
    }
    catch(...)
    {
        std::cout << "Error" << std::endl;
    }
}