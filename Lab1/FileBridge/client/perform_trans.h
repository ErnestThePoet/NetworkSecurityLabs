#ifndef PERFORM_TRANS_H_
#define PERFORM_TRANS_H_

#include "../common/sha256.h"
#include "../common/operation_result.h"

OperationResult UploadFile(
    FILE *local_file, int server_socket);
OperationResult DownloadFile(
    FILE *local_file, int server_socket);

#endif