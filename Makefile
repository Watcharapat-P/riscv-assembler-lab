CC=gcc
CFLAGS=-Wall -Wextra -Isrc

SRC=src/main.c src/assembler.c src/parser.c \
    src/registers.c src/instr_table.c \
    src/symbols.c src/encode.c

assembler:
	gcc $(CFLAGS) $(SRC) -o assembler

clean:
	rm -f assembler
