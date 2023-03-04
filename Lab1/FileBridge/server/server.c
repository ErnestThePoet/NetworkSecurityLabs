#include "server.h"

int main(int argc, char *argv[])
{
    const char *const kUsageHelp =
        "Usage:\n"
        "server <port>";

    int port = 0;
    OperationResult operation_result = ParseArgs(argc, argv, &port);
    if (!operation_result.is_successful)
    {
        printf("Argument Error: %s\n", operation_result.error_info);
        puts(kUsageHelp);
        return FAILURE;
    }

    int server_socket = 0;
    operation_result = CreateServer(port, &server_socket);
    if (!operation_result.is_successful)
    {
        printf("Failed to create server: %s\n", operation_result.error_info);
        return FAILURE;
    }

    printf("Server started, listening on port %d\n", port);

    RunServer(server_socket);

    return SUCCESS;
}
