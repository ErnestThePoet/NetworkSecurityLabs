#include "packets.h"

uint8_t *MakeClientUploadRequestPacket(const char *server_file_path,
                                       const uint64_t file_size,
                                       const uint8_t *sha256,
                                       uint64_t *packet_size_ret)
{
    const size_t server_file_path_length = strlen(server_file_path);

    const size_t packet_size = PACKET_HEADER_SIZE + 4 + 8 + 32 + server_file_path_length;

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
    memcpy(packet + PACKET_HEADER_SIZE + 4 + server_file_path_length + 8,
           sha256,
           32);

    return packet;
}

uint8_t *MakeClientDownloadRequestPacket(const char *server_file_path,
                                         uint64_t *packet_size_ret)
{
    const size_t server_file_path_length = strlen(server_file_path);

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

    return packet;
}

uint8_t *MakeServerDenialPacket(const char *reason,
                                uint64_t *packet_size_ret)
{
    const size_t reason_length = strlen(reason);

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

    return packet;
}

uint8_t *MakeServerUploadAckPacket(uint64_t *packet_size_ret)
{
    const size_t packet_size = PACKET_HEADER_SIZE;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        return NULL;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_UPLOAD_ACK;
    *((uint64_t *)(packet + 1)) = packet_size;

    return packet;
}

uint8_t *MakeServerDownloadAckPacket(const uint64_t file_size,
                                     const uint8_t *sha256,
                                     uint64_t *packet_size_ret)
{
    const size_t packet_size = PACKET_HEADER_SIZE + 8 + 32;

    uint8_t *packet = (uint8_t *)malloc(packet_size);
    if (packet == NULL)
    {
        return NULL;
    }

    *packet_size_ret = packet_size;

    packet[0] = PACKET_TYPE_SERVER_DOWNLOAD_ACK;
    *((uint64_t *)(packet + 1)) = packet_size;

    *((uint64_t *)(packet + PACKET_HEADER_SIZE)) = file_size;
    memcpy(packet + PACKET_HEADER_SIZE + 8,
           sha256,
           32);

    return packet;
}

void ReleasePacket(uint8_t *packet){
    free(packet);
}