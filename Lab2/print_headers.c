#include "print_headers.h"

static void GetCurrentTimeString(char *buffer, const size_t size)
{
        time_t current_time = time(NULL);
        struct tm *current_time_tuple = localtime(&current_time);
        strftime(buffer,
                 size,
                 "%Y/%m/%d %H:%M:%S",
                 current_time_tuple);
}

void PrintEthernetHeader(FILE *output_file, const EthernetHeader *header)
{
        char current_time_string[50];
        GetCurrentTimeString(current_time_string, sizeof(current_time_string));

        fprintf(output_file, "[%s] - [Ethernet Frame]\n  ", current_time_string);
        fprintf(output_file, "Source MAC Address: %02x-%02x-%02x-%02x-%02x-%02x\n  ",
                header->src_mac[0],
                header->src_mac[1],
                header->src_mac[2],
                header->src_mac[3],
                header->src_mac[4],
                header->src_mac[5]);
        fprintf(output_file, "Destination MAC Address: %02x-%02x-%02x-%02x-%02x-%02x\n  ",
                header->dest_mac[0],
                header->dest_mac[1],
                header->dest_mac[2],
                header->dest_mac[3],
                header->dest_mac[4],
                header->dest_mac[5]);
        fprintf(output_file, "Payload Type: %#06x\n", ntohs(header->type));
        fputc('\n', output_file);
}

void PrintIp4Header(FILE *output_file, const Ip4Header *header)
{
        char current_time_string[50];
        GetCurrentTimeString(current_time_string, sizeof(current_time_string));

        fprintf(output_file, "[%s] - [IPv4 Packet]\n  ", current_time_string);

        // it's worth noticing that inside a byte, the lowest-bit here
        // is the highest-bit shown in header diagram.
        fprintf(output_file, "Header Length: %d *4bytes\n  ", header->hlv & 0x0F);
        fprintf(output_file, "Version Number: %d\n  ", header->hlv >> 4);
        fprintf(output_file, "Type of Service: %d\n  ", header->tos);
        fprintf(output_file, "Total Length: %d bytes\n  ", ntohs(header->total_length));
        fprintf(output_file, "ID: %#06x\n  ", ntohs(header->id));
        uint16_t foff = ntohs(header->foff);
        fprintf(output_file, "Flags: %d %d %d\n  ",
                (foff & 0x8000U) >> 15,
                (foff & 0x4000U) >> 14,
                (foff & 0x2000U) >> 13);
        fprintf(output_file, "Fragment Offset: %d *8bytes\n  ", foff & 0x1FFFU);
        fprintf(output_file, "TTL: %d\n  ", header->ttl);
        fprintf(output_file, "Protocol: %#04x\n  ", header->protocol);
        fprintf(output_file, "Checksum: %#06x\n  ", ntohs(header->checksum));
        fprintf(output_file, "Source IPv4 Address: %d.%d.%d.%d\n  ",
                header->src_ip[0],
                header->src_ip[1],
                header->src_ip[2],
                header->src_ip[3]);
        fprintf(output_file, "Destination IPv4 Address: %d.%d.%d.%d\n  ",
                header->dest_ip[0],
                header->dest_ip[1],
                header->dest_ip[2],
                header->dest_ip[3]);
        fputc('\n', output_file);
}

void PrintTcpHeader(FILE *output_file, const TcpHeader *header)
{
        char current_time_string[50];
        GetCurrentTimeString(current_time_string, sizeof(current_time_string));

        fprintf(output_file, "[%s] - [TCP Segment]\n  ", current_time_string);

        fprintf(output_file, "Source Port: %d\n  ", ntohs(header->src_port));
        fprintf(output_file, "Destination Port: %d\n  ", ntohs(header->dest_port));
        fprintf(output_file, "Seq Number: %u\n  ", ntohl(header->seq));
        fprintf(output_file, "Ack Number: %u\n  ", ntohl(header->ack));
        fprintf(output_file, "Header Length: %d *4bytes\n  ", header->headlen >> 4);
        fprintf(output_file, "Flags: %#04x\n  ", header->flags);
        fprintf(output_file, "Window Size: %d\n  ", ntohs(header->window_size));
        fprintf(output_file, "Checksum: %#06x\n  ", ntohs(header->checksum));
        fprintf(output_file, "Urgent Pointer: %#06x\n  ", ntohs(header->urgent_pointer));
        fputc('\n', output_file);
}

void PrintUdpHeader(FILE *output_file, const UdpHeader *header)
{
        char current_time_string[50];
        GetCurrentTimeString(current_time_string, sizeof(current_time_string));

        fprintf(output_file, "[%s] - [UDP Segment]\n  ", current_time_string);

        fprintf(output_file, "Source Port: %d\n  ", ntohs(header->src_port));
        fprintf(output_file, "Destination Port: %d\n  ", ntohs(header->dest_port));
        fprintf(output_file, "Total Length: %d bytes\n  ", ntohs(header->total_length));
        fprintf(output_file, "Checksum: %#06x\n  ", ntohs(header->checksum));
        fputc('\n', output_file);
}