#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

ParsedLine *parse_line(const char *line) {
    ParsedLine *pl = malloc(sizeof(ParsedLine));
    if (!pl) return NULL;
    pl->label = NULL;
    pl->mnemonic = NULL;
    pl->operands = NULL;
    pl->num_operands = 0;

    // Copy line to modifiable string
    char *copy = strdup(line);
    if (!copy) {
        free(pl);
        return NULL;
    }

    // Remove comment
    char *comment = strchr(copy, '#');
    if (comment) *comment = '\0';

    // Trim leading/trailing whitespace
    char *start = copy;
    while (*start && isspace(*start)) start++;
    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;
    *(end + 1) = '\0';

    if (*start == '\0') {
        free(copy);
        return pl; // empty line
    }

    // Check for label
    char *colon = strchr(start, ':');
    if (colon) {
        *colon = '\0';
        // Trim label
        char *label_start = start;
        while (*label_start && isspace(*label_start)) label_start++;
        char *label_end = label_start + strlen(label_start) - 1;
        while (label_end > label_start && isspace(*label_end)) label_end--;
        *(label_end + 1) = '\0';
        if (*label_start) {
            pl->label = strdup(label_start);
        }
        start = colon + 1;
        while (*start && isspace(*start)) start++;
    }

    // Now, split into tokens (mnemonic and operands)
    char *tokens[20]; // assume max 20 tokens
    int count = 0;
    char *token = strtok(start, " \t,");
    while (token && count < 20) {
        // Trim token
        while (*token && isspace(*token)) token++;
        char *t_end = token + strlen(token) - 1;
        while (t_end > token && isspace(*t_end)) t_end--;
        *(t_end + 1) = '\0';
        if (*token) {
            tokens[count++] = token;
        }
        token = strtok(NULL, " \t,");
    }

    if (count > 0) {
        pl->mnemonic = strdup(tokens[0]);
        pl->num_operands = count - 1;
        if (pl->num_operands > 0) {
            pl->operands = malloc(sizeof(char*) * pl->num_operands);
            for (int i = 0; i < pl->num_operands; i++) {
                pl->operands[i] = strdup(tokens[i + 1]);
            }
        } else {
            pl->operands = NULL;
        }
    } else {
        pl->mnemonic = NULL;
        pl->num_operands = 0;
        pl->operands = NULL;
    }

    free(copy);
    return pl;
}

void free_parsed_line(ParsedLine *pl) {
    if (!pl) return;
    free(pl->label);
    free(pl->mnemonic);
    for (int i = 0; i < pl->num_operands; i++) {
        free(pl->operands[i]);
    }
    free(pl->operands);
    free(pl);
}
