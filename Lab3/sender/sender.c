#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../common/defs.h"
#include "create_pac.h"

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

int main(int argc, char *argv[])
{
    const char *const kUsagePrompt = "Usage: sender <dest-ip4> <dest-port>\n";
    const char kUdpPayload[] = "HELLO WORLD from Ernest Cui";

    int dest_port = 0;
    if (!ParseArgs(argc, argv, &dest_port))
    {
        fputs(kUsagePrompt, stderr);
        return FAILURE;
    }

    createPac(argv[1], dest_port, kUdpPayload);

    return SUCCESS;
}