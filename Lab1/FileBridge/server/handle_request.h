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

#define FAILURE_RETURN_S_F(M)                     \
    do                                            \
    {                                             \
        printf(HANDLE_REQUEST_ERROR_TEMPLATE, M); \
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

#define FAILURE_DENY_RETURN_S_F(M)                \
    do                                            \
    {                                             \
        printf(HANDLE_REQUEST_ERROR_TEMPLATE, M); \
        SendServerDenial(client_socket, M);       \
        close(client_socket);                     \
        if (server_file != NULL)                  \
        {                                         \
            CloseFile(server_file);               \
        }                                         \
        return NULL;                              \
    } while (false)

#define CHECK_FAILURE_RETURN_S_F                   \
    do                                             \
    {                                              \
        if (!result.is_successful)                 \
        {                                          \
            FAILURE_RETURN_S_F(result.error_info); \
        }                                          \
    } while (false)

#define CHECK_FAILURE_DENY_RETURN_S_F                   \
    do                                                  \
    {                                                   \
        if (!result.is_successful)                      \
        {                                               \
            FAILURE_DENY_RETURN_S_F(result.error_info); \
        }                                               \
    } while (false)

void *HandleRequest(void *client_socket_ptr);

#endif
