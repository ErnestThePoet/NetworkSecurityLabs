#ifndef TRANS_PERFORM_H_
#define TRANS_PERFORM_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../common/sha256.h"
#include "../common/operation_result.h"
#include "../common/packets.h"
#include "../common/read_socket.h"
#include "../common/progress_bar.h"
#include "trans.h"

#define UPLOAD_BUFFER_SIZE (1024 * 1024)
#define DOWNLOAD_BUFFER_SIZE (1024 * 1024)

OperationResult UploadFile(
    FILE *local_file, const size_t file_size, const int server_socket);
OperationResult DownloadFile(
    FILE *local_file, const size_t file_size, const int server_socket);

#endif