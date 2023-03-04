#ifndef CLIENT_H_
#define CLIENT_H_

#include "parse_args.h"
#include "file_operations.h"
#include "connection.h"
#include "request_trans.h"
#include "perform_trans.h"

#include "../common/file_bridge.h"
#include "../common/fs_helpers.h"

#define CHECK_FAILURE(TEMPLATE)                            \
    do                                                     \
    {                                                      \
        if (!operation_result.is_successful)               \
        {                                                  \
            printf(TEMPLATE, operation_result.error_info); \
            return FAILURE;                                \
        }                                                  \
    } while (false)

#define CHECK_FAILURE_S(TEMPLATE)                          \
    do                                                     \
    {                                                      \
        if (!operation_result.is_successful)               \
        {                                                  \
            printf(TEMPLATE, operation_result.error_info); \
            CloseConnection(server_socket);                \
            return FAILURE;                                \
        }                                                  \
    } while (false)

#define CHECK_FAILURE_S_F(TEMPLATE)                        \
    do                                                     \
    {                                                      \
        if (!operation_result.is_successful)               \
        {                                                  \
            printf(TEMPLATE, operation_result.error_info); \
            CloseConnection(server_socket);                \
            if (local_file != NULL)                        \
            {                                              \
                CloseFile(local_file);                     \
            }                                              \
            return FAILURE;                                \
        }                                                  \
    } while (false)

#endif