#include "recv_udp.h"

void recvUDP(const int server_port)
{
    const int server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket == -1)
    {
        fputs("socket() failed\n", stderr);
        exit(FAILURE);
    }

    sockaddr_in server_sockaddr, client_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(server_port);
    server_sockaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_sockaddr, sizeof(sockaddr_in)) == -1)
    {
        fputs("bind() failed\n", stderr);
        exit(FAILURE);
    }

    printf("Receiver started on port: %d\n", server_port);

    char recv_buf[100];
    int client_sockaddr_length = sizeof(sockaddr_in);
    if (recvfrom(server_socket,
                 recv_buf,
                 sizeof(recv_buf),
                 0,
                 (struct sockaddr *)&client_sockaddr,
                 &client_sockaddr_length) == -1)
    {
        fputs("recvfrom() failed\n", stderr);
        exit(FAILURE);
    }

    printf("Received from %s:%d\nMessage:\n%s\n",
           inet_ntoa(client_sockaddr.sin_addr),
           ntohs(client_sockaddr.sin_port),
           recv_buf);

    close(server_socket);
}