#ifndef PARSER_H
#define PARSER_H

typedef struct {
    char *label;        // NULL if no label
    char *mnemonic;     // the instruction mnemonic
    char **operands;    // array of operand strings
    int num_operands;   // number of operands
} ParsedLine;

ParsedLine *parse_line(const char *line);
void free_parsed_line(ParsedLine *pl);

#endif
