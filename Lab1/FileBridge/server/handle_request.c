#include "handle_request.h"

static void SendServerDenial(const int client_socket, const char *reason)
{
    size_t packet_size = 0;
    char *packet = MakeServerDenialPacket(reason, &packet_size);

    size_t sent_size = send(client_socket, packet, packet_size, 0);

    ReleasePacket(packet);

    if (sent_size != packet_size)
    {
        printf(HANDLE_REQUEST_ERROR_TEMPLATE, "Failed to send server denial packet");
    }
}

static bool SendServerUploadPermission(const int client_socket)
{
    size_t packet_size = 0;
    char *packet = MakeServerUploadPermissionPacket(&packet_size);

    size_t sent_size = send(client_socket, packet, packet_size, 0);

    ReleasePacket(packet);

    if (sent_size != packet_size)
    {
        return false;
    }

    return true;
}

static bool SendServerUploadSuccess(const int client_socket)
{
    size_t packet_size = 0;
    char *packet = MakeServerUploadSuccessPacket(&packet_size);

    size_t sent_size = send(client_socket, packet, packet_size, 0);

    ReleasePacket(packet);

    if (sent_size != packet_size)
    {
        return false;
    }

    return true;
}

static void SendServerUploadFailure(const int client_socket, const char *reason)
{
    size_t packet_size = 0;
    char *packet = MakeServerUploadFailurePacket(reason, &packet_size);

    size_t sent_size = send(client_socket, packet, packet_size, 0);

    ReleasePacket(packet);

    if (sent_size != packet_size)
    {
        printf(HANDLE_REQUEST_ERROR_TEMPLATE, "Failed to send server upload failure packet");
    }
}

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

    size_t file_size = 0;
    FILE *server_file = NULL;

    switch (packet_type)
    {
    case PACKET_TYPE_CLIENT_UPLOAD_REQUEST:
        char *request_packet_data = (char *)malloc(request_packet_data_size);
        if (request_packet_data == NULL)
        {
            EXIT_FAILURE;
        }

        if (!ReadSocket(client_socket, request_packet_data, request_packet_data_size))
        {
            FAILURE_DENY_RETURN_S_F_FREE("Server failed to receive file upload request packet data",
                                         request_packet_data);
        }

        OperationResult result = PrepareFileUpload(request_packet_data + 4, &server_file);
        CHECK_FAILURE_S_F_FREE;

        size_t file_path_length = *((uint32_t *)request_packet_data);
        file_size = *((uint64_t *)(request_packet_data + 4 + file_path_length));

        result = CheckFreeDiskSpace(file_size);
        CHECK_FAILURE_S_F_FREE;

        if (!SendServerUploadPermission(client_socket))
        {
            FAILURE_RETURN_S_F_FREE(
                "Failed to send server upload permission packet", request_packet_data);
        }

        result = ReceiveFile(server_file, file_size, client_socket);
        if (result.is_successful)
        {
            if (!SendServerUploadSuccess(client_socket))
            {
                FAILURE_RETURN_S_F_FREE(
                    "Failed to send server upload success packet", request_packet_data);
            }
        }
        else
        {
            SendServerUploadFailure(client_socket, result.error_info);
            FAILURE_RETURN_S_F_FREE(
                result.error_info, request_packet_data);
        }

        free(request_packet_data);
        fclose(server_file);
        close(client_socket);
        
        return NULL;

    case PACKET_TYPE_CLIENT_DOWNLOAD_REQUEST:

    default:
    }
}