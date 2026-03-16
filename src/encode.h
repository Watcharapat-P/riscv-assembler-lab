#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>

uint32_t encode_r(uint32_t opcode, uint32_t rd, uint32_t func3, uint32_t rs1, uint32_t rs2, uint32_t func7);
uint32_t encode_i(uint32_t opcode, uint32_t rd, uint32_t func3, uint32_t rs1, int32_t imm);
uint32_t encode_s(uint32_t opcode, uint32_t func3, uint32_t rs1, uint32_t rs2, int32_t imm);
uint32_t encode_b(uint32_t opcode, uint32_t func3, uint32_t rs1, uint32_t rs2, int32_t imm);
uint32_t encode_j(uint32_t opcode, uint32_t rd, int32_t imm);

#endif
