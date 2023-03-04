#include "handle_request.h"

void *HandleRequest(void *client_socket_ptr)
{
    const int client_socket = *((int *)client_socket_ptr);

    uint8_t request_packet_header[PACKET_HEADER_SIZE];

    if (!ReadSocket(client_socket, request_packet_header, PACKET_HEADER_SIZE))
    {
        FAILURE_RETURN_S("Failed to receive header of upload feedback header");
    }

    size_t request_packet_data_size = 0;
    uint8_t packet_type = ExtractPacketHeader(
        request_packet_header, NULL, &request_packet_data_size);

    switch(packet_type){
        case PACKET_TYPE_CLIENT_UPLOAD_REQUEST:
            char* request_packet_data=malloc

        case PACKET_TYPE_CLIENT_DOWNLOAD_REQUEST:

        default:

    }
}