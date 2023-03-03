#include "connection.h"

OperationResult ConnectToServer(
    const char *ip4_address, const int port, int *server_socket_ret)
{
    OperationResult result = {false};

    const int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        strcpy(result.error_info, "Failed to create socket");
        return result;
    }

    sockaddr_in server_sockaddr;
    const in_addr_t server_addr = inet_addr(ip4_address);
    if (server_addr == (in_addr_t)-1)
    {
        close(server_socket);
        strcpy(result.error_info, "Failed to convert server IPv4 address: ");
        strcat(result.error_info, ip4_address);
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
        sprintf(
            result.error_info, "Failed to connect to server %s:%d", ip4_address, port);
        return result;
    }

    *server_socket_ret = server_socket;

    result.is_successful = true;
    return result;
}

void CloseConnection(const int server_socket)
{
    close(server_socket);
}