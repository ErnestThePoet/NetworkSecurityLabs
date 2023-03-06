#include "client.h"

int main(int argc, char *argv[])
{
    const char *const kUsageHelp =
        "Usage:\n"
        "client -u <local file path> <server IPv4 address:server port:server file path>\n"
        "client -d <server IPv4 address:server port:server file path> <local file path>\n"
        "client -l <server IPv4 address:server port:server directory path>";

    ClientArg client_arg;
    OperationResult operation_result = ParseArgs(argc, argv, &client_arg);
    if (!operation_result.is_successful)
    {
        printf("Argument Error: %s\n", operation_result.error_info);
        puts(kUsageHelp);
        return FAILURE;
    }

    int server_socket = 0;
    size_t file_size = 0;
    FILE *local_file = NULL;

    switch (client_arg.operation_mode)
    {
    case OPERATION_MODE_UPLOAD:
        operation_result = PrepareFileUpload(
            client_arg.local_file_path, &local_file, &file_size);
        CHECK_FAILURE_S_F("Upload File Preparation Error: %s\n");

        operation_result = ConnectToServer(client_arg.server_ip4_address,
                                           client_arg.server_port,
                                           &server_socket);
        CHECK_FAILURE_S_F("Connecting Error: %s\n");

        operation_result = RequestFileUpload(
            file_size, client_arg.server_file_path, server_socket);
        CHECK_FAILURE_S_F("File Upload Request Error: %s\n");

        operation_result = UploadFile(local_file, file_size, server_socket);
        CHECK_FAILURE_S_F("File Upload Error: %s\n");

        break;

    case OPERATION_MODE_DOWNLOAD:
        operation_result = PrepareFileDownload(
            client_arg.local_file_path, &local_file);
        CHECK_FAILURE_S_F("Download File Preparation Error: %s\n");

        operation_result = ConnectToServer(client_arg.server_ip4_address,
                                           client_arg.server_port,
                                           &server_socket);
        CHECK_FAILURE_S_F("Connecting Error: %s\n");

        operation_result = RequestFileDownload(
            client_arg.server_file_path, server_socket, &file_size);
        CHECK_FAILURE_S_F("File Download Request Error: %s\n");

        operation_result = CheckFreeDiskSpace(file_size);
        CHECK_FAILURE_S_F("Disk Space Error: %s\n");

        operation_result = DownloadFile(local_file, file_size, server_socket);
        CHECK_FAILURE_S_F("File Download Error: %s\n");

        break;

    case OPERATION_MODE_LIST_DIR:
        operation_result = ConnectToServer(client_arg.server_ip4_address,
                                           client_arg.server_port,
                                           &server_socket);
        CHECK_FAILURE_S_F("Connecting Error: %s\n");

        operation_result = RequestListDir(
            client_arg.server_file_path, server_socket);
        CHECK_FAILURE_S_F("List Dir Request Error: %s\n");
    }

    CloseFile(local_file);
    CloseConnection(server_socket);

    return SUCCESS;
}
