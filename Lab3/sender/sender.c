#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <libnet.h>

#define SUCCESS 0
#define FAILURE 1

#define CLEAN_RETURN_FAILURE    \
    do                          \
    {                           \
        libnet_destroy(handle); \
        return FAILURE;         \
    } while (false)

bool ParseArgs(int argc, char *argv[], int *dest_port_ret)
{
    if (argc != 3)
    {
        return false;
    }

    char *dest_port_end = NULL;
    int dest_port = strtol(argv[2], &dest_port_end, 10);
    if (dest_port_end == argv[2] || dest_port > 65535 || dest_port < 0)
    {
        fputs("Invalid port\n", stderr);
        return false;
    }

    *dest_port_ret = dest_port;

    return true;
}

int main(int argc, char *argv[])
{
    const char *const kUsagePrompt = "Usage: sender <dest-ip> <dest-port>\n";
    const char kUdpPayload[] = "HELLO WORLD from Ernest Cui";

    int dest_port = 0;
    if (!ParseArgs(argc, argv, &dest_port))
    {
        fputs(kUsagePrompt, stderr);
        return FAILURE;
    }

    char kErrorBuf[LIBNET_ERRBUF_SIZE];

    libnet_t *handle = libnet_init(LIBNET_RAW4, "ens33", kErrorBuf);
    if (handle == NULL)
    {
        fprintf(stderr, "libnet_init() failed: %s\n", kErrorBuf);
        return FAILURE;
    }

    uint32_t src_ip = libnet_get_ipaddr4(handle);
    uint32_t dest_ip = libnet_name2addr4(handle, argv[1], LIBNET_DONT_RESOLVE);

    if (src_ip == -1)
    {
        fputs("Failed to get source IPv4 address\n", stderr);
        CLEAN_RETURN_FAILURE;
    }

    if (dest_ip == -1)
    {
        fputs("Failed to convert destination IPv4 address\n", stderr);
        CLEAN_RETURN_FAILURE;
    }

    const int udp_payload_size = sizeof(kUdpPayload);
    const int udp_packet_size = LIBNET_UDP_H + udp_payload_size;
    const int ip_packet_size = LIBNET_IPV4_H + udp_packet_size;

    libnet_ptag_t udp_tag = libnet_build_udp(
        31100,
        dest_port,
        udp_packet_size,
        0,
        (const uint8_t *)kUdpPayload,
        udp_payload_size,
        handle,
        0);

    if (udp_tag == -1)
    {
        fprintf(stderr, "libnet_build_udp() error: %s\n", libnet_geterror(handle));
        CLEAN_RETURN_FAILURE;
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
        CLEAN_RETURN_FAILURE;
    }

    int written_byte_count = libnet_write(handle);
    if (written_byte_count == -1)
    {
        fprintf(stderr, "libnet_write() error: %s\n", libnet_geterror(handle));
        CLEAN_RETURN_FAILURE;
    }

    libnet_destroy(handle);

    printf("Successfully sent %d bytes to %s:%s via UDP\n", udp_payload_size, argv[1], argv[2]);

    return SUCCESS;
}