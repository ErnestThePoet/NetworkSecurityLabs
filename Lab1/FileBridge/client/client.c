#include "client.h"

int main(int argc, char *argv[])
{
    const char *kUsageHelp =
        "Usage:\n"
        "client -u <file path> <server IPv4 address:server port:server storage path>\n"
        "client -d <server IPv4 address:server port:server storage path> <file path>";

    ArgParseResult r = ParseArgs(argc, argv);

    printf("%d", r.is_valid);

    return 0;
}

ArgParseResult ParseArgs(int argc, char *argv[])
{
    ArgParseResult result = {false};

    if (argc != 4)
    {
        result.error_info = "Invalid input argument number";
        return result;
    }

    if (!strcmp(argv[1], "-u"))
    {
        result.client_arg.operation_mode = OPERATION_MODE_UPLOAD;
    }
    else if (!strcmp(argv[1], "-d"))
    {
        result.client_arg.operation_mode = OPERATION_MODE_DOWNLOAD;
    }
    else
    {
        result.error_info = "Invalid operation mode specifier";
        return result;
    }

    const char *server_arg =
        result.client_arg.operation_mode == OPERATION_MODE_UPLOAD ? argv[3] : argv[2];


    

    return result;
}