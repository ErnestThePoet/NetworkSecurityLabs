#ifndef CREATE_PAC_H_
#define CREATE_PAC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <libnet.h>

#define SUCCESS 0
#define FAILURE 1

#define CLEAN_EXIT_FAILURE      \
    do                          \
    {                           \
        libnet_destroy(handle); \
        exit(FAILURE);          \
    } while (false)

void createPac(const char *dest_ip4, const int dest_port, const char *udp_payload);

#endif