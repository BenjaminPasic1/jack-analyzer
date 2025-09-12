#ifndef JACK_TOKENIZER_H
#define JACK_TOKENIZER_H

#include <stdio.h>
#define MAX_BUFFER_SIZE 256

FILE *open_file(char *filename);

int has_next_line(FILE *file);
int has_next_char(FILE *file);
char *get_next_line(FILE *file);

int has_next_token(char **current_line);
char *get_next_token(char **current_line);

void trim(char *current_line);

#endif
