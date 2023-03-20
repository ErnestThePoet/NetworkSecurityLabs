#include <stdio.h>
#include <stdbool.h>
#include "create_pac.h"

bool ParseArgs(int argc, char *argv[], int *dest_port_ret)
{
    if (argc != 3)
    {
        return false;
    }

    char *dest_port_end = NULL;
    int dest_port = strtol(argv[2], &dest_port_end, 10);
    if (dest_port_end == argv[2] || dest_port > 65535 || dest_port < 0)
    {
        fputs("Invalid port\n", stderr);
        return false;
    }

    *dest_port_ret = dest_port;

    return true;
}

int main(int argc, char *argv[])
{
    const char *const kUsagePrompt = "Usage: sender <dest-ip> <dest-port>\n";
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