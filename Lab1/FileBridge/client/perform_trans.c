#include "perform_trans.h"

OperationResult UploadFile(
    FILE *local_file, const size_t file_size, const int server_socket)
{
    OperationResult result = {false};

    SHA256_CTX sha256_ctx;
    sha256_init(&sha256_ctx);

    char *file_buffer = (char *)malloc(UPLOAD_BUFFER_SIZE);
    if (file_buffer == NULL)
    {
        strcpy(result.error_info, "Failed to allocate file upload buffer");
        return result;
    }

    const int chunk_count = file_size == 0 ? 0 : (file_size - 1) / UPLOAD_BUFFER_SIZE + 1;

    StartProgressBar();

    size_t read_file_size = 0;
    for (int i = 0; i < chunk_count; i++)
    {
        const size_t current_read_size =
            (file_size - read_file_size) >= UPLOAD_BUFFER_SIZE
                ? UPLOAD_BUFFER_SIZE
                : (file_size - read_file_size);
        if (fread(file_buffer, 1, current_read_size, local_file) != current_read_size)
        {
            FAILURE_RETURN_FREE(file_buffer, "Failed to read local file");
        }

        if (send(server_socket, file_buffer, current_read_size, 0) != current_read_size)
        {
            FAILURE_RETURN_FREE(file_buffer, "Failed to send local file");
        }

        sha256_update(&sha256_ctx, file_buffer, current_read_size);

        read_file_size += current_read_size;

        UpdateProgressBar((double)(i + 1) / chunk_count);
    }

    EndProgressBar();

    free(file_buffer);

    char sha256[SHA256_BLOCK_SIZE];
    sha256_final(&sha256_ctx, sha256);
    if (send(server_socket, sha256, SHA256_BLOCK_SIZE, 0) != SHA256_BLOCK_SIZE)
    {
        strcpy(result.error_info, "Failed to send file SHA256");
        return result;
    }

    uint8_t response_packet_header[PACKET_HEADER_SIZE];

    if (!ReadSocket(server_socket, response_packet_header, PACKET_HEADER_SIZE))
    {
        strcpy(result.error_info,
               "Failed to receive header of upload feedback header");
        return result;
    }

    size_t response_packet_data_size = 0;
    uint8_t packet_type = ExtractPacketHeader(
        response_packet_header, NULL, &response_packet_data_size);

    switch (packet_type)
    {
    case PACKET_TYPE_SERVER_UPLOAD_SUCCESSFUL:
        result.is_successful = true;
        return result;
    case PACKET_TYPE_SERVER_UPLOAD_FAILED:
        char *response_packet_data = (char *)malloc(response_packet_data_size);
        if (response_packet_data == NULL)
        {
            strcpy(result.error_info,
                   "Failed to allocate memory for server upload failed feedback packet data");
            return result;
        }

        if (!ReadSocket(server_socket, response_packet_data, response_packet_data_size))
        {
            FAILURE_RETURN_FREE(response_packet_data,
                                "Failed to receive server upload failed feedback packet data");
        }

        sprintf(result.error_info,
                "Server reported upload failure because: %s",
                response_packet_data + 4);

        free(response_packet_data);

        return result;
    default:
        sprintf(result.error_info,
                "Invalid upload request response received with TYPE %d",
                packet_type);
        return result;
    }
}

OperationResult DownloadFile(
    FILE *local_file, const size_t file_size, const int server_socket)
{
    OperationResult result = {false};

    SHA256_CTX sha256_ctx;
    sha256_init(&sha256_ctx);

    char *file_buffer = (char *)malloc(DOWNLOAD_BUFFER_SIZE);
    if (file_buffer == NULL)
    {
        strcpy(result.error_info, "Failed to allocate file download buffer");
        return result;
    }

    const int chunk_count = file_size == 0 ? 0 : (file_size - 1) / DOWNLOAD_BUFFER_SIZE + 1;

    StartProgressBar();

    size_t received_file_size = 0;
    for (int i = 0; i < chunk_count; i++)
    {
        const size_t current_receive_size =
            (file_size - received_file_size) >= DOWNLOAD_BUFFER_SIZE
                ? DOWNLOAD_BUFFER_SIZE
                : (file_size - received_file_size);

        if (!ReadSocket(server_socket, file_buffer, current_receive_size))
        {
            FAILURE_RETURN_FREE(file_buffer, "Failed to receive server file");
        }

        if (fwrite(file_buffer, 1, current_receive_size, local_file) != current_receive_size)
        {
            FAILURE_RETURN_FREE(file_buffer, "Failed to write local file");
        }

        sha256_update(&sha256_ctx, file_buffer, current_receive_size);

        received_file_size += current_receive_size;

        UpdateProgressBar((double)(i + 1) / chunk_count);
    }

    EndProgressBar();

    free(file_buffer);

    char sha256[SHA256_BLOCK_SIZE];
    char received_sha256[SHA256_BLOCK_SIZE];

    sha256_final(&sha256_ctx, sha256);

    if (!ReadSocket(server_socket, received_sha256, SHA256_BLOCK_SIZE))
    {
        strcpy(result.error_info, "Failed to receive file SHA256");
        return result;
    }

    if (memcmp(sha256, received_sha256, SHA256_BLOCK_SIZE))
    {
        puts("Download file SHA256 check failed, the file might have been corrupted");
    }

    result.is_successful = true;
    return result;
}