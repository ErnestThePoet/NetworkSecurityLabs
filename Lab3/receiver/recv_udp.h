#ifndef RECV_UDP_H_
#define RECV_UDP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../common/defs.h"

void recvUDP(const int server_port, const char *recv_ip4);

#endif