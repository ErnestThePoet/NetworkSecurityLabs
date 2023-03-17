#ifndef PRINT_HEADERS_H_
#define PRINT_HEADERS_H_

#include <stdio.h>
#include <time.h>
#include <arpa/inet.h>
#include "protocol_headers.h"

static void GetCurrentTimeString(char *buffer, const size_t size);
void PrintEthernetHeader(FILE *output_file, const EthernetHeader *header);
void PrintIp4Header(FILE *output_file, const Ip4Header *header);
void PrintTcpHeader(FILE *output_file, const Ip4Header *ip4_header, const TcpHeader *header);
void PrintUdpHeader(FILE *output_file, const Ip4Header *ip4_header, const UdpHeader *header);

#endif