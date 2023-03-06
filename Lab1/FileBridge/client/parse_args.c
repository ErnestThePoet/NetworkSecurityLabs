#include "parse_args.h"

OperationResult ParseArgs(
    const int argc, char *argv[], ClientArg *client_arg_ret)
{
    OperationResult result = {false};

    const char *kInvalidArgcErrorInfo = "Invalid number of input arguments";

    if (argc < 3)
    {
        strcpy(result.error_info, kInvalidArgcErrorInfo);
        return result;
    }

    if (!strcmp(argv[1], "-u"))
    {
        if (argc != 4)
        {
            strcpy(result.error_info, kInvalidArgcErrorInfo);
            return result;
        }
        client_arg_ret->operation_mode = OPERATION_MODE_UPLOAD;
    }
    else if (!strcmp(argv[1], "-l"))
    {
        if (argc != 3)
        {
            strcpy(result.error_info, kInvalidArgcErrorInfo);
            return result;
        }
        client_arg_ret->operation_mode = OPERATION_MODE_LIST_DIR;
    }
    else if (!strcmp(argv[1], "-d"))
    {
        if (argc != 4)
        {
            strcpy(result.error_info, kInvalidArgcErrorInfo);
            return result;
        }
        client_arg_ret->operation_mode = OPERATION_MODE_DOWNLOAD;
    }
    else
    {
        strcpy(result.error_info, "Invalid operation mode specifier");
        return result;
    }

    char *server_arg = NULL;

    switch (client_arg_ret->operation_mode)
    {
    case OPERATION_MODE_UPLOAD:
        client_arg_ret->local_file_path = argv[2];
        server_arg = argv[3];
        break;
    case OPERATION_MODE_DOWNLOAD:
        client_arg_ret->local_file_path = argv[3];
        server_arg = argv[2];
        break;
    case OPERATION_MODE_LIST_DIR:
        client_arg_ret->local_file_path = NULL;
        server_arg = argv[2];
        break;
    }

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

    client_arg_ret->server_file_path = server_arg_part;

    result.is_successful = true;
    return result;
}
