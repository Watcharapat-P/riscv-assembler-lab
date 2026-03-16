#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../src/parser.h"

void test_parse_line() {
    // Test basic instruction
    ParsedLine *pl = parse_line("add x1, x2, x3");
    assert(pl != NULL);
    assert(pl->label == NULL);
    assert(strcmp(pl->mnemonic, "add") == 0);
    assert(pl->num_operands == 3);
    assert(strcmp(pl->operands[0], "x1") == 0);
    assert(strcmp(pl->operands[1], "x2") == 0);
    assert(strcmp(pl->operands[2], "x3") == 0);
    free_parsed_line(pl);

    // Test with label
    pl = parse_line("loop: add x1, x2, x3");
    assert(pl != NULL);
    assert(strcmp(pl->label, "loop") == 0);
    assert(strcmp(pl->mnemonic, "add") == 0);
    assert(pl->num_operands == 3);
    free_parsed_line(pl);

    // Test with comment
    pl = parse_line("li x1, 42 # load immediate");
    assert(pl != NULL);
    assert(pl->label == NULL);
    assert(strcmp(pl->mnemonic, "li") == 0);
    assert(pl->num_operands == 2);
    assert(strcmp(pl->operands[0], "x1") == 0);
    assert(strcmp(pl->operands[1], "42") == 0);
    free_parsed_line(pl);

    // Test empty line
    pl = parse_line("");
    assert(pl != NULL);
    assert(pl->label == NULL);
    assert(pl->mnemonic == NULL);
    assert(pl->num_operands == 0);
    free_parsed_line(pl);

    // Test only label
    pl = parse_line("label:");
    assert(pl != NULL);
    assert(strcmp(pl->label, "label") == 0);
    assert(pl->mnemonic == NULL);
    assert(pl->num_operands == 0);
    free_parsed_line(pl);

    // Test instruction with spaces
    pl = parse_line("  add   x1  ,  x2 , x3  ");
    assert(pl != NULL);
    assert(pl->label == NULL);
    assert(strcmp(pl->mnemonic, "add") == 0);
    assert(pl->num_operands == 3);
    free_parsed_line(pl);

    // Test label with spaces
    pl = parse_line("  loop : add x1, x2");
    assert(pl != NULL);
    assert(strcmp(pl->label, "loop") == 0);
    assert(strcmp(pl->mnemonic, "add") == 0);
    assert(pl->num_operands == 2);
    free_parsed_line(pl);
}

int main() {
    test_parse_line();
    printf("All parser tests passed!\n");
    return 0;
}
