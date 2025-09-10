#ifndef JACK_TOKENIZER_H
#define JACK_TOKENIZER_H

#include <stdio.h>
#define MAX_BUFFER_SIZE 256

FILE *open_file(char *filename);
int has_next_line(FILE *file);
char *get_next_line(FILE *file);
void trim(char *current_line);

#endif
