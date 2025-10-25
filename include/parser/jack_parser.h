#ifndef JACK_PARSER_H
#define JACK_PARSER_H

#include <stdio.h>
#define BUFFER_SIZE 256
#define EXTRACTED_SIZE 64

extern FILE *tokens_xml;
extern FILE *parsed_xml;
extern char buffer[BUFFER_SIZE];

typedef enum { DATA, TYPE } ExtractMode;
typedef struct {
  char *token;
  ExtractMode mode;
} PossibleTokens;

void parse_tokens();

// Single Expected token
void eat(char *exptected_token, ExtractMode mode);
void eat_any(PossibleTokens *tokens, size_t tokens_size);

void read_line();
// Skip redundant tokens
void skip_line();

char *extract_from_buffer(ExtractMode mode);

// Write to files functions
void write_buffer_to_file();
void write_tag_to_file(char *tag);

#endif
