#ifndef INSTR_TABLE_H
#define INSTR_TABLE_H

#include <stdint.h>

typedef enum {
    FMT_R,
    FMT_I,
    FMT_S,
    FMT_B,
    FMT_U,
    FMT_J
} InstructionFormat;

typedef struct {
    const char *mnemonic;
    InstructionFormat format;
    uint32_t opcode;
    uint32_t func3;
    uint32_t func7;
} Instruction;

Instruction *lookup_instruction(const char *mnemonic);

#endif
