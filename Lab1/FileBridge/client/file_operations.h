#ifndef PREPARE_FILE_H_
#define PREPARE_FILE_H_

#include <stdio.h>
#include <string.h>

#include "../common/operation_result.h"

OperationResult PrepareFileUpload(
    const char *local_file_path, FILE **file_ret, size_t *file_size_ret);
OperationResult PrepareFileDownload(const char *local_file_path, FILE **file_ret);

#endif