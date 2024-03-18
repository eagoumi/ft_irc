#include "server.hpp"
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
    Server sr(std::atoi(av[1]), av[2]);
    sr.ServerStarting();
}