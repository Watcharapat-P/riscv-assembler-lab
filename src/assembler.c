#include "assembler.h"
#include "instr_table.h"
#include "encode.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Use parse_register() from registers.c which handles both xN and ABI names */
#include "registers.h"
#define parse_reg parse_register

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
    char *lines_str[4096];
    int num_lines = 0;
    char *line = strtok(copy, "\n");
    while (line && num_lines < 4096) {
        lines_str[num_lines++] = line;
        line = strtok(NULL, "\n");
    }

    // Parse lines
    ParsedLine *lines[4096];
    for (int i = 0; i < num_lines; i++) {
        lines[i] = parse_line(lines_str[i]);
    }

    // Pass 1: record labels
    // Two-segment model matching Venus: .text starts at 0x00, .data starts at 0x10000000
    uint32_t text_address = 0;
    uint32_t data_address = 0x10000000;
    int in_data = 0;  // 0 = .text, 1 = .data
    for (int i = 0; i < num_lines; i++) {
        if (!lines[i]) continue;
        const char *mn = lines[i]->mnemonic; /* may be NULL for label-only lines */

        /* Section switches */
        if (mn && strcmp(mn, ".data") == 0) { in_data = 1; continue; }
        if (mn && strcmp(mn, ".text") == 0) { in_data = 0; continue; }
        if (mn && (strcmp(mn, ".globl") == 0 || strcmp(mn, ".global") == 0)) continue;

        if (in_data) {
            /* Record data label at current data address */
            if (lines[i]->label) symbols_add(st, lines[i]->label, data_address);
            if (!mn) continue;
            if (strcmp(mn, ".space") == 0 || strcmp(mn, ".zero") == 0) {
                int n = lines[i]->num_operands > 0 ? atoi(lines[i]->operands[0]) : 0;
                data_address += (uint32_t)n;
            } else if (strcmp(mn, ".word") == 0) {
                data_address += 4 * (lines[i]->num_operands > 0 ? lines[i]->num_operands : 1);
            } else if (strcmp(mn, ".half") == 0) {
                data_address += 2 * (lines[i]->num_operands > 0 ? lines[i]->num_operands : 1);
            } else if (strcmp(mn, ".byte") == 0) {
                data_address += 1 * (lines[i]->num_operands > 0 ? lines[i]->num_operands : 1);
            }
        } else {
            /* Text: record label FIRST so label-only lines (equal:, end:) are not skipped */
            if (lines[i]->label) symbols_add(st, lines[i]->label, text_address);
            if (!mn || mn[0] == '.') continue;  /* label-only or directive: no PC advance */

            /* la = auipc + addi = 8 bytes; all others = 4 */
            text_address += (strcmp(mn, "la") == 0) ? 8 : 4;
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
            const char *mn = lines[i]->mnemonic;

            // Skip all assembler directives in pass 2
            if (mn[0] == '.') continue;

            // Handle la pseudo-instruction: expands to auipc rd, hi + addi rd, rd, lo
            if (strcmp(mn, "la") == 0 && lines[i]->num_operands == 2) {
                int rd = parse_reg(lines[i]->operands[0]);
                uint32_t sym_addr = symbols_find(st, lines[i]->operands[1]);
                if (sym_addr == (uint32_t)-1) sym_addr = 0;
                int32_t offset = (int32_t)sym_addr - (int32_t)current_address;
                int32_t hi = (offset + 0x800) >> 12;   // upper 20 bits (rounded)
                int32_t lo = offset - (hi << 12);       // lower 12 bits
                // auipc rd, hi
                (*output)[(*num_instr)++] = ((uint32_t)hi << 12) | ((uint32_t)rd << 7) | 0x17;
                current_address += 4;
                // addi rd, rd, lo
                (*output)[(*num_instr)++] = encode_i(0x13, rd, 0, rd, lo);
                current_address += 4;
                continue;
            }

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
                // slli/srli/srai encode funct7 in imm[11:5] and shamt in imm[4:0]
                if (instr->opcode == 0x13 && (instr->func3 == 0x1 || instr->func3 == 0x5)) {
                    imm = (int)(((instr->func7 & 0x7F) << 5) | (imm & 0x1F));
                }
                code = encode_i(instr->opcode, rd, instr->func3, rs1, imm);
            } else if (instr->format == FMT_I && lines[i]->num_operands == 2) {
                // load: rd, offset(rs1)
                int rd = parse_reg(lines[i]->operands[0]);
                int imm, rs1;
                parse_mem(lines[i]->operands[1], &imm, &rs1);
                code = encode_i(instr->opcode, rd, instr->func3, rs1, imm);
            } else if (instr->format == FMT_I && lines[i]->num_operands == 0) {
                // ecall / ebreak: all fields zero except opcode (imm=0)
                code = encode_i(instr->opcode, 0, instr->func3, 0, (int)instr->func7);
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
                int imm = (int)target - (int)current_address;  // byte offset
                code = encode_b(instr->opcode, instr->func3, rs1, rs2, imm);
            } else if (instr->format == FMT_J && lines[i]->num_operands == 2) {
                int rd = parse_reg(lines[i]->operands[0]);
                uint32_t target = symbols_find(st, lines[i]->operands[1]);
                int imm = (int)target - (int)current_address;  // byte offset
                code = encode_j(instr->opcode, rd, imm);
            } else if (instr->format == FMT_J && lines[i]->num_operands == 1) {
                // jal label  (implied rd = x1)
                uint32_t target = symbols_find(st, lines[i]->operands[0]);
                int imm = (int)target - (int)current_address;
                code = encode_j(instr->opcode, 1, imm);
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
