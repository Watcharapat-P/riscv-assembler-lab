#include "registers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int parse_register(const char *name) {
    if (name == NULL) return -1;
    int num;
    if (sscanf(name, "x%d", &num) != 1) return -1;
    if (num < 0 || num > 31) return -1;
    // Check if exactly "x%d" with no extra chars
    char expected[10];
    sprintf(expected, "x%d", num);
    if (strcmp(name, expected) != 0) return -1;
    return num;
}
