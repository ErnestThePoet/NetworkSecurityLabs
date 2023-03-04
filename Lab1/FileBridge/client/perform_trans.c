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

    size_t read_file_size = 0;
    for (int i = 0; i < chunk_count; i++)
    {
        const size_t current_read_size =
            (file_size - read_file_size) >= UPLOAD_BUFFER_SIZE
                ? UPLOAD_BUFFER_SIZE
                : (file_size - read_file_size);
        if(fread(file_buffer,1,current_read_size,local_file)!=current_read_size){
            strcpy(result.error_info, "Failed to read local file");
            return result;
        }

        if(send(server_socket,file_buffer,current_read_size,0)!=current_read_size){
            strcpy(result.error_info, "Failed to send local file");
            return result;
        }

        sha256_update(&sha256_ctx, file_buffer, current_read_size);

        read_file_size += current_read_size;
    }

    free(file_buffer);

    char sha256[SHA256_BLOCK_SIZE];
    sha256_final(&sha256_ctx, sha256);
    if (send(server_socket, sha256, SHA256_BLOCK_SIZE, 0) != SHA256_BLOCK_SIZE)
    {
        strcpy(result.error_info, "Failed to send file SHA256");
        return result;
    }

    uint8_t response_packet_header[PACKET_HEADER_SIZE];

    if (!ReadSocket(server_socket, PACKET_HEADER_SIZE, response_packet_header))
    {
        strcpy(result.error_info,
               "Failed to receive header of upload feedback header");
        return result;
    }

    size_t response_packet_data_size = 0;
    uint8_t packet_type = ExtractPacketHeader(
        response_packet_header, NULL, &response_packet_data_size);

    if(packet_type==PACKET_TYPE_SERVER_UPLOAD_SUCCESSFUL){
        result.is_successful = true;
        return result;
    }
    else if(packet_type==PACKET_TYPE_SERVER_UPLOAD_FAILED){
        char *response_packet_data = (char *)malloc(response_packet_data_size);
        if (response_packet_data == NULL)
        {
            strcpy(result.error_info,
                   "Failed to allocate memory for server upload failed feedback packet data");
            return result;
        }

        if (!ReadSocket(server_socket, response_packet_data_size, response_packet_data))
        {
            strcpy(result.error_info,
                   "Failed to receive server upload failed feedback packet data");
            return result;
        }

        sprintf(result.error_info,
                "Server reported upload failure because: %s",
                response_packet_data + 4);

        free(response_packet_data);

        return result;
    }
    else{
        sprintf(result.error_info,
                "Invalid upload request response received with TYPE %d",
                packet_type);
        return result;
    }
}