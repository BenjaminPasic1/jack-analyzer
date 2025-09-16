#include "./include/jack_tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *file = open_file("test.txt");

    while(has_next_line(file)){
	char *line = get_next_line(file);

	//Keep track of the beginning of the assigned memory
	//for line, since we will manipulate it and lose the initial reference
	char *line_start = line;

	if(*line == '\0' || *line == '/'){
	    free(line);
	    continue;
	}
	
	while(has_next_token(&line)){
	    char *token = get_next_token(&line);

	    printf("Token: %s\n", token);
	    free(token);
	}

	free(line_start);
    }
    
    fclose(file);
}
