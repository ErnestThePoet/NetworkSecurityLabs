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

    return A345;
}


OperationResult PrepareFileUpload(const char *local_file_path, FILE **file_ret){
    OperationResult result = {false};

    FILE *file = fopen(local_file_path, "rb");
    if(file==NULL){
        strcpy(result.error_info, "Failed to open local file");
        return result;
    }

    *file_ret = file;

    result.is_successful = true;
    return result;
}

OperationResult PrepareFileDownload(const char *local_file_path, FILE **file_ret){
    OperationResult result = {false};

    FILE *file = fopen(local_file_path, "wb");
    if (file == NULL)
    {
        strcpy(result.error_info, "Failed to open local file");
        return result;
    }

    *file_ret = file;

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