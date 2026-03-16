#include "assembler.h"
#include "instr_table.h"
#include "encode.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Helper to parse register from string
int parse_reg(const char *s) {
    if (s && s[0] == 'x') {
        return atoi(s + 1);
    }
    return -1;
}

// Helper to parse immediate, may be number or label
int parse_imm(const char *s, SymbolTable *st) {
    if (!s) return 0;
    if ((s[0] >= '0' && s[0] <= '9') || s[0] == '-') {
        return atoi(s);
    } else {
        uint32_t addr = symbols_find(st, s);
        if (addr == (uint32_t)-1) return 0; // or error
        return addr;
    }
}

// Helper to parse memory operand "imm(reg)"
int parse_mem(const char *s, int *imm, int *reg) {
    if (!s) return 0;
    char *copy = strdup(s);
    char *paren = strchr(copy, '(');
    if (!paren) {
        free(copy);
        return 0;
    }
    *paren = '\0';
    *imm = atoi(copy);
    char *r = paren + 1;
    if (r[strlen(r) - 1] == ')') r[strlen(r) - 1] = '\0';
    *reg = parse_reg(r);
    free(copy);
    return 1;
}

int assembler_assemble(const char *assembly, SymbolTable *st, uint32_t **output, int *num_instr) {
    // Split assembly into lines
    char *copy = strdup(assembly);
    if (!copy) return -1;
    char *lines_str[100];
    int num_lines = 0;
    char *line = strtok(copy, "\n");
    while (line && num_lines < 100) {
        lines_str[num_lines++] = line;
        line = strtok(NULL, "\n");
    }

    // Parse lines
    ParsedLine *lines[100];
    for (int i = 0; i < num_lines; i++) {
        lines[i] = parse_line(lines_str[i]);
    }

    // Pass 1: record labels
    uint32_t address = 0;
    for (int i = 0; i < num_lines; i++) {
        if (lines[i] && lines[i]->label) {
            symbols_add(st, lines[i]->label, address);
        }
        if (lines[i] && lines[i]->mnemonic) {
            address += 4;
        }
    }

    // Pass 2: encode
    *num_instr = 0;
    *output = malloc(sizeof(uint32_t) * num_lines);
    if (!*output) {
        free(copy);
        for (int i = 0; i < num_lines; i++) free_parsed_line(lines[i]);
        return -1;
    }
    uint32_t current_address = 0;
    for (int i = 0; i < num_lines; i++) {
        if (lines[i] && lines[i]->mnemonic) {
            Instruction *instr = lookup_instruction(lines[i]->mnemonic);
            if (!instr) continue;
            uint32_t code = 0;
            if (instr->format == FMT_R && lines[i]->num_operands == 3) {
                int rd = parse_reg(lines[i]->operands[0]);
                int rs1 = parse_reg(lines[i]->operands[1]);
                int rs2 = parse_reg(lines[i]->operands[2]);
                code = encode_r(instr->opcode, rd, instr->func3, rs1, rs2, instr->func7);
            } else if (instr->format == FMT_I && lines[i]->num_operands == 3) {
                int rd = parse_reg(lines[i]->operands[0]);
                int rs1 = parse_reg(lines[i]->operands[1]);
                int imm = parse_imm(lines[i]->operands[2], st);
                code = encode_i(instr->opcode, rd, instr->func3, rs1, imm);
            } else if (instr->format == FMT_I && lines[i]->num_operands == 2) {
                // load
                int rd = parse_reg(lines[i]->operands[0]);
                int imm, rs1;
                parse_mem(lines[i]->operands[1], &imm, &rs1);
                code = encode_i(instr->opcode, rd, instr->func3, rs1, imm);
            } else if (instr->format == FMT_S && lines[i]->num_operands == 2) {
                int rs2 = parse_reg(lines[i]->operands[0]);
                int imm, rs1;
                if (parse_mem(lines[i]->operands[1], &imm, &rs1)) {
                    code = encode_s(instr->opcode, instr->func3, rs1, rs2, imm);
                }
            } else if (instr->format == FMT_B && lines[i]->num_operands == 3) {
                int rs1 = parse_reg(lines[i]->operands[0]);
                int rs2 = parse_reg(lines[i]->operands[1]);
                uint32_t target = symbols_find(st, lines[i]->operands[2]);
                int imm = ((int)target - (int)current_address) / 2;
                code = encode_b(instr->opcode, instr->func3, rs1, rs2, imm);
            } else if (instr->format == FMT_J && lines[i]->num_operands == 2) {
                int rd = parse_reg(lines[i]->operands[0]);
                uint32_t target = symbols_find(st, lines[i]->operands[1]);
                int imm = ((int)target - (int)current_address) / 2;
                code = encode_j(instr->opcode, rd, imm);
            }
            (*output)[(*num_instr)++] = code;
            current_address += 4;
        }
    }

    // Free
    for (int i = 0; i < num_lines; i++) {
        free_parsed_line(lines[i]);
    }
    free(copy);
    return 0;
}
