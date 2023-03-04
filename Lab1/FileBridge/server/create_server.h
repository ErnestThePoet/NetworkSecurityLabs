#ifndef CREATE_SERVER_H_
#define CREATE_SERVER_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../common/file_bridge.h"
#include "../common/operation_result.h"

OperationResult CreateServer(int port, int *server_socket_ret);

#endif