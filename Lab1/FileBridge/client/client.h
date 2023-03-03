#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "client_arg.h"
#include "../common/operation_result.h"
#include "../common/sha256.h"

#define SUCCESS 0
#define FAILURE 1

typedef struct sockaddr_in sockaddr_in;

OperationResult ParseArgs(int argc, char **argv, ClientArg *client_arg_ret);
OperationResult PrepareFileUpload(const char *local_file_path, FILE **file_ret);
OperationResult PrepareFileDownload(const char *local_file_path, FILE **file_ret);
OperationResult ConnectToServer(const char *ip4_address, int port, int *server_socket_ret);
OperationResult RequestFileUpload(
    const char *server_file_path, int server_socket);
OperationResult RequestFileDownload(
    const char *server_file_path, int server_socket);
OperationResult UploadFile(
    FILE *local_file, int server_socket);
OperationResult DownloadFile(
    FILE *local_file, int server_socket);
void CloseConnection(int server_socket);