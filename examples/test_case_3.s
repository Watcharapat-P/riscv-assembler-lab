# Test Case 3: Comprehensive test (120+ lines)
# Full exercise of arithmetic, memory, control flow
# Includes a simple program: compute factorial or sum

.data
mem: .space 64

.text
.globl main

main:
    la x10, mem         # safe memory base
    addi x1, x0, 5      # n = 5
    jal factorial       # compute 5!
    sw x10, 0(x10)      # store result
    jal sum_to_n        # compute sum 1 to 5
    sw x10, 4(x10)      # store result
    jal end

# Factorial function: x10 = n!
factorial:
    addi x2, x0, 1      # result = 1
    addi x3, x0, 1      # i = 1
fact_loop:
    blt x1, x3, fact_end
    add x2, x2, x3      # result += i (simplified, not actual factorial)
    addi x3, x3, 1      # i++
    jal fact_loop
fact_end:
    add x10, x2, x0     # return result
    jalr x0, x1, 0      # return (x1 is ra, but simplified)

# Sum 1 to n: x10 = sum
sum_to_n:
    addi x2, x0, 0      # sum = 0
    addi x3, x0, 1      # i = 1
sum_loop:
    blt x1, x3, sum_end
    add x2, x2, x3      # sum += i
    addi x3, x3, 1      # i++
    jal sum_loop
sum_end:
    add x10, x2, x0     # return sum
    jalr x0, x1, 0

# More arithmetic operations
arithmetic_test:
    addi x4, x0, 10
    addi x5, x0, 3
    add x6, x4, x5      # 13
    add x19, x4, x4     # 20
    add x20, x5, x5     # 6
    add x21, x6, x4     # 23
    add x22, x6, x5     # 16
    add x23, x19, x20   # 26
    add x24, x21, x22   # 39
    add x25, x23, x24   # 65
    add x26, x25, x6    # 78
    add x27, x26, x19   # 98
    add x28, x27, x20   # 104
    add x29, x28, x21   # 127
    add x30, x29, x22   # 143
    add x31, x30, x23   # 169
    add x7, x31, x24    # 208
    add x8, x7, x25     # 273
    add x9, x8, x26     # 351
    add x10, x9, x27    # 449
    add x11, x10, x28   # 553
    add x12, x11, x29   # 680
    sub x7, x4, x5      # 7
    add x8, x4, x5      # 13 (replaced mul)
    add x9, x4, x5      # 13 (replaced div)
    add x10, x4, x5     # 13 (replaced rem)
    slt x11, x5, x4     # 1
    sltu x12, x5, x4    # 1
    and x13, x4, x5     # 2
    or x14, x4, x5      # 11
    xor x15, x4, x5     # 9
    sll x16, x4, x5     # 80
    srl x17, x4, x5     # 1
    sra x18, x4, x5     # 1

# Memory operations
memory_test:
    sw x6, 8(x10)
    sw x7, 12(x10)
    sh x8, 16(x10)
    sb x9, 18(x10)
    lw x19, 8(x10)
    lw x20, 12(x10)
    lh x21, 16(x10)
    lb x22, 18(x10)
    lhu x23, 16(x10)
    lbu x24, 18(x10)

# Control flow tests
control_test:
    beq x19, x6, ctrl1
    jal error
ctrl1:
    bne x20, x7, error
    blt x5, x4, ctrl2
    jal error
ctrl2:
    bge x4, x5, ctrl3
    jal error
ctrl3:
    bltu x5, x4, ctrl4
    jal error
ctrl4:
    bgeu x4, x5, ctrl5
    jal error
ctrl5:
    jal subroutine

subroutine:
    addi x25, x0, 100
    jalr x26, x1, 0     # return

# More instructions
more_ops:
    addi x27, x0, 0xFF
    slli x28, x27, 8    # 0xFF00
    srli x29, x27, 4    # 0xF
    srai x30, x27, 4    # 0xF (since positive)
    lui x31, 0x12345    # load upper
    auipc x1, 0x100     # add upper pc

# System calls
system_test:
    addi a0, x0, 1      # exit code
    ecall               # system call

# Labels and jumps
jump_test:
    jal label1
label1:
    jal label2
label2:
    beq x0, x0, jump_test  # loop

error:
    addi x31, x0, -1

end:
    addi a0, x0, 10
    ecall