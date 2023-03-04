#include "run_server.h"

void RunServer(const int server_socket)
{
    sockaddr_in client_sockaddr;
    int sockaddr_size = sizeof(sockaddr_in);

    while (true)
    {
        int client_socket = accept(
            server_socket, (struct sockaddr *)&client_sockaddr, &sockaddr_size);
        if (client_socket == -1)
        {
            close(client_socket);
            puts("Client socket creation failed");
            continue;
        }

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, HandleRequest, &client_socket))
        {
            close(client_socket);
            puts("Client thread creation failed");
            continue;
        }

        pthread_detach(client_thread);
    }

    close(server_socket);
}