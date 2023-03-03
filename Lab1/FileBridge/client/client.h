#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>

#include "arg_parse_result.h"
#include "../common/sha256.h"

ArgParseResult ParseArgs(int argc, char **argv);
