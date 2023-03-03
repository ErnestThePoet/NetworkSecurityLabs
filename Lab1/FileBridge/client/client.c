#include "client.h"

int main(int argc, char *argv[])
{
    const char *const kUsageHelp =
        "Usage:\n"
        "client -u <local file path> <server IPv4 address:server port:server file path>\n"
        "client -d <server IPv4 address:server port:server file path> <local file path>";

    ClientArg client_arg;
    OperationResult operation_result = ParseArgs(argc, argv, &client_arg);
    if (!operation_result.is_successful)
    {
        printf("Argument Error: %s\n", operation_result.error_info);
        puts(kUsageHelp);
        return FAILURE;
    }

    int server_socket = 0;
    FILE *local_file = NULL;
    if (client_arg.operation_mode == OPERATION_MODE_UPLOAD)
    {
        size_t file_size = 0;
        operation_result = PrepareFileUpload(
            client_arg.local_file_path, &local_file, &file_size);
        CHECK_FAILURE_C_F("Upload File Preparation Error: %s\n");

        operation_result = ConnectToServer(client_arg.server_ip4_address,
                                           client_arg.server_port,
                                           &server_socket);
        CHECK_FAILURE_C_F("Connecting Error: %s\n");

        operation_result = RequestFileUpload(
            file_size, client_arg.server_file_path, server_socket);
        CHECK_FAILURE_C_F("File Upload Request Error: %s\n");
    }
    else
    {
        operation_result = PrepareFileDownload(
            client_arg.local_file_path, &local_file);
        CHECK_FAILURE_C_F("Download File Preparation Error: %s\n");

        operation_result = ConnectToServer(client_arg.server_ip4_address,
                                           client_arg.server_port,
                                           &server_socket);
        CHECK_FAILURE_C_F("Connecting Error: %s\n");

        size_t file_size = 0;
        operation_result = RequestFileDownload(
            client_arg.server_file_path, server_socket, &file_size);
        CHECK_FAILURE_C_F("File Download Request Error: %s\n");
    }

    return SUCCESS;
}
