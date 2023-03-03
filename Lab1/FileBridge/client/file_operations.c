#include "file_operations.h"

static OperationResult PrepareFile(
    const char *local_file_path, const char *mode, FILE **file_ret)
{
    OperationResult result = {false};

    FILE *file = fopen(local_file_path, mode);
    if (file == NULL)
    {
        strcpy(result.error_info, "Failed to open local file: ");
        strcat(result.error_info, local_file_path);
        return result;
    }

    *file_ret = file;

    result.is_successful = true;
    return result;
}

static size_t GetFileSize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    const size_t file_size = ftell(file);
    rewind(file);
    return file_size;
}

OperationResult PrepareFileUpload(
    const char *local_file_path, FILE **file_ret, size_t *file_size_ret)
{
    FILE *local_file = NULL;
    OperationResult result = PrepareFile(local_file_path, "rb", &local_file);
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
    return PrepareFile(local_file_path, "wb", file_ret);
}