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
    operation_result = ConnectToServer(client_arg.server_ip4_address,
                                       client_arg.server_port,
                                       &server_socket);
    if (!operation_result.is_successful)
    {
        printf("Connecting Error: %s\n", operation_result.error_info);
        return FAILURE;
    }

    FILE *local_file = NULL;
    if(client_arg.operation_mode==OPERATION_MODE_UPLOAD){
        operation_result = PrepareFileUpload(client_arg.local_file_path, &local_file);
        if (!operation_result.is_successful)
        {
            printf("Upload File Prepartion Error: %s\n", operation_result.error_info);
            return FAILURE;
        }


    }
    else{
        operation_result = PrepareFileDownload(client_arg.local_file_path, &local_file);
        if (!operation_result.is_successful)
        {
            printf("I/O Error: %s\n", operation_result.error_info);
            return FAILURE;
        }
    }

    return SUCCESS;
}
