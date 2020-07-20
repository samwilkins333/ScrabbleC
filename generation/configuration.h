#pragma once

#define DIMENSIONS 15
#define RACK_CAPACITY 7
#define DELIMITER '{'
#define BLANK '{'

#define NEW(type, var, multiple)\
    type *var = (type *)malloc(multiple * sizeof(type));\
    if (!var) {\
        printf("Out of memory.\n");\
        exit(1);\
    }\
    memset(var, 0, multiple * sizeof(type));\
    allocations++;

#define DONE(allocated)\
    free(allocated);\
    allocations--;