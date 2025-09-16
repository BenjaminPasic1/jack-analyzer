#ifndef JACK_TOKENIZER_H
#define JACK_TOKENIZER_H

#include "../include/hashmap.h"

#include <stdio.h>
#define MAX_LINE_SIZE 256
#define MAX_TOKEN_SIZE 32
#define HASHMAP_SIZE 1024

typedef enum {
    KEYWORD,
    SYMBOL,
    INT_CONSTANT,
    STR_CONSTANT,
    IDENTIFIER,
} TokenType;

FILE *open_file(char *filename);
HashMap *generate_token_hashmap();

int has_next_line(FILE *file);
int has_next_char(FILE *file);
char *get_next_line(FILE *file);

int has_next_token(char **current_line);
char *get_next_token(char **current_line, HashMap *jack_tokens);
TokenType get_token_type(char * token);

void trim(char *current_line);

#endif
