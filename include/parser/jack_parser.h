#ifndef JACK_PARSER_H
#define JACK_PARSER_H

#include <stdio.h>

extern FILE *tokens_xml;
extern FILE *parsed_xml;

void parse_tokens(FILE *tokens);

#endif
