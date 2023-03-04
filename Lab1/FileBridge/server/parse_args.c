#include "parse_args.h"

OperationResult ParseArgs(
    const int argc, char *argv[], int *port_ret)
{
    OperationResult result = {false};

    if (argc != 2)
    {
        strcpy(result.error_info, "Invalid number of input arguments");
        return result;
    }

    char *port_end = NULL;
    int port = strtol(argv[1], &port_end, 10);
    if (port_end == argv[1] || port > 65535 || port < 0)
    {
        strcpy(result.error_info, "Invalid port");
        return result;
    }

    *port_ret = port;

    result.is_successful = true;
    return result;
}
