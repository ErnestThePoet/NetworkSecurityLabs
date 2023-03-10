#include "file_operations.h"

OperationResult PrepareFileUpload(
    const char *local_file_path, FILE **file_ret, size_t *file_size_ret)
{
    FILE *local_file = NULL;
    OperationResult result = OpenFile(local_file_path, "rb", &local_file);
    if (!result.is_successful)
    {
        return result;
    }

    *file_ret = local_file;
    
    size_t file_size = GetFileSize(local_file);
    *file_size_ret = file_size;

    return result;
}

OperationResult PrepareFileDownload(const char *local_file_path, FILE **file_ret)
{
    return OpenFile(local_file_path, "wb", file_ret);
}