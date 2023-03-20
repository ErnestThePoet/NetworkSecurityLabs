#include "create_pac.h"

void createPac(const char *dest_ip4, const int dest_port, const char *udp_payload)
{
    char kErrorBuf[LIBNET_ERRBUF_SIZE];

    libnet_t *handle = libnet_init(LIBNET_RAW4, "ens33", kErrorBuf);
    if (handle == NULL)
    {
        fprintf(stderr, "libnet_init() failed: %s\n", kErrorBuf);
        exit(FAILURE);
    }

    uint32_t src_ip = libnet_get_ipaddr4(handle);
    uint32_t dest_ip = libnet_name2addr4(handle, dest_ip4, LIBNET_DONT_RESOLVE);

    if (src_ip == -1)
    {
        fputs("Failed to get source IPv4 address\n", stderr);
        CLEAN_EXIT_FAILURE;
    }

    if (dest_ip == -1)
    {
        fputs("Failed to convert destination IPv4 address\n", stderr);
        CLEAN_EXIT_FAILURE;
    }

    const int udp_payload_size = strlen(udp_payload) + 1;
    const int udp_packet_size = LIBNET_UDP_H + udp_payload_size;
    const int ip_packet_size = LIBNET_IPV4_H + udp_packet_size;

    libnet_ptag_t udp_tag = libnet_build_udp(
        31100,
        dest_port,
        udp_packet_size,
        0,
        (const uint8_t *)udp_payload,
        udp_payload_size,
        handle,
        0);

    if (udp_tag == -1)
    {
        fprintf(stderr, "libnet_build_udp() error: %s\n", libnet_geterror(handle));
        CLEAN_EXIT_FAILURE;
    }

    libnet_ptag_t ip4_tag = libnet_build_ipv4(
        ip_packet_size,
        0,
        libnet_get_prand(LIBNET_PRu16),
        0,
        64,
        IPPROTO_UDP,
        0,
        src_ip,
        dest_ip,
        NULL,
        0,
        handle,
        0);

    if (ip4_tag == -1)
    {
        fprintf(stderr, "libnet_build_ipv4() error: %s\n", libnet_geterror(handle));
        CLEAN_EXIT_FAILURE;
    }

    int written_byte_count = libnet_write(handle);
    if (written_byte_count == -1)
    {
        fprintf(stderr, "libnet_write() error: %s\n", libnet_geterror(handle));
        CLEAN_EXIT_FAILURE;
    }

    libnet_destroy(handle);

    printf("Successfully sent %d bytes to %s:%d via UDP\n",
           udp_payload_size,
           dest_ip4,
           dest_port);
}