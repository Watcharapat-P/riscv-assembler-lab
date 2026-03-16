#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "symbols.h"
#include <stdint.h>

int assembler_assemble(const char *assembly, SymbolTable *st, uint32_t **output, int *num_instr);

#endif
