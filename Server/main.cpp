#include "server.hpp"

int main(int ac, char **av)
{
    Server sr(std::atoi(av[1]), av[2]);
    sr.ServerStarting();
}