#include "request_trans.h"

static OperationResult ReadServerDenial(
    const size_t response_packet_data_size, const int server_socket)
{
    OperationResult result = {false};

    char *response_packet_data = (char *)malloc(response_packet_data_size);
    if (response_packet_data == NULL)
    {
        FAILURE_EXIT;
    }

    if (!ReadSocket(server_socket, response_packet_data, response_packet_data_size))
    {
        FAILURE_RETURN_FREE(response_packet_data,
                            "Failed to receive server denial packet data");
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

    size_t sent_size = send(server_socket, packet, packet_size, 0);

    ReleasePacket(packet);

    if (sent_size != packet_size)
    {
        strcpy(result.error_info, "Failed to send upload request packet");
        return result;
    }

    uint8_t response_packet_header[PACKET_HEADER_SIZE];

    if (!ReadSocket(server_socket, response_packet_header, PACKET_HEADER_SIZE))
    {
        strcpy(result.error_info,
               "Failed to receive header of upload request response header");
        return result;
    }

    size_t response_packet_data_size = 0;
    uint8_t packet_type = ExtractPacketHeader(
        response_packet_header, NULL, &response_packet_data_size);

    switch (packet_type)
    {
    case PACKET_TYPE_SERVER_DENIAL:
        return ReadServerDenial(response_packet_data_size, server_socket);
    case PACKET_TYPE_SERVER_UPLOAD_PERMISSION:
        result.is_successful = true;
        return result;
    default:
        sprintf(result.error_info,
                "Invalid upload request response received with TYPE %d",
                packet_type);
        return result;
    }
}

OperationResult RequestFileDownload(
    const char *server_file_path, const int server_socket, size_t *file_size_ret)
{
    OperationResult result = {false};

    size_t packet_size = 0;
    char *packet = MakeClientDownloadRequestPacket(server_file_path, &packet_size);

    size_t sent_size = send(server_socket, packet, packet_size, 0);

    ReleasePacket(packet);

    if (sent_size != packet_size)
    {
        strcpy(result.error_info, "Failed to send download request packet");
        return result;
    }

    uint8_t response_packet_header[PACKET_HEADER_SIZE];

    if (!ReadSocket(server_socket, response_packet_header, PACKET_HEADER_SIZE))
    {
        strcpy(result.error_info,
               "Failed to receive header of download request response header");
        return result;
    }

    size_t response_packet_data_size = 0;
    uint8_t packet_type = ExtractPacketHeader(
        response_packet_header, NULL, &response_packet_data_size);

    switch (packet_type)
    {
    case PACKET_TYPE_SERVER_DENIAL:
        return ReadServerDenial(response_packet_data_size, server_socket);
    case PACKET_TYPE_SERVER_DOWNLOAD_PERMISSION:
        break;
    default:
        sprintf(result.error_info,
                "Invalid download request response received with TYPE %d",
                packet_type);
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
    if (!ReadSocket(server_socket, file_size_data, 8))
    {
        strcpy(result.error_info,
               "Failed to receive download request response data");
        return result;
    }

    *file_size_ret = *((uint64_t *)file_size_data);

    result.is_successful = true;
    return result;
}