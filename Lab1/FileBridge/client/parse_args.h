#ifndef PARSE_ARGS_H_
#define PARSE_ARGS_H_

#include <stdlib.h>
#include <string.h>

#include "../common/operation_result.h"
#include "client_arg.h"

OperationResult ParseArgs(int argc, char *argv[], ClientArg *client_arg_ret);

#endif