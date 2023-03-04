#ifndef READ_SOCKET_H_
#define READ_SOCKET_H_

#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>

bool ReadSocket(const int socket, char *buffer, const size_t target_read_size);

#endif