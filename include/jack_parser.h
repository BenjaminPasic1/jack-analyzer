#ifndef JACK_PARSER_H
#define JACK_PARSER_H

#include "../include/jack_tokenizer.h"
#include <stdio.h>

#define MAX_VALUE_SIZE 64
#define MAX_TOKEN_LINE_SIZE 128

typedef enum { TOKEN_TYPE, TOKEN_VALUE } MatchMode;
typedef struct {
  char *keyword;
  MatchMode mode;
} ExpectedToken;

FILE *parse_to_xml();

// Takes the string, compares to current token
// If it does not match exit the program
void eat(char *expected, FILE *token_xml, MatchMode mode);

// Battles the limitations of eat, where it's only good for comparing
// the exact expected value, but in the case where a keyword can be more than 1
// thing, eat_any comes into play.
void eat_any(ExpectedToken *token, size_t size, FILE *token_xml);

// Starts the compilation process. Everything else should cascade
void compile_class(FILE *output_file, FILE *token_xml);
// Compiles class fields
void compile_class_var_dec(FILE *output_file, FILE *token_xml);
// Compiles individual field
void compile_var_dec(FILE *output_file, FILE *token_xml);

// Compile
void compile_class_subroutine_dec(FILE *output_file, FILE *token_xml);
void compile_subroutine_dec(FILE *output_file, FILE *token_xml);
void compile_parameter_list(FILE *output_file, FILE *token_xml);
void compile_subroutine_body(FILE *output_file, FILE *token_xml);

// Compile Statements
void compile_var_declaration(FILE *output_file, FILE *token_xml);
void compile_let_statement(FILE *output_file, FILE *token_xml);

int check_for_match(char *buffer, char *expected, MatchMode mode);
void peek_line(FILE *file);

#endif
