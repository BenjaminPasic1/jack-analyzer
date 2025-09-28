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

typedef struct {
  char *token;
  TokenType type;
} Token;

FILE *open_file(char *filename);
void write_token_to_output(Token *token, FILE *output_file);

FILE *create_output_file(char *filename);
HashMap *generate_token_hashmap();

int has_next_line(FILE *file);
int has_next_char(FILE *file);
char *get_next_line(FILE *file);

int has_next_token(char **current_line);
char *get_next_token(char **current_line, HashMap *jack_tokens);
Token *get_token_details(HashMap *map, char *token);

void trim(char *current_line);
int is_integer(char *token);

FILE *tokenize_to_xml(FILE *input_file, HashMap *jack_tokens);

#endif
