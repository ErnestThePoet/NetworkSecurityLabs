#include "perform_trans.h"

OperationResult ReceiveFile(
    FILE *server_file, const size_t file_size, const int client_socket)
{
    OperationResult result = {false};

    SHA256_CTX sha256_ctx;
    sha256_init(&sha256_ctx);

    char *file_buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
    if (file_buffer == NULL)
    {
        FAILURE_EXIT;
    }

    const int chunk_count = file_size == 0 ? 0 : (file_size - 1) / RECEIVE_BUFFER_SIZE + 1;

    size_t received_file_size = 0;
    for (int i = 0; i < chunk_count; i++)
    {
        const size_t current_receive_size =
            (file_size - received_file_size) >= RECEIVE_BUFFER_SIZE
                ? RECEIVE_BUFFER_SIZE
                : (file_size - received_file_size);

        if (!ReadSocket(client_socket, file_buffer, current_receive_size))
        {
            FAILURE_RETURN_FREE(file_buffer, "Failed to receive upload file");
        }

        if (fwrite(file_buffer, 1, current_receive_size, server_file) != current_receive_size)
        {
            FAILURE_RETURN_FREE(file_buffer, "Failed to write server file");
        }

        sha256_update(&sha256_ctx, file_buffer, current_receive_size);

        received_file_size += current_receive_size;
    }

    free(file_buffer);

    char sha256[SHA256_BLOCK_SIZE];
    char received_sha256[SHA256_BLOCK_SIZE];

    sha256_final(&sha256_ctx, sha256);

    if (!ReadSocket(client_socket, received_sha256, SHA256_BLOCK_SIZE))
    {
        strcpy(result.error_info, "Failed to receive upload file SHA256");
        return result;
    }

    if (memcmp(sha256, received_sha256, SHA256_BLOCK_SIZE))
    {
        strcpy(result.error_info, "Upload file SHA256 check failed, the file might have been corrupted");
        return result;
    }

    result.is_successful = true;
    return result;
}
OperationResult SendFile(
    FILE *server_file, const size_t file_size, const int client_socket)
{
    OperationResult result = {false};

    SHA256_CTX sha256_ctx;
    sha256_init(&sha256_ctx);

    char *file_buffer = (char *)malloc(SEND_BUFFER_SIZE);
    if (file_buffer == NULL)
    {
        FAILURE_EXIT;
    }

    const int chunk_count = file_size == 0 ? 0 : (file_size - 1) / SEND_BUFFER_SIZE + 1;

    size_t read_file_size = 0;
    for (int i = 0; i < chunk_count; i++)
    {
        const size_t current_read_size =
            (file_size - read_file_size) >= SEND_BUFFER_SIZE
                ? SEND_BUFFER_SIZE
                : (file_size - read_file_size);
        if (fread(file_buffer, 1, current_read_size, server_file) != current_read_size)
        {
            FAILURE_RETURN_FREE(file_buffer, "Failed to read server file");
        }

        if (send(client_socket, file_buffer, current_read_size, 0) != current_read_size)
        {
            FAILURE_RETURN_FREE(file_buffer, "Failed to send server file");
        }

        sha256_update(&sha256_ctx, file_buffer, current_read_size);

        read_file_size += current_read_size;
    }

    free(file_buffer);

    char sha256[SHA256_BLOCK_SIZE];
    sha256_final(&sha256_ctx, sha256);
    if (send(client_socket, sha256, SHA256_BLOCK_SIZE, 0) != SHA256_BLOCK_SIZE)
    {
        strcpy(result.error_info, "Failed to send file SHA256");
        return result;
    }

    result.is_successful = true;
    return result;
}