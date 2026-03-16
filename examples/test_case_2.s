.data
mem: .space 32

.text
.globl init

init:
    la x10, mem        # safe memory base

    addi x1, x0, 100
    addi x2, x0, 50
    addi x3, x0, -10

arithmetic:
    add x4, x1, x2
    add x18, x1, x3    # 90
    add x19, x2, x3    # 40
    add x20, x4, x1    # 200
    add x21, x4, x2    # 250
    add x22, x4, x3    # 190
    add x23, x18, x19  # 130
    add x24, x20, x21  # 450
    add x25, x22, x23  # 320
    add x26, x24, x25  # 770
    add x27, x26, x1   # 870
    add x28, x27, x2   # 920
    add x29, x28, x3   # 910
    add x30, x29, x4   # 1100
    add x31, x30, x18  # 1190
    add x5, x31, x19   # 1230
    add x6, x5, x20    # 1430
    add x7, x6, x21    # 1680
    add x8, x7, x22    # 1870
    add x9, x8, x23    # 2000
    sub x5, x1, x2
    slt x6, x3, x1
    sltu x7, x3, x1
    sll x8, x1, x0
    slli x9, x2, 2
    srl x11, x1, x0
    srli x12, x1, 2
    sra x13, x3, x0
    srai x14, x3, 1
    and x15, x1, x2
    or  x16, x1, x2
    xor x17, x1, x2

memory_ops:
    sw x4, 0(x10)
    sw x5, 4(x10)
    sh x6, 8(x10)
    sb x7, 10(x10)

    lw x18, 0(x10)
    lw x19, 4(x10)
    lh x20, 8(x10)
    lhu x21, 8(x10)
    lb x22, 10(x10)
    lbu x23, 10(x10)

control_flow:
    beq x18, x4, branch1
    jal error

branch1:
    bne x19, x5, error
    blt x3, x1, branch2
    jal error

branch2:
    bge x1, x3, branch3
    jal error

branch3:
    bltu x3, x1, branch4
    jal error

branch4:
    bgeu x1, x3, branch5
    jal error

branch5:
    jal func

func:
    addi x24, x0, 42
    jalr x0, x1, 0     # still not safe but just tests jalr encoding

end:
    addi a0, x0, 10
    ecall

error:
    addi x31, x0, -1