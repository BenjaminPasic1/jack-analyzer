#ifndef JACK_PARSER_H
#define JACK_PARSER_H

#include <stdio.h>
#define BUFFER_SIZE 256
#define EXTRACTED_SIZE 64

extern FILE *tokens_xml;
extern FILE *parsed_xml;
extern char buffer[BUFFER_SIZE];

typedef enum { DATA, TYPE } ExtractMode;

void parse_tokens(FILE *tokens);
void eat(char *exptected_token, ExtractMode mode);
char *read_line_and_extract(ExtractMode mode);

#endif
