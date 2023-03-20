#ifndef PARSE_ARGS_H_
#define PARSE_ARGS_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool ParseArgs(const int argc, const char *argv[], int *port_ret);

#endif