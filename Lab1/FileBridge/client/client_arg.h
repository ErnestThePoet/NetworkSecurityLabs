#define OPERATION_MODE_UPLOAD 0
#define OPERATION_MODE_DOWNLOAD 1

typedef struct
{
    int operation_mode;
    const char *client_path;
    const char *server_ip4_address;
    int server_port;
    const char *server_path;
} ClientArg;