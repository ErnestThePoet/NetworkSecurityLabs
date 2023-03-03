#include "client.h"

int main(int argc, char *argv[])
{
    const char *const kUsageHelp =
        "Usage:\n"
        "client -u <local file path> <server IPv4 address:server port:server file path>\n"
        "client -d <server IPv4 address:server port:server file path> <local file path>";

    ClientArg client_arg;
    OperationResult operation_result = ParseArgs(argc, argv, &client_arg);
    if (!operation_result.is_successful)
    {
        printf("Argument Error: %s\n", operation_result.error_info);
        puts(kUsageHelp);
        return FAILURE;
    }

    int server_socket = 0;
    operation_result = ConnectToServer(client_arg.server_ip4_address,
                                       client_arg.server_port,
                                       &server_socket);
    if (!operation_result.is_successful)
    {
        printf("Connecting Error: %s\n", operation_result.error_info);
        return FAILURE;
    }

    return SUCCESS;
}

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

OperationResult ConnectToServer(const char *ip4_address, int port, int *server_socket_ret)
{
    OperationResult result = {false};

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        strcpy(result.error_info, "Failed to create socket");
        return result;
    }

    sockaddr_in server_sockaddr;
    in_addr_t server_addr = inet_addr(ip4_address);
    if (server_addr == (in_addr_t)-1)
    {
        close(server_socket);
        strcpy(result.error_info, "Failed to convert server IPv4 address");
        return result;
    }

    server_sockaddr.sin_addr.s_addr = server_addr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);

    if (connect(server_socket,
                (struct sockaddr *)&server_sockaddr,
                sizeof(sockaddr_in)) == -1)
    {
        close(server_socket);
        strcpy(result.error_info, "Failed to connect to server");
        return result;
    }

    *server_socket_ret = server_socket;

    result.is_successful = true;

    return result;
}