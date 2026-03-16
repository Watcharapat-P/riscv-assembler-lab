#include "encode.h"

uint32_t encode_r(uint32_t opcode, uint32_t rd, uint32_t func3, uint32_t rs1, uint32_t rs2, uint32_t func7) {
    return (func7 << 25) | (rs2 << 20) | (rs1 << 15) | (func3 << 12) | (rd << 7) | opcode;
}

uint32_t encode_i(uint32_t opcode, uint32_t rd, uint32_t func3, uint32_t rs1, int32_t imm) {
    uint32_t imm12 = imm & 0xFFF;
    return (imm12 << 20) | (rs1 << 15) | (func3 << 12) | (rd << 7) | opcode;
}

uint32_t encode_s(uint32_t opcode, uint32_t func3, uint32_t rs1, uint32_t rs2, int32_t imm) {
    uint32_t imm5 = imm & 0x1F;
    uint32_t imm7 = (imm >> 5) & 0x7F;
    return (imm7 << 25) | (rs2 << 20) | (rs1 << 15) | (func3 << 12) | (imm5 << 7) | opcode;
}

uint32_t encode_b(uint32_t opcode, uint32_t func3, uint32_t rs1, uint32_t rs2, int32_t imm) {
    uint32_t imm12 = (imm >> 11) & 1;
    uint32_t imm11 = (imm >> 11) & 1;
    uint32_t imm10_5 = (imm >> 5) & 0x3F;
    uint32_t imm4_1 = (imm >> 1) & 0xF;
    return (imm12 << 31) | (imm10_5 << 25) | (rs2 << 20) | (rs1 << 15) | (func3 << 12) | (imm4_1 << 8) | (imm11 << 7) | opcode;
}

uint32_t encode_j(uint32_t opcode, uint32_t rd, int32_t imm) {
    uint32_t imm20 = (imm >> 20) & 1;
    uint32_t imm10_1 = (imm >> 1) & 0x3FF;
    uint32_t imm11 = (imm >> 11) & 1;
    uint32_t imm19_12 = (imm >> 12) & 0xFF;
    return (imm20 << 31) | (imm10_1 << 21) | (imm11 << 20) | (imm19_12 << 12) | (rd << 7) | opcode;
}
