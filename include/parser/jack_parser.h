#ifndef JACK_PARSER_H
#define JACK_PARSER_H

#include <stdio.h>
#define BUFFER_SIZE 256
#define EXTRACTED_SIZE 64

// Sizes for each tokens keywords array
#define VAR_TYPE_KEYWORDS_SIZE 4

extern FILE *tokens_xml;
extern FILE *parsed_xml;
extern char buffer[BUFFER_SIZE];

typedef enum { DATA, TYPE } ExtractMode;
typedef struct {
  char *token;
  ExtractMode mode;
} PossibleTokens;

// Global token keywords used in multiple compile classes.
// type → 'int' | 'char' | 'boolean' | className
extern PossibleTokens var_type_keywords[VAR_TYPE_KEYWORDS_SIZE];

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

// Used to check next line without moving the pointer forwards
// in the tokens_xml file. Mainly used for loops
// for example when there's an unknown number of class var decs.
char *peek_next_line(ExtractMode mode);

// Used to check if the next token we select matches any of the given
// tokens in the list.
int is_next_any_of(PossibleTokens *tokens, size_t tokens_size);
int is_next(char *token, ExtractMode mode);

// Check for square brackets only
void check_for_square_brackets_only();
// Check for square brackets and expression.
void check_for_brackets_and_expression();

#endif
