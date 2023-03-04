#include "run_server.h"

void RunServer(const int server_socket){
    sockaddr_in client_sockaddr;
    int sockaddr_size = sizeof(sockaddr_in);

    while (true)
    {
        const int client_socket = accept(
            server_socket, (struct sockaddr *)&client_sockaddr, &sockaddr_size);
        if (client_socket == -1)
        {
            puts("Client socket creation failed");
            continue;
        }

        
    }

    close(server_socket);
}