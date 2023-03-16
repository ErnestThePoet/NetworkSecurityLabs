#ifndef PACKET_HEADERS_H_
#define PACKET_HEADERS_H_

#include <stdint.h>

#define ETHERNET_HEADER_LENGTH 14

#define ETHERNET_PAYLOAD_TYPE_IP4 0x0800

#define IP4_PAYLOAD_TYPE_TCP 0x06
#define IP4_PAYLOAD_TYPE_UDP 0x11

typedef struct
{
    uint8_t dest_mac[6];
    uint8_t src_mac[6];
    uint16_t type;
} EthernetHeader;

typedef struct
{
    uint8_t hlv;
    uint8_t tos;
    uint16_t total_length;
    uint16_t id;
    uint16_t foff;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint8_t src_ip[4];
    uint8_t dest_ip[4];
} Ip4Header;

typedef struct
{
    uint16_t src_port;
    uint16_t dest_port;
    uint32_t seq;
    uint32_t ack;
    uint8_t headlen;
    uint8_t flags;
    uint16_t window_size;
    uint16_t checksum;
    uint16_t urgent_pointer;
} TcpHeader;

typedef struct
{
    uint16_t src_port;
    uint16_t dest_port;
    uint16_t total_length;
    uint16_t checksum;
} UdpHeader;

#endif