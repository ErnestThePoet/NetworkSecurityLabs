#ifndef PCAP_ROUTINES_H_
#define PCAP_ROUTINES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pcap/pcap.h>
#include "defs.h"

#define CHECK_ERROR                   \
    do                                \
    {                                 \
        if (error_code == PCAP_ERROR) \
        {                             \
            puts(kErrorBuf);          \
            exit(FAILURE);            \
        }                             \
    } while (0)

static char kErrorBuf[PCAP_ERRBUF_SIZE];

void Initialize();
int PrintDeviceList(pcap_if_t **device_list_ret);
static void GetAddressString(
    char *buffer, const size_t maxlen, const struct sockaddr *address);

#endif