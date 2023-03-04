#include "fs_helpers.h"

OperationResult CheckFreeDiskSpace(size_t file_size)
{
    OperationResult result = {false};
    struct statvfs vfs;
    if (statvfs("/", &vfs) == -1)
    {
        strcpy(result.error_info, "Failed to get free disk space with statvfs()");
        return result;
    }

    const size_t free_disk_size = vfs.f_bsize * vfs.f_bavail;

    if (file_size >= free_disk_size)
    {
        sprintf(result.error_info,
                "Insufficient free disk space(%zdB) for a %zdB file",
                free_disk_size,
                file_size);
        return result;
    }

    result.is_successful = true;
    return result;
}

OperationResult OpenFile(
    const char *file_path, const char *mode, FILE **file_ret)
{
    OperationResult result = {false};

    FILE *file = fopen(file_path, mode);
    if (file == NULL)
    {
        sprintf(result.error_info, "Failed to open local file: %s", file_path);
        return result;
    }

    *file_ret = file;

    result.is_successful = true;
    return result;
}

size_t GetFileSize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    const size_t file_size = ftell(file);
    rewind(file);
    return file_size;
}

void CloseFile(FILE *file)
{
    fclose(file);
}