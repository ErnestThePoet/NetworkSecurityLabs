#ifndef PCAP_ROUTINES_H_
#define PCAP_ROUTINES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include "defs.h"
#include "protocol_headers.h"
#include "print_headers.h"

#define CHECK_ERROR          \
    do                       \
    {                        \
        if (error_code)      \
        {                    \
            puts(kErrorBuf); \
            exit(FAILURE);   \
        }                    \
    } while (false)

static char kErrorBuf[PCAP_ERRBUF_SIZE];

static void GetAddressString(
    char *buffer, const size_t maxlen, const struct sockaddr *address);
static void ClearStdInBuffer();
static void HandlePacket(
    uint8_t *user, const struct pcap_pkthdr *h, const uint8_t *bytes);

void Initialize();
int PrintDeviceList(pcap_if_t **device_list_ret);
int GetUserSelectedDeviceIndex(const int device_count);
pcap_t *GetCaptureHandle(pcap_if_t *device_list, const int device_index);
void SetFilter(pcap_t *capture_handle, const char *filter);
FILE *PrepareOutputFile(pcap_t *capture_handle, const char *filter);
void StartCaptureLoop(pcap_t *capture_handle, FILE *output_file);
void CleanUp(pcap_t *capture_handle, FILE *output_file);

#endif