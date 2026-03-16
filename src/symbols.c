#include "symbols.h"
#include <string.h>

void symbols_init(SymbolTable *st) {
    st->count = 0;
}

int symbols_add(SymbolTable *st, const char *name, uint32_t value) {
    // Check if already exists
    for (int i = 0; i < st->count; i++) {
        if (strcmp(st->symbols[i].name, name) == 0) {
            st->symbols[i].value = value;
            return 1; // updated
        }
    }
    // Add new
    if (st->count >= MAX_SYMBOLS) {
        return 0; // failed
    }
    strcpy(st->symbols[st->count].name, name);
    st->symbols[st->count].value = value;
    st->count++;
    return 1; // added
}

uint32_t symbols_find(SymbolTable *st, const char *name) {
    for (int i = 0; i < st->count; i++) {
        if (strcmp(st->symbols[i].name, name) == 0) {
            return st->symbols[i].value;
        }
    }
    return (uint32_t)-1; // not found
}
