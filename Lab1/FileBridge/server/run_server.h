#ifndef RUN_SERVER_H_
#define RUN_SERVER_H_

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../common/file_bridge.h"
#include "../common/operation_result.h"
#include "handle_request.h"

void RunServer(const int server_socket);

#endif