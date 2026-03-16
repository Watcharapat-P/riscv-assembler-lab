# RISC-V Assembler — Lab 8-4

A two-pass assembler for a subset of RV32I, built incrementally as described in the 219223 Computer System Laboratory Lab 8-4 spec.

## Supported Instructions

**R-type:** `add sub and or xor sll srl sra slt sltu`  
**I-type:** `addi andi ori xori slli srli srai slti sltiu`  
**Loads:** `lb lbu lh lhu lw`  
**Stores:** `sb sh sw`  
**Branches:** `beq bne blt bge bltu bgeu`  
**Jumps:** `jal jalr`

## Build & Run

```bash
# Build the assembler
make assembler

# Assemble a file (output defaults to <name>.out)
./assembler examples/test_case_1.s

# Assemble with explicit output
./assembler examples/test_case_1.s output.txt

# Build and run all unit tests
make test

# Clean
make clean
```

## Project Structure

```
riscv-assembler-lab/
├── Makefile
├── README.md
├── src/
│   ├── main.c          # Entry point, CLI arg handling
│   ├── assembler.c/h   # Two-pass pipeline controller
│   ├── parser.c/h      # Line parser → ParsedLine struct
│   ├── registers.c/h   # Register name → number (x0..x31, ABI names)
│   ├── instr_table.c/h # Instruction metadata lookup table
│   ├── symbols.c/h     # Label symbol table
│   └── encode.c/h      # Bit-field packing for each format
├── tests/
│   ├── test_registers.c
│   ├── test_parser.c
│   ├── test_instr_table.c
│   ├── test_symbols.c
│   ├── test_encode.c
│   └── test_assembler.c
└── examples/
    ├── test_case_1.s       # ~20 lines: array sum
    ├── test_case_2.s       # ~50 lines: bubble sort
    ├── test_case_3.s       # ~120 lines: factorial + fibonacci + memory
    └── TEST_REPORT.md      # Machine code dumps vs Venus verification
```

## Two-Pass Design

**Pass 1** — scans every line, records label → address in the symbol table.  
**Pass 2** — encodes each instruction; resolves register names, immediate values, and label offsets.

Branch offsets use PC-relative byte addressing:  
`offset = label_address − current_PC`

## Assembly Syntax

```asm
# Comments with #
label:          addi x1, x0, 10     # label on same line as instruction
loop:                                # label-only line
                lw   x2, 4(x1)      # load: offset(base) syntax
                beq  x1, x2, loop   # branch to label
                jal  x0, loop       # unconditional jump
```

Registers accepted as `x0`–`x31` or ABI names (`zero ra sp gp tp t0-t6 s0-s11 a0-a7 fp`).

## Output Format

One hex word per instruction:
```
0x00500093
0x06400193
0xFE9FF06F
```
