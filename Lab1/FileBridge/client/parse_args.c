#include "parse_args.h"

OperationResult ParseArgs(int argc, char *argv[], ClientArg *client_arg_ret)
{
    OperationResult result = {false};

    if (argc != 4)
    {
        strcpy(result.error_info, "Invalid number of input arguments");
        return result;
    }

    if (!strcmp(argv[1], "-u"))
    {
        client_arg_ret->operation_mode = OPERATION_MODE_UPLOAD;
    }
    else if (!strcmp(argv[1], "-d"))
    {
        client_arg_ret->operation_mode = OPERATION_MODE_DOWNLOAD;
    }
    else
    {
        strcpy(result.error_info, "Invalid operation mode specifier");
        return result;
    }

    client_arg_ret->client_path =
        client_arg_ret->operation_mode == OPERATION_MODE_UPLOAD ? argv[2] : argv[3];

    char *server_arg =
        client_arg_ret->operation_mode == OPERATION_MODE_UPLOAD ? argv[3] : argv[2];

    const char *const kServerArgDelim = ":";

    char *server_arg_part = strtok(server_arg, kServerArgDelim);

    // IPv4 address format is not checked
    client_arg_ret->server_ip4_address = server_arg_part;

    server_arg_part = strtok(NULL, kServerArgDelim);
    if (server_arg_part == NULL)
    {
        strcpy(result.error_info, "Server port not found");
        return result;
    }

    char *port_end = NULL;
    int server_port = strtol(server_arg_part, &port_end, 10);
    if (port_end == server_arg_part || server_port > 65535 || server_port < 0)
    {
        strcpy(result.error_info, "Invalid server port");
        return result;
    }

    client_arg_ret->server_port = server_port;

    server_arg_part = strtok(NULL, kServerArgDelim);
    if (server_arg_part == NULL)
    {
        strcpy(result.error_info, "Server file path not found");
        return result;
    }

    client_arg_ret->server_path = server_arg_part;

    result.is_successful = true;
    return result;
}
