#include "../../include/parser/jack_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declared as global in .h via extern
FILE *tokens_xml;
FILE *parsed_xml;
char buffer[BUFFER_SIZE];

void parse_tokens(FILE *tokens) {
  // Set tokens_xml as global, so no need to pass it everywhere.
  tokens_xml = tokens;

  // Create a new file for the output
  parsed_xml = fopen("parsed_output.xml", "w");

  if (!parsed_xml) {
    perror("[ERROR] -> parse_tokens: Unable to create parsed_xml file. "
           "EXITING...\n");
    exit(EXIT_FAILURE);
  }
}

// Eat should work as follows: I should read a line inside of eat()
// Extract either the type of token or the token itself
// If it matches the expected_token, then it's okay. Otherwise exit.
void eat(char *exptected_token, ExtractMode mode) {
  // Extract either VALUE or TYPE from the token into global BUFFER
  char *extracted_val = read_line_and_extract(mode);

  // Compare expected and current token
  if (strcmp(extracted_val, exptected_token) != 0) {
    perror("[ERROR] -> eat: Expected_token does NOT match current_token. "
           "EXITING...\n");
    free(extracted_val);
    exit(EXIT_FAILURE);
  }

  free(extracted_val);
}

char *read_line_and_extract(ExtractMode mode) {
  char *extracted_string = malloc(EXTRACTED_SIZE);

  if (!fgets(buffer, BUFFER_SIZE, tokens_xml)) {
    perror("[ERROR] -> read_line: Could not read the next line. EXITING..."
           " EXITING...\n");
    exit(EXIT_FAILURE);
  }

  char *start, *end;
  int size = 0;

  if (mode == DATA) {
    start = strchr(buffer, '>');
    if (!start) {
      perror("[ERROR] -> read_line: Could not locate the desired character in "
             "read_line. EXITING...\n");
      exit(EXIT_FAILURE);
    }
    // Want to point after >
    start++;

    // Pointer directly on the opening of the closing xml tag.
    end = strchr(start, '<');
    if (!end) {
      perror("[ERROR] -> read_line: Could not locate the desired character in "
             "read_line. EXITING...\n");
      exit(EXIT_FAILURE);
    }

    size = end - start;

    strncpy(extracted_string, start, size);
    extracted_string[size] = '\0';
    return extracted_string;
  } else if (mode == TYPE) {
    start = strchr(buffer, '<');
    if (!start) {
      perror("[ERROR] -> read_line: Could not locate the desired character in "
             "read_line. EXITING...\n");
      exit(EXIT_FAILURE);
    }

    // Want to point after >
    start++;

    // Pointer directly on the opening of the closing xml tag.
    end = strchr(start, '>');
    if (!end) {
      perror("[ERROR] -> read_line: Could not locate the desired character in "
             "read_line. EXITING...\n");
      exit(EXIT_FAILURE);
    }

    size = end - start;

    strncpy(extracted_string, start, size);
    extracted_string[size] = '\0';

    return extracted_string;
  }
}
