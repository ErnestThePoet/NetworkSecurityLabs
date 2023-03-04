#ifndef HANDLE_REQUEST_H_
#define HANDLE_REQUEST_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "file_operations.h"
#include "perform_trans.h"

#include "../common/file_bridge.h"
#include "../common/fs_helpers.h"
#include "../common/operation_result.h"
#include "../common/packets.h"
#include "../common/read_socket.h"

#define HANDLE_REQUEST_ERROR_TEMPLATE "Request Handling Error: %s\n"

#define FAILURE_RETURN_S(M)                       \
    do                                            \
    {                                             \
        printf(HANDLE_REQUEST_ERROR_TEMPLATE, M); \
        close(client_socket);                     \
        return NULL;                              \
    } while (false)

#define FAILURE_RETURN_S_F_FREE(M, P)             \
    do                                            \
    {                                             \
        printf(HANDLE_REQUEST_ERROR_TEMPLATE, M); \
        free(P);                                  \
        close(client_socket);                     \
        if (server_file != NULL)                  \
        {                                         \
            CloseFile(server_file);               \
        }                                         \
        return NULL;                              \
    } while (false)

#define FAILURE_DENY_RETURN_S(M)                  \
    do                                            \
    {                                             \
        printf(HANDLE_REQUEST_ERROR_TEMPLATE, M); \
        SendServerDenial(client_socket, M);       \
        close(client_socket);                     \
        return NULL;                              \
    } while (false)

#define FAILURE_DENY_RETURN_S_F_FREE(M, P)        \
    do                                            \
    {                                             \
        printf(HANDLE_REQUEST_ERROR_TEMPLATE, M); \
        free(P);                                  \
        SendServerDenial(client_socket, M);       \
        close(client_socket);                     \
        if (server_file != NULL)                  \
        {                                         \
            CloseFile(server_file);               \
        }                                         \
        return NULL;                              \
    } while (false)

#define CHECK_FAILURE_S_F_FREE                           \
    do                                                   \
    {                                                    \
        if (!result.is_successful)                       \
        {                                                \
            FAILURE_DENY_RETURN_S_F_FREE(                \
                result.error_info, request_packet_data); \
        }                                                \
    } while (false)

void *HandleRequest(void *client_socket_ptr);

#endif
