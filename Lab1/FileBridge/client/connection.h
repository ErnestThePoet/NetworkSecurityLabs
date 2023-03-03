#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../common/operation_result.h"

typedef struct sockaddr_in sockaddr_in;

OperationResult ConnectToServer(const char *ip4_address, int port, int *server_socket_ret);
void CloseConnection(int server_socket);

#endif