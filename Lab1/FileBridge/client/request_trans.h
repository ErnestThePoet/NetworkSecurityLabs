#ifndef REQUEST_TRANS_H_
#define REQUEST_TRANS_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "../common/operation_result.h"
#include "../common/packets.h"
#include "../common/read_socket.h"

OperationResult RequestFileUpload(
    const size_t file_size,
    const char *server_file_path,
    const int server_socket);
OperationResult RequestFileDownload(
    const char *server_file_path,
    const int server_socket,
    size_t *file_size_ret);

#endif