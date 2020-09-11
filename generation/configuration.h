#pragma once

#include <ntsid.h>

#define DIMENSIONS 15
#define RACK_CAPACITY 7
#define BINGO 50
#define DELIMITER '{'
#define BLANK '{'
#define DICTIONARY "../ospd4.txt"

size_t malloc_size;

#define NEW(type, var, multiple)\
    malloc_size = multiple * sizeof(type);\
    type *var = (type *)malloc(malloc_size);\
    if (!var) {\
        printf("Out of memory.\n");\
        exit(1);\
    }\
    memset(var, 0, malloc_size);\
    allocations++;

#define DONE(allocated)\
    free(allocated);\
    allocations--;