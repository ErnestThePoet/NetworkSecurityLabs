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
 * 4B         Server file path length in bytes including '\0'
 * variable   Server file path string, with terminator '\0'
 * 8B         File Size in bytes
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
 * variable   Reason string
 *
 * Server Upload Permitted
 * TYPE=101
 * DATA Field is empty
 *
 * Server Download Permitted
 * TYPE=102
 * DATA Field:
 * 8B         File Size in bytes
 *
 * Server Upload Successful
 * TYPE=103
 * DATA Field is empty
 *
 * Server Upload Failed
 * TYPE=104
 * DATA Field:
 * 4B         Reason length in bytes
 * variable   Reason string
 *
 * *********************************************/

#define PACKET_HEADER_SIZE 9

#define PACKET_TYPE_CLIENT_UPLOAD_REQUEST 1
#define PACKET_TYPE_CLIENT_DOWNLOAD_REQUEST 2
#define PACKET_TYPE_SERVER_DENIAL 100
#define PACKET_TYPE_SERVER_UPLOAD_PERMITTED 101
#define PACKET_TYPE_SERVER_DOWNLOAD_PERMITTED 102
#define PACKET_TYPE_SERVER_UPLOAD_SUCCESSFUL 103
#define PACKET_TYPE_SERVER_UPLOAD_FAILED 104

char *MakeClientUploadRequestPacket(const char *server_file_path,
                                    const size_t file_size,
                                    size_t *packet_size_ret);

char *MakeClientDownloadRequestPacket(const char *server_file_path,
                                      size_t *packet_size_ret);

char *MakeServerDenialPacket(const char *reason,
                             size_t *packet_size_ret);

char *MakeServerUploadPermittedPacket(size_t *packet_size_ret);

char *MakeServerDownloadPermittedPacket(const size_t file_size,
                                        size_t *packet_size_ret);

char *MakeServerUploadSuccessfulPacket(size_t *packet_size_ret);

char *MakeServerUploadFailedPacket(const char *reason,
                                   size_t *packet_size_ret);

void ReleasePacket(const char *packet);

#endif