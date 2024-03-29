#include <stdio.h>
#include <stdbool.h>
#include "../common/defs.h"
#include "recv_udp.h"

bool ParseArgs(int argc, char *argv[], int *port_ret)
{
    if (argc != 2)
    {
        return false;
    }

    char *port_end = NULL;
    int port = strtol(argv[1], &port_end, 10);
    if (port_end == argv[1] || port > 65535 || port < 0)
    {
        fputs("Invalid port\n", stderr);
        return false;
    }

    *port_ret = port;

    return true;
}

int main(int argc, char *argv[])
{
    const char *const kUsagePrompt = "Usage: receiver <server-port>\n";

    int server_port = 0;
    if (!ParseArgs(argc, argv, &server_port))
    {
        fputs(kUsagePrompt, stderr);
        return FAILURE;
    }

    recvUDP(server_port);

    return SUCCESS;
}