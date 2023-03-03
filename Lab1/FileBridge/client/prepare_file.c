#include "prepare_file.h"

static OperationResult PrepareFile(const char *local_file_path, const char *mode, FILE **file_ret)
{
    OperationResult result = {false};

    FILE *file = fopen(local_file_path, mode);
    if (file == NULL)
    {
        strcpy(result.error_info, "Failed to open local file");
        return result;
    }

    *file_ret = file;

    result.is_successful = true;
    return result;
}

OperationResult PrepareFileUpload(const char *local_file_path, FILE **file_ret)
{
    return PrepareFile(local_file_path, "rb", file_ret);
}

OperationResult PrepareFileDownload(const char *local_file_path, FILE **file_ret)
{
    return PrepareFile(local_file_path, "wb", file_ret);
}