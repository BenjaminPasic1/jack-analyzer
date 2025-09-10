#include "./include/jack_tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *file = open_file("test.txt");

    while(has_next_line(file)){
	char *line = get_next_line(file);

	if(*line == '\0'){
	    free(line);
	    continue;
	}

	printf("%s\n", line);

	free(line);
    }

    fclose(file);
}
