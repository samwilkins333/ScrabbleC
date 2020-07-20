#pragma once

#define DIMENSIONS 15
#define RACK_CAPACITY 7
#define DELIMITER '{'
#define BLANK '{'

static int allocations = 0;

#define NEW(type, var, multiple)\
    type *var = (type *)malloc(multiple * sizeof(type));\
    if (!var) {\
        exit(1);\
    }\
    memset(var, 0, multiple * sizeof(type));\
    allocations++;