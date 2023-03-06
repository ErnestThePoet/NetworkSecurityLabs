#ifndef PACKETS_H_
#define PACKETS_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "file_bridge.h"

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
 * Client List Dir Request
 * TYPE=3
 * DATA Field:
 * 4B         Server dir path length in bytes
 * variable   Server dir path string
 *
 * Server Denial
 * TYPE=100
 * DATA Field:
 * 4B         Reason length in bytes
 * variable   Reason string
 *
 * Server Upload Permission
 * TYPE=101
 * DATA Field is empty
 *
 * Server Download Permission
 * TYPE=102
 * DATA Field:
 * 8B         File Size in bytes
 *
 * Server Upload Success
 * TYPE=103
 * DATA Field is empty
 *
 * Server Upload Failure
 * TYPE=104
 * DATA Field:
 * 4B         Reason length in bytes
 * variable   Reason string
 *
 * Server List Dir Result
 * TYPE=105
 * DATA Field:
 * 4B         List dir result length in bytes
 * variable   List dir result string
 *
 * *********************************************/

#define PACKET_HEADER_SIZE 9

#define PACKET_TYPE_CLIENT_UPLOAD_REQUEST 1
#define PACKET_TYPE_CLIENT_DOWNLOAD_REQUEST 2
#define PACKET_TYPE_CLIENT_LIST_DIR_REQUEST 3

#define PACKET_TYPE_SERVER_DENIAL 100
#define PACKET_TYPE_SERVER_UPLOAD_PERMISSION 101
#define PACKET_TYPE_SERVER_DOWNLOAD_PERMISSION 102
#define PACKET_TYPE_SERVER_UPLOAD_SUCCESS 103
#define PACKET_TYPE_SERVER_UPLOAD_FAILURE 104
#define PACKET_TYPE_SERVER_LIST_DIR_RESULT 105

char *MakeClientUploadRequestPacket(const char *server_file_path,
                                    const size_t file_size,
                                    size_t *packet_size_ret);

char *MakeClientDownloadRequestPacket(const char *server_file_path,
                                      size_t *packet_size_ret);

char *MakeClientListDirRequestPacket(const char *server_dir_path,
                                      size_t *packet_size_ret);

char *MakeServerDenialPacket(const char *reason,
                             size_t *packet_size_ret);

char *MakeServerUploadPermissionPacket(size_t *packet_size_ret);

char *MakeServerDownloadPermissionPacket(const size_t file_size,
                                        size_t *packet_size_ret);

char *MakeServerUploadSuccessPacket(size_t *packet_size_ret);

char *MakeServerUploadFailurePacket(const char *reason,
                                   size_t *packet_size_ret);

char *MakeServerListDirResultPacket(const char *list_dir_result,
                                    size_t *packet_size_ret);

uint8_t ExtractPacketHeader(const char *header,
                            size_t *packet_size_ret,
                            size_t *packet_data_size_ret);

void ReleasePacket(char *packet);

#endif