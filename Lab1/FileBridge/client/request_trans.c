#include "request_trans.h"

static OperationResult ReadServerDenial(
    const size_t response_packet_data_size, const int server_socket)
{
    OperationResult result = {false};

    char *response_packet_data = (char *)malloc(response_packet_data_size);
    if (response_packet_data == NULL)
    {
        strcpy(result.error_info,
               "Failed to allocate memory for server denial packet data");
        return result;
    }

    if (!ReadSocket(server_socket, response_packet_data_size, response_packet_data))
    {
        strcpy(result.error_info,
               "Failed to receive server denial packet data");
        return result;
    }

    sprintf(result.error_info,
            "Server denied the upload request because: %s",
            response_packet_data + 4);

    free(response_packet_data);

    return result;
}

OperationResult RequestFileUpload(
    const size_t file_size,
    const char *server_file_path,
    const int server_socket)
{
    OperationResult result = {false};

    size_t packet_size = 0;
    char *packet = MakeClientUploadRequestPacket(
        server_file_path, file_size, &packet_size);
    if (packet == NULL)
    {
        strcpy(result.error_info, "Failed to allocate memory for upload request packet");
        return result;
    }

    if (send(server_socket, packet, packet_size, 0) != packet_size)
    {
        strcpy(result.error_info, "Failed to send upload request packet");
        return result;
    }

    uint8_t response_packet_header[PACKET_HEADER_SIZE];

    if (!ReadSocket(server_socket, PACKET_HEADER_SIZE, response_packet_header))
    {
        strcpy(result.error_info,
               "Failed to receive header of upload request response header");
        return result;
    }

    size_t response_packet_data_size =
        *((uint64_t *)(response_packet_header + 1)) - PACKET_HEADER_SIZE;

    if (response_packet_header[0] == PACKET_TYPE_SERVER_DENIAL)
    {
        return ReadServerDenial(response_packet_data_size, server_socket);
    }
    else if (response_packet_header[0] != PACKET_TYPE_SERVER_UPLOAD_PERMITTED)
    {
        sprintf(result.error_info,
                "Invalid upload request response received with TYPE %d",
                response_packet_header[0]);
        return result;
    }

    result.is_successful = true;
    return result;
}

OperationResult RequestFileDownload(
    const char *server_file_path, const int server_socket, size_t *file_size_ret)
{
    OperationResult result = {false};

    size_t packet_size = 0;
    char *packet = MakeClientDownloadRequestPacket(server_file_path, &packet_size);
    if (packet == NULL)
    {
        strcpy(result.error_info, "Failed to allocate memory for download request packet");
        return result;
    }

    if (send(server_socket, packet, packet_size, 0) != packet_size)
    {
        strcpy(result.error_info, "Failed to send download request packet");
        return result;
    }

    uint8_t response_packet_header[PACKET_HEADER_SIZE];

    if (!ReadSocket(server_socket, PACKET_HEADER_SIZE, response_packet_header))
    {
        strcpy(result.error_info,
               "Failed to receive header of download request response header");
        return result;
    }

    size_t response_packet_data_size =
        *((uint64_t *)(response_packet_header + 1)) - PACKET_HEADER_SIZE;

    if (response_packet_header[0] == PACKET_TYPE_SERVER_DENIAL)
    {
        return ReadServerDenial(response_packet_data_size, server_socket);
    }
    else if (response_packet_header[0] != PACKET_TYPE_SERVER_UPLOAD_PERMITTED)
    {
        sprintf(result.error_info,
                "Invalid download request response received with TYPE %d",
                response_packet_header[0]);
        return result;
    }

    if (response_packet_data_size != 8)
    {
        sprintf(result.error_info,
                "Unexpected download request response data size: %zd",
                response_packet_data_size);
        return result;
    }

    char file_size_data[8];
    if (!ReadSocket(server_socket, 8, file_size_data))
    {
        strcpy(result.error_info,
               "Failed to receive download request response data");
        return result;
    }

    *file_size_ret = *((uint64_t *)file_size_data);

    result.is_successful = true;
    return result;
}