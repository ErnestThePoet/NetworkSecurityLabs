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
        return NULL;
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
        return NULL;
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

char *MakeServerDenialPacket(const char *reason,
                             size_t *packet_size_ret)
{
    const size_t reason_length = strlen(reason) + 1;

    const size_t packet_size = PACKET_HEADER_SIZE + 4 + reason_length;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        return NULL;
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

char *MakeServerUploadPermittedPacket(size_t *packet_size_ret)
{
    const size_t packet_size = PACKET_HEADER_SIZE;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        return NULL;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_UPLOAD_PERMITTED;
    *((uint64_t *)(packet + 1)) = packet_size;

    return (char *)packet;
}

char *MakeServerDownloadPermittedPacket(const size_t file_size,
                                        size_t *packet_size_ret)
{
    const size_t packet_size = PACKET_HEADER_SIZE + 8;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        return NULL;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_DOWNLOAD_PERMITTED;
    *((uint64_t *)(packet + 1)) = packet_size;

    *((uint64_t *)(packet + PACKET_HEADER_SIZE)) = file_size;

    return (char *)packet;
}

char *MakeServerUploadSuccessfulPacket(size_t *packet_size_ret)
{
    const size_t packet_size = PACKET_HEADER_SIZE;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        return NULL;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_UPLOAD_SUCCESSFUL;
    *((uint64_t *)(packet + 1)) = packet_size;

    return (char *)packet;
}

char *MakeServerUploadFailedPacket(const char *reason,
                                   size_t *packet_size_ret)
{
    const size_t reason_length = strlen(reason) + 1;

    const size_t packet_size = PACKET_HEADER_SIZE + 4 + reason_length;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        return NULL;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_UPLOAD_FAILED;
    *((uint64_t *)(packet + 1)) = packet_size;

    *((uint32_t *)(packet + PACKET_HEADER_SIZE)) = reason_length;
    memcpy(packet + PACKET_HEADER_SIZE + 4,
           reason,
           reason_length);

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