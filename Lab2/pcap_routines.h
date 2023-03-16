#ifndef PCAP_ROUTINES_H_
#define PCAP_ROUTINES_H_

#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include "defs.h"

#define CHECK_ERROR                   \
    do                                \
    {                                 \
        if (error_code == PCAP_ERROR) \
        {                             \
            exit(FAILURE);            \
        }                             \
    } while (0)

static char kErrorBuf[PCAP_ERRBUF_SIZE];

void Initialize();

#endif