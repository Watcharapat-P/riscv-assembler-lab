.data
mem: .space 8

.text
.globl start

start:
    la x10, mem

    addi x1, x0, 10
    addi x2, x0, 20
    add x3, x1, x2
    add x14, x1, x1    # 20
    add x15, x2, x2    # 40
    add x16, x3, x1    # 30
    add x17, x3, x2    # 50
    add x18, x14, x15  # 60
    add x19, x16, x17  # 80
    add x20, x18, x19  # 140
    add x21, x20, x1   # 150
    add x22, x21, x2   # 170
    add x23, x22, x3   # 200
    add x24, x23, x14  # 220
    add x25, x24, x15  # 260
    add x26, x25, x16  # 290
    add x27, x26, x17  # 340
    add x28, x27, x18  # 400
    add x29, x28, x19  # 480
    add x30, x29, x20  # 620
    add x31, x30, x21  # 770
    sub x4, x2, x1
    and x5, x1, x2
    or  x6, x1, x2
    xor x7, x1, x2
    sll x8, x1, x0
    srl x9, x2, x0

    sw x3, 0(x10)
    sw x4, 4(x10)
    lw x11, 0(x10)
    lw x12, 4(x10)

    beq x11, x3, equal
    jal end

equal:
    addi x13, x0, 1

end:
    addi a0, x0, 10
    ecall