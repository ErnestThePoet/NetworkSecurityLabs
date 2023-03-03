#ifndef REQUEST_TRANS_H_
#define REQUEST_TRANS_H_

#include <stdio.h>

#include "../common/operation_result.h"

OperationResult RequestFileUpload(
    const char *server_file_path, const int server_socket);
OperationResult RequestFileDownload(
    const char *server_file_path, const int server_socket);

#endif