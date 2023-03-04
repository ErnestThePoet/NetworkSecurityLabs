#include "read_socket.h"

bool ReadSocket(const int socket, char *buffer, const size_t target_receive_size)
{
    size_t received_size = 0;

    while (received_size < target_receive_size)
    {
        size_t remaining_receive_size = target_receive_size - received_size;
        ssize_t current_receive_size = recv(
            socket, buffer + received_size, remaining_receive_size, 0);
        if (current_receive_size == -1)
        {
            return false;
        }

        received_size += current_receive_size;
    }

    return true;
}