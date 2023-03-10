#include "packets.h"

char *MakeClientUploadRequestPacket(const char *server_file_path,
                                    const size_t file_size,
                                    size_t *packet_size_ret)
{
    const size_t server_file_path_length = strlen(server_file_path) + 1;

    const size_t packet_size = PACKET_HEADER_SIZE + 4 + 8 + server_file_path_length;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        FAILURE_EXIT;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_CLIENT_UPLOAD_REQUEST;
    *((uint64_t *)(packet + 1)) = packet_size;

    *((uint32_t *)(packet + PACKET_HEADER_SIZE)) = server_file_path_length;
    memcpy(packet + PACKET_HEADER_SIZE + 4,
           server_file_path,
           server_file_path_length);
    *((uint64_t *)(packet + PACKET_HEADER_SIZE + 4 + server_file_path_length)) = file_size;

    return (char *)packet;
}

char *MakeClientDownloadRequestPacket(const char *server_file_path,
                                      size_t *packet_size_ret)
{
    const size_t server_file_path_length = strlen(server_file_path) + 1;

    const size_t packet_size = PACKET_HEADER_SIZE + 4 + server_file_path_length;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        FAILURE_EXIT;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_CLIENT_DOWNLOAD_REQUEST;
    *((uint64_t *)(packet + 1)) = packet_size;

    *((uint32_t *)(packet + PACKET_HEADER_SIZE)) = server_file_path_length;
    memcpy(packet + PACKET_HEADER_SIZE + 4,
           server_file_path,
           server_file_path_length);

    return (char *)packet;
}

char *MakeClientListDirRequestPacket(const char *server_dir_path,
                                     size_t *packet_size_ret)
{
    const size_t server_dir_path_length = strlen(server_dir_path) + 1;

    const size_t packet_size = PACKET_HEADER_SIZE + 4 + server_dir_path_length;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        FAILURE_EXIT;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_CLIENT_LIST_DIR_REQUEST;
    *((uint64_t *)(packet + 1)) = packet_size;

    *((uint32_t *)(packet + PACKET_HEADER_SIZE)) = server_dir_path_length;
    memcpy(packet + PACKET_HEADER_SIZE + 4,
           server_dir_path,
           server_dir_path_length);

    return (char *)packet;
}

char *MakeServerDenialPacket(const char *reason,
                             size_t *packet_size_ret)
{
    const size_t reason_length = strlen(reason) + 1;

    const size_t packet_size = PACKET_HEADER_SIZE + 4 + reason_length;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        FAILURE_EXIT;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_DENIAL;
    *((uint64_t *)(packet + 1)) = packet_size;

    *((uint32_t *)(packet + PACKET_HEADER_SIZE)) = reason_length;
    memcpy(packet + PACKET_HEADER_SIZE + 4,
           reason,
           reason_length);

    return (char *)packet;
}

char *MakeServerUploadPermissionPacket(size_t *packet_size_ret)
{
    const size_t packet_size = PACKET_HEADER_SIZE;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        FAILURE_EXIT;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_UPLOAD_PERMISSION;
    *((uint64_t *)(packet + 1)) = packet_size;

    return (char *)packet;
}

char *MakeServerDownloadPermissionPacket(const size_t file_size,
                                         size_t *packet_size_ret)
{
    const size_t packet_size = PACKET_HEADER_SIZE + 8;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        FAILURE_EXIT;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_DOWNLOAD_PERMISSION;
    *((uint64_t *)(packet + 1)) = packet_size;

    *((uint64_t *)(packet + PACKET_HEADER_SIZE)) = file_size;

    return (char *)packet;
}

char *MakeServerUploadSuccessPacket(size_t *packet_size_ret)
{
    const size_t packet_size = PACKET_HEADER_SIZE;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        FAILURE_EXIT;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_UPLOAD_SUCCESS;
    *((uint64_t *)(packet + 1)) = packet_size;

    return (char *)packet;
}

char *MakeServerUploadFailurePacket(const char *reason,
                                    size_t *packet_size_ret)
{
    const size_t reason_length = strlen(reason) + 1;

    const size_t packet_size = PACKET_HEADER_SIZE + 4 + reason_length;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        FAILURE_EXIT;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_UPLOAD_FAILURE;
    *((uint64_t *)(packet + 1)) = packet_size;

    *((uint32_t *)(packet + PACKET_HEADER_SIZE)) = reason_length;
    memcpy(packet + PACKET_HEADER_SIZE + 4,
           reason,
           reason_length);

    return (char *)packet;
}

char *MakeServerListDirResultPacket(const char *list_dir_result,
                                    size_t *packet_size_ret)
{
    const size_t list_dir_result_length = strlen(list_dir_result) + 1;

    const size_t packet_size = PACKET_HEADER_SIZE + 4 + list_dir_result_length;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        FAILURE_EXIT;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_LIST_DIR_RESULT;
    *((uint64_t *)(packet + 1)) = packet_size;

    *((uint32_t *)(packet + PACKET_HEADER_SIZE)) = list_dir_result_length;
    memcpy(packet + PACKET_HEADER_SIZE + 4,
           list_dir_result,
           list_dir_result_length);

    return (char *)packet;
}

uint8_t ExtractPacketHeader(const char *header,
                            size_t *packet_size_ret,
                            size_t *packet_data_size_ret)
{
    if (packet_size_ret != NULL)
    {
        *packet_size_ret = *((uint64_t *)(header + 1));
    }

    if (packet_data_size_ret != NULL)
    {
        *packet_data_size_ret = *((uint64_t *)(header + 1)) - PACKET_HEADER_SIZE;
    }

    return header[0];
}

void ReleasePacket(char *packet)
{
    free(packet);
}