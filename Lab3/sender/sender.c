#include <stdio.h>
#include "../common/parse_args.h"
#include "create_pac.h"

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