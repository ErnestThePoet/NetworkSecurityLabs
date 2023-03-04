#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../common/file_bridge.h"
#include "../common/operation_result.h"

OperationResult ConnectToServer(
    const char *ip4_address, const int port, int *server_socket_ret);
void CloseConnection(const int server_socket);

#endif