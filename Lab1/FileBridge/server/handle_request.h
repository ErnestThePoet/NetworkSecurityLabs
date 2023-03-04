#ifndef HANDLE_REQUEST_H_
#define HANDLE_REQUEST_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "../common/file_bridge.h"
#include "../common/operation_result.h"
#include "../common/packets.h"
#include "../common/read_socket.h"

#define FAILURE_RETURN_S(M)                        \
    do                                             \
    {                                              \
        printf("Request Handling Error: %s\n", M); \
        close(client_socket);                      \
        return NULL;                               \
    } while (false)

void *HandleRequest(void *client_socket_ptr);

#endif
