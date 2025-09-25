#include "./include/jack_tokenizer.h"
#include "include/hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  FILE *file = open_file("test.txt");
  FILE *output_file = generate_token_output_file();
  HashMap *jack_tokens = generate_token_hashmap();

  while (has_next_line(file)) {
    char *line = get_next_line(file);

    // Keep track of the beginning of the assigned memory
    // for line, since we will manipulate it and lose the initial reference
    char *line_start = line;

    if (*line == '\0' || *line == '/') {
      free(line);
      continue;
    }

    while (has_next_token(&line)) {
      char *token = get_next_token(&line, jack_tokens);
      Token *token_details = get_token_details(jack_tokens, token);
      write_token_to_output(token_details, output_file);

      free(token);
      free(token_details);
    }

    free(line_start);
  }

  // Close the tokens wrapper
  fprintf(output_file, "</tokens>");

  fclose(output_file);
  fclose(file);
}
