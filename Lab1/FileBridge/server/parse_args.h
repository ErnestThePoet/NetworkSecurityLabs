#ifndef PARSE_ARGS_H_
#define PARSE_ARGS_H_

#include <stdlib.h>
#include <string.h>

#include "../common/operation_result.h"

OperationResult ParseArgs(
    const int argc, char *argv[], int *port_ret);

#endif