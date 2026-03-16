#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/assembler.h"

void test_assembler() {
    SymbolTable st;
    symbols_init(&st);
    uint32_t *output;
    int num_instr;

    // Test simple assembly
    const char *asm_code = "add x1, x2, x3\nlw x1, 8(x2)";
    int ret = assembler_assemble(asm_code, &st, &output, &num_instr);
    assert(ret == 0);
    assert(num_instr == 2);
    assert(output[0] == 0x003100B3); // add x1, x2, x3
    assert(output[1] == 0x00812083); // lw x1, 8(x2)
    free(output);

    // Test with label
    symbols_init(&st); // reset
    const char *asm_code2 = "loop: add x1, x2, x3\nbeq x1, x2, loop";
    ret = assembler_assemble(asm_code2, &st, &output, &num_instr);
    assert(ret == 0);
    assert(num_instr == 2);
    assert(output[0] == 0x003100B3); // add
    // beq x1, x2, loop (imm = (0 - 4) / 2 = -2)
    assert(output[1] == 0xFE220FE3); // beq x1, x2, -2
    free(output);
}

int main() {
    test_assembler();
    printf("All assembler tests passed!\n");
    return 0;
}
