#include <stdbool.h>
#include "client_arg.h"

typedef struct{
    bool is_valid;
    const char *error_info;
    ClientArg client_arg;
} ArgParseResult;