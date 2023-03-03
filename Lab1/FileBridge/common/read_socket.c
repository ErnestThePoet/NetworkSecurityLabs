#include "read_socket.h"

bool ReadSocket(const int socket, const size_t target_receive_size, char *buffer){
    size_t receive_size = 0;

    while (receive_size < target_receive_size)
    {
        size_t remaining_receive_size = target_receive_size - receive_size;
        ssize_t current_receive_size = recv(
            socket, buffer + receive_size, remaining_receive_size, 0);
        if (current_receive_size == -1)
        {
            return false;
        }

        receive_size += current_receive_size;
    }

    return true;
}