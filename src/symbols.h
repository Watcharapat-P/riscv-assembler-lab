#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdint.h>

#define MAX_SYMBOLS 100

typedef struct {
    char name[32];
    uint32_t value;
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
} SymbolTable;

void symbols_init(SymbolTable *st);
int symbols_add(SymbolTable *st, const char *name, uint32_t value);
uint32_t symbols_find(SymbolTable *st, const char *name);

#endif
