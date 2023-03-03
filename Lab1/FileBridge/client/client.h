#ifndef CLIENT_H_
#define CLIENT_H_

#include "parse_args.h"
#include "file_operations.h"
#include "connection.h"
#include "request_trans.h"
#include "perform_trans.h"

#define SUCCESS 0
#define FAILURE 1

#define CHECK_FAILURE(TEMPLATE)                            \
    do                                                     \
    {                                                      \
        if (!operation_result.is_successful)               \
        {                                                  \
            printf(TEMPLATE, operation_result.error_info); \
            return FAILURE;                                \
        }                                                  \
    } while (false)

#define CHECK_FAILURE_C(TEMPLATE)                          \
    do                                                     \
    {                                                      \
        if (!operation_result.is_successful)               \
        {                                                  \
            printf(TEMPLATE, operation_result.error_info); \
            CloseConnection(server_socket);                \
            return FAILURE;                                \
        }                                                  \
    } while (false)

#define CHECK_FAILURE_C_F(TEMPLATE)                        \
    do                                                     \
    {                                                      \
        if (!operation_result.is_successful)               \
        {                                                  \
            printf(TEMPLATE, operation_result.error_info); \
            CloseConnection(server_socket);                \
            if (local_file != NULL)                        \
            {                                              \
                fclose(local_file);                        \
            }                                              \
            return FAILURE;                                \
        }                                                  \
    } while (false)

#endif