#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <assembly_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *assembly = malloc(length + 1);
    if (!assembly) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }

    fread(assembly, 1, length, file);
    assembly[length] = '\0';
    fclose(file);

    SymbolTable st;
    symbols_init(&st);
    uint32_t *output;
    int num_instr;

    int ret = assembler_assemble(assembly, &st, &output, &num_instr);
    if (ret != 0) {
        fprintf(stderr, "Assembly failed\n");
        free(assembly);
        return 1;
    }

    printf("Assembled %d instructions:\n", num_instr);
    for (int i = 0; i < num_instr; i++) {
        printf("0x%08X\n", output[i]);
    }

    free(output);
    free(assembly);
    return 0;
}
