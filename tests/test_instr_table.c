#include <assert.h>
#include <stdio.h>
#include "../src/instr_table.h"

void test_lookup_instruction() {
    // Test valid instructions
    Instruction *instr = lookup_instruction("add");
    assert(instr != NULL);
    assert(instr->format == FMT_R);
    assert(instr->opcode == 0x33);
    assert(instr->func3 == 0x0);
    assert(instr->func7 == 0x00);
    
    instr = lookup_instruction("sub");
    assert(instr != NULL);
    assert(instr->format == FMT_R);
    assert(instr->opcode == 0x33);
    assert(instr->func3 == 0x0);
    assert(instr->func7 == 0x20);
    
    instr = lookup_instruction("lw");
    assert(instr != NULL);
    assert(instr->format == FMT_I);
    assert(instr->opcode == 0x03);
    assert(instr->func3 == 0x2);
    
    instr = lookup_instruction("beq");
    assert(instr != NULL);
    assert(instr->format == FMT_B);
    assert(instr->opcode == 0x63);
    assert(instr->func3 == 0x0);
    
    instr = lookup_instruction("lui");
    assert(instr != NULL);
    assert(instr->format == FMT_U);
    assert(instr->opcode == 0x37);
    
    instr = lookup_instruction("jal");
    assert(instr != NULL);
    assert(instr->format == FMT_J);
    assert(instr->opcode == 0x6f);
    
    // Test invalid instruction
    instr = lookup_instruction("invalid");
    assert(instr == NULL);
    
    // Test case sensitivity
    instr = lookup_instruction("ADD");
    assert(instr == NULL);
}

int main() {
    test_lookup_instruction();
    printf("All instruction table tests passed!\n");
    return 0;
}
