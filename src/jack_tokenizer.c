#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "../include/jack_tokenizer.h"

FILE *open_file(char *filename){
    FILE *file = fopen(filename, "r");

    if(file == NULL){
	perror("ERROR: Error with reading in the file input.\n");
	exit(EXIT_FAILURE);
    }

    return file;
}

int has_next_line(FILE *file){
    int ch = fgetc(file);        

    if(ch == EOF){
	return 0;
    }

    ungetc(ch, file);
    return 1;
}

char *get_next_line(FILE *file){
    char *line = malloc(MAX_BUFFER_SIZE);
    
    if(!line){
	perror("ERROR: Failed to allocate memory for line string.");
	exit(EXIT_FAILURE);
    }

    fgets(line, MAX_BUFFER_SIZE, file);

    trim(line);

    return line; 
}

void trim(char *line){
    char *start = line;
    char *end = line + strlen(line) -1;

    while (*start && isspace((unsigned char)*start))
	start++;

    while (end >= start && isspace((unsigned char)*end))
	end--;

    size_t len = end - start + 1;
    memmove(line, start, len);
    line[len] = '\0';
}


