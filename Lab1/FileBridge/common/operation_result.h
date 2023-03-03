#ifndef OPERATION_RESULT_H_
#define OPERATION_RESULT_H_

#include <stdbool.h>

typedef struct{
    bool is_successful;
    char error_info[200];
} OperationResult;

#endif