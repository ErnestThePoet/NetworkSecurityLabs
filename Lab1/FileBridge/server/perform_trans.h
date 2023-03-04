#ifndef PERFORM_TRANS_H_
#define PERFORM_TRANS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../common/sha256.h"
#include "../common/operation_result.h"
#include "../common/packets.h"
#include "../common/read_socket.h"
#include "../common/trans.h"

#define RECEIVE_BUFFER_SIZE (1024 * 1024)
#define SEND_BUFFER_SIZE (1024 * 1024)

OperationResult ReceiveFile(
    FILE *server_file, const size_t file_size, const int client_socket);
OperationResult SendFile(
    FILE *server_file, const size_t file_size, const int client_socket);

#endif