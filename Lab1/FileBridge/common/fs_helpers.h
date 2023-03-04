#ifndef FS_HELPERS_H_
#define FS_HELPERS_H_

#include <stdio.h>
#include <string.h>
#include <sys/statvfs.h>

#include "operation_result.h"

OperationResult CheckFreeDiskSpace(size_t file_size);
OperationResult OpenFile(
    const char *file_path, const char *mode, FILE **file_ret);
size_t GetFileSize(FILE *file);
void CloseFile(FILE *file);

#endif