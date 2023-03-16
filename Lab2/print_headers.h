#ifndef PRINT_HEADERS_H_
#define PRINT_HEADERS_H_

#include <stdio.h>
#include <arpa/inet.h>
#include "protocol_headers.h"

void PrintEthernetHeader(FILE *output_file, const EthernetHeader *header);
void PrintIp4Header(FILE *output_file, const Ip4Header *header);
void PrintTcpHeader(FILE *output_file, const TcpHeader *header);
void PrintUdpHeader(FILE *output_file, const UdpHeader *header);

#endif