#include "instr_table.h"
#include <string.h>

static Instruction instructions[] = {
    // R-type
    {"add", FMT_R, 0x33, 0x0, 0x00},
    {"sub", FMT_R, 0x33, 0x0, 0x20},
    {"sll", FMT_R, 0x33, 0x1, 0x00},
    {"slt", FMT_R, 0x33, 0x2, 0x00},
    {"sltu", FMT_R, 0x33, 0x3, 0x00},
    {"xor", FMT_R, 0x33, 0x4, 0x00},
    {"srl", FMT_R, 0x33, 0x5, 0x00},
    {"sra", FMT_R, 0x33, 0x5, 0x20},
    {"or", FMT_R, 0x33, 0x6, 0x00},
    {"and", FMT_R, 0x33, 0x7, 0x00},
    
    // I-type loads
    {"lb", FMT_I, 0x03, 0x0, 0},
    {"lh", FMT_I, 0x03, 0x1, 0},
    {"lw", FMT_I, 0x03, 0x2, 0},
    {"lbu", FMT_I, 0x03, 0x4, 0},
    {"lhu", FMT_I, 0x03, 0x5, 0},
    
    // I-type arithmetic
    {"addi", FMT_I, 0x13, 0x0, 0},
    {"slti", FMT_I, 0x13, 0x2, 0},
    {"sltiu", FMT_I, 0x13, 0x3, 0},
    {"xori", FMT_I, 0x13, 0x4, 0},
    {"ori", FMT_I, 0x13, 0x6, 0},
    {"andi", FMT_I, 0x13, 0x7, 0},
    {"slli", FMT_I, 0x13, 0x1, 0x00},
    {"srli", FMT_I, 0x13, 0x5, 0x00},
    {"srai", FMT_I, 0x13, 0x5, 0x20},
    {"jalr", FMT_I, 0x67, 0x0, 0},
    
    // S-type
    {"sb", FMT_S, 0x23, 0x0, 0},
    {"sh", FMT_S, 0x23, 0x1, 0},
    {"sw", FMT_S, 0x23, 0x2, 0},
    
    // B-type
    {"beq", FMT_B, 0x63, 0x0, 0},
    {"bne", FMT_B, 0x63, 0x1, 0},
    {"blt", FMT_B, 0x63, 0x4, 0},
    {"bge", FMT_B, 0x63, 0x5, 0},
    {"bltu", FMT_B, 0x63, 0x6, 0},
    {"bgeu", FMT_B, 0x63, 0x7, 0},
    
    // U-type
    {"lui", FMT_U, 0x37, 0, 0},
    {"auipc", FMT_U, 0x17, 0, 0},
    
    // J-type
    {"jal", FMT_J, 0x6f, 0, 0},
    {"jal",   FMT_J, 0x6F, 0, 0},
    {"jalr",  FMT_I, 0x67, 0, 0},
    {"lui",   FMT_U, 0x37, 0, 0},
    {"auipc", FMT_U, 0x17, 0, 0},
    
    // System
    {"ecall", FMT_I, 0x73, 0x0, 0},
    {"ebreak", FMT_I, 0x73, 0x0, 0x00000001}, // func7 for ebreak is 1, but usually imm=1
    
    {NULL, 0, 0, 0, 0} // sentinel
};

Instruction *lookup_instruction(const char *mnemonic) {
    for (int i = 0; instructions[i].mnemonic != NULL; i++) {
        if (strcmp(instructions[i].mnemonic, mnemonic) == 0) {
            return &instructions[i];
        }
    }
    return NULL;
}
