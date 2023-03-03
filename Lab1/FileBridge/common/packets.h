#ifndef PACKETS_H_
#define PACKETS_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/***********************************************
 *     PACKET FORMAT SPECS
 *
 * General packet format:
 *
 * ------------------------------------------
 * | TYPE | TOTAL_LENGTH |     DATA ...     |
 * ------------------------------------------
 *    1B         8B        (TOTAL_ENGTH-9)B
 *
 * DATA field format for each packet type:
 *
 * Client Upload Request
 * TYPE=1
 * DATA Field:
 * 4B         Server file path length in bytes
 * variable   Server file path string, without terminator '\0'
 * 8B         File Size in bytes
 * 32B        File SHA256
 *
 * Client Download Request
 * TYPE=2
 * DATA Field:
 * 4B         Server file path length in bytes
 * variable   Server file path string
 *
 * Server Denial
 * TYPE=100
 * DATA Field:
 * 4B         Reason length in bytes
 * 32B        Reason string
 *
 * Server Upload Acknowledgement
 * TYPE=101
 * DATA Field is empty
 *
 * Server Download Acknowledgement
 * TYPE=102
 * DATA Field:
 * 8B         File Size in bytes
 * 32B        File SHA256
 *
 * *********************************************/

#define PACKET_HEADER_SIZE                  9

#define PACKET_TYPE_CLIENT_UPLOAD_REQUEST   1
#define PACKET_TYPE_CLIENT_DOWNLOAD_REQUEST 2
#define PACKET_TYPE_SERVER_DENIAL           100
#define PACKET_TYPE_SERVER_UPLOAD_ACK       101
#define PACKET_TYPE_SERVER_DOWNLOAD_ACK     102

uint8_t *MakeClientUploadRequestPacket(const char *server_file_path,
                                       const uint64_t file_size,
                                       const uint8_t *sha256,
                                       uint64_t *packet_size_ret);

uint8_t *MakeClientDownloadRequestPacket(const char *server_file_path,
                                         uint64_t *packet_size_ret);

uint8_t *MakeServerDenialPacket(const char *reason,
                                uint64_t *packet_size_ret);

uint8_t *MakeServerUploadAckPacket(uint64_t *packet_size_ret);

uint8_t *MakeServerDownloadAckPacket(const uint64_t file_size,
                                     const uint8_t *sha256,
                                     uint64_t *packet_size_ret);

void ReleasePacket(uint8_t *packet);

#endif