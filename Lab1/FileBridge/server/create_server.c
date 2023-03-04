#include "create_server.h"

OperationResult CreateServer(int port,int *server_socket_ret){
    OperationResult result = {false};
    
    const int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        strcpy(result.error_info, "Failed to create server socket");
        return result;
    }

    sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_addr.s_addr = INADDR_ANY;
    server_sockaddr.sin_port = htons(port);

    if (bind(server_socket,
             (struct sockaddr *)&server_sockaddr,
             sizeof(sockaddr_in))==-1){
        close(server_socket);
        sprintf(result.error_info, "Failed to bind to port %d", port);
        return result;
    }

    if(listen(server_socket,SOMAXCONN)==-1){
        close(server_socket);
        strcpy(result.error_info, "Server socket listen() failed");
        return result;
    }

    *server_socket_ret = server_socket;

    result.is_successful = true;
    return result;
}