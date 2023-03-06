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

static bool SendServerDownloadPermission(const int client_socket, const size_t file_size)
{
    size_t packet_size = 0;
    char *packet = MakeServerDownloadPermissionPacket(file_size, &packet_size);

    size_t sent_size = send(client_socket, packet, packet_size, 0);

    ReleasePacket(packet);

    if (sent_size != packet_size)
    {
        return false;
    }

    return true;
}

static bool SendListDirResult(const int client_socket, const char *server_dir_path)
{
    char command_buffer[150];

    // 2>&1 redirects stderr to stdout and we can get the error output of cd
    sprintf(command_buffer, "cd %s 2>&1 && ls -p", server_dir_path);
    FILE *ls_pipe = popen(command_buffer, "r");

    char list_dir_result_buffer[1024] = "";
    char current_filename_buffer[150];
    while (!feof(ls_pipe) && fgets(current_filename_buffer, sizeof(current_filename_buffer), ls_pipe))
    {
        strcat(list_dir_result_buffer, current_filename_buffer);
    }

    pclose(ls_pipe);

    size_t packet_size = 0;
    char *packet = MakeServerListDirResultPacket(list_dir_result_buffer, &packet_size);

    size_t sent_size = send(client_socket, packet, packet_size, 0);

    ReleasePacket(packet);

    if (sent_size != packet_size)
    {
        return false;
    }

    return true;
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
    char *request_packet_data = NULL;
    uint8_t packet_type = ExtractPacketHeader(
        request_packet_header, NULL, &request_packet_data_size);

    size_t file_size = 0;
    FILE *server_file = NULL;

    OperationResult result = {false};

    switch (packet_type)
    {
    case PACKET_TYPE_CLIENT_UPLOAD_REQUEST:
        request_packet_data = (char *)malloc(request_packet_data_size);
        if (request_packet_data == NULL)
        {
            FAILURE_EXIT;
        }

        if (!ReadSocket(client_socket, request_packet_data, request_packet_data_size))
        {
            free(request_packet_data);
            FAILURE_DENY_RETURN_S("Server failed to receive file upload request packet data");
        }

        size_t file_path_length = *((uint32_t *)request_packet_data);
        file_size = *((uint64_t *)(request_packet_data + 4 + file_path_length));

        printf("[FILE UPLOAD REQUEST] %s\n", request_packet_data + 4);

        result = PrepareFileUpload(request_packet_data + 4, &server_file);
        free(request_packet_data);
        CHECK_FAILURE_DENY_RETURN_S_F;

        result = CheckFreeDiskSpace(file_size);
        CHECK_FAILURE_DENY_RETURN_S_F;

        if (!SendServerUploadPermission(client_socket))
        {
            FAILURE_RETURN_S_F("Failed to send server upload permission packet");
        }

        result = ReceiveFile(server_file, file_size, client_socket);
        if (result.is_successful)
        {
            if (!SendServerUploadSuccess(client_socket))
            {
                FAILURE_RETURN_S_F("Failed to send server upload success packet");
            }
        }
        else
        {
            SendServerUploadFailure(client_socket, result.error_info);
            FAILURE_RETURN_S_F(result.error_info);
        }

        fclose(server_file);

        break;

    case PACKET_TYPE_CLIENT_DOWNLOAD_REQUEST:
        request_packet_data = (char *)malloc(request_packet_data_size);
        if (request_packet_data == NULL)
        {
            FAILURE_EXIT;
        }

        if (!ReadSocket(client_socket, request_packet_data, request_packet_data_size))
        {
            free(request_packet_data);
            FAILURE_DENY_RETURN_S("Server failed to receive file download request packet data");
        }

        printf("[FILE DOWNLOAD REQUEST] %s\n", request_packet_data + 4);

        result = PrepareFileDownload(request_packet_data + 4, &server_file, &file_size);
        free(request_packet_data);
        CHECK_FAILURE_DENY_RETURN_S_F;

        if (!SendServerDownloadPermission(client_socket, file_size))
        {
            FAILURE_RETURN_S_F("Failed to send server download permission packet");
        }

        result = SendFile(server_file, file_size, client_socket);
        CHECK_FAILURE_RETURN_S_F;

        fclose(server_file);

        break;

    case PACKET_TYPE_CLIENT_LIST_DIR_REQUEST:
        request_packet_data = (char *)malloc(request_packet_data_size);
        if (request_packet_data == NULL)
        {
            FAILURE_EXIT;
        }

        if (!ReadSocket(client_socket, request_packet_data, request_packet_data_size))
        {
            free(request_packet_data);
            FAILURE_RETURN_S("Server failed to receive list dir request packet data");
        }

        printf("[LIST DIR REQUEST] %s\n", request_packet_data + 4);

        if (!SendListDirResult(client_socket, request_packet_data + 4))
        {
            FAILURE_RETURN_S("Failed to send server list dir permission packet");
        }

        free(request_packet_data);

        break;

    default:
        char message[50];
        sprintf(message, "Invalid packet received with TYPE %d", packet_type);
        FAILURE_RETURN_S(message);
    }

    close(client_socket);

    return NULL;
}