#include "parse_args.h"

bool ParseArgs(const int argc, const char *argv[], int *port_ret)
{
    if (argc != 3)
    {
        return false;
    }

    char *port_end = NULL;
    int port = strtol(argv[2], &port_end, 10);
    if (port_end == argv[2] || port > 65535 || port < 0)
    {
        fputs("Invalid port\n", stderr);
        return false;
    }

    *port_ret = port;

    return true;
}