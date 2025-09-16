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

int has_next_token(char **current_line){

    if(**current_line == '\0'){
	return 0;
    }

    return 1;
}

char *get_next_token(char **current_line){
    char *token = calloc(MAX_TOKEN_SIZE);

    while(**current_line != '\0'){
	if()

	*current_line++;
    }
    
}

char *get_next_line(FILE *file){
    char *line = malloc(MAX_LINE_SIZE);
    
    if(!line){
	perror("ERROR: Failed to allocate memory for line string.");
	exit(EXIT_FAILURE);
    }

    fgets(line, MAX_LINE_SIZE, file);

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

// TokenType get_token_type(char * token){
//     //Check first character
//     //If it's a number -> not identifier, symbol, keyword, string constant
// 		    // -> then see if there are any more numbers left to grab
//     //If it's length of 1 -> check if it's a symbol otherwise it must be a single letter identifier or number
// 			//-> but we check if it's a number before this
//     //If it's not a number, or single length then it's a string, keyword or multiletter identifier
// 			//->first check if it's a string by the " sign, if not check keywords otherwise it's an identifier
//     return;
// }


