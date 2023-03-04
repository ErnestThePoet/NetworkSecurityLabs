#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <stdio.h>
#include <string.h>

#include "../common/operation_result.h"
#include "../common/fs_helpers.h"

OperationResult PrepareFileUpload(const char *server_file_path, FILE **file_ret);
OperationResult PrepareFileDownload(
    const char *server_file_path, FILE **file_ret, size_t *file_size_ret);

#endif