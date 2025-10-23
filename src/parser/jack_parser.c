#include <stdio.h>
#include <stdlib.h>

// Declared as global in .h via extern
FILE *tokens_xml;
FILE *parsed_xml;

void parse_tokens(FILE *tokens) {
  // Set tokens_xml as global, so no need to pass it everywhere.
  tokens_xml = tokens;

  // Create a new file for the output
  parsed_xml = fopen("parsed_output.xml", "w");

  if (!parsed_xml) {
    perror("[ERROR]: Unable to create parsed_xml file. EXITING...");
    exit(EXIT_FAILURE);
  }
}

void eat(char *current_token, char *exptected_token) {
  if (!current_token || !exptected_token) {
    perror("[ERROR]: ");
    exit(EXIT_FAILURE);
  }
}
