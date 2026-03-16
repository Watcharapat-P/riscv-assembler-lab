#include "registers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static const char *abi_names[32] = {
    "zero","ra","sp","gp","tp",
    "t0","t1","t2",
    "s0","s1",
    "a0","a1","a2","a3","a4","a5","a6","a7",
    "s2","s3","s4","s5","s6","s7","s8","s9","s10","s11",
    "t3","t4","t5","t6"
};

int parse_register(const char *name) {
    if (name == NULL) return -1;

    // fp is alias for s0 (x8)
    if (strcmp(name, "fp") == 0) return 8;

    // xN style
    if (name[0] == 'x') {
        int num;
        if (sscanf(name, "x%d", &num) != 1) return -1;
        if (num < 0 || num > 31) return -1;
        char expected[10];
        sprintf(expected, "x%d", num);
        if (strcmp(name, expected) != 0) return -1;
        return num;
    }

    // ABI names
    for (int i = 0; i < 32; i++) {
        if (strcmp(name, abi_names[i]) == 0) return i;
    }
    return -1;
}
