#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stdbool.h>

typedef enum{
    INT,
    CHAR,
    STRING,
    FLOAT,
    BOOL
} arg_type;

typedef union arg_var{
    int i;
    char c;
    float f;
    double d;
    char *s;
};

typedef struct{
    char short_name[2];
    char long_name[16];
    bool has_arg;
    bool has_multi_arg;
    arg_type type;
    arg_var value;
} argparse;

#endif