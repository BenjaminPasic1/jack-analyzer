#include "../../include/parser/jack_parser.h"
#include "../../include/parser/compile_class.h"
#include "../../include/util.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Declared as global in .h via extern
FILE *tokens_xml;
FILE *parsed_xml;
char buffer[BUFFER_SIZE];

void parse_tokens() {
  // Set tokens_xml as global, so no need to pass it everywhere.
  tokens_xml = fopen("tokens_output.xml", "r");

  if (!tokens_xml)
    fatal_error("[ERROR] -> parse_tokens: Unable to open tokens_output.xml. "
                "Exiting...\n");

  rewind(tokens_xml);

  // Create a new file for the output
  parsed_xml = fopen("parsed_output.xml", "w");

  if (!parsed_xml) {
    fatal_error("[ERROR] -> parse_tokens: Unable to create parsed_xml file. "
                "Exiting...");
  }

  compile_class();
  fclose(parsed_xml);
  fclose(tokens_xml);
}

// Eat should work as follows: I should read a line inside of eat()
// Extract either the type of token or the token itself
// If it matches the expected_token, then it's okay. Otherwise exit.
void eat(char *expected_token, ExtractMode mode) {
  // Extract either VALUE or TYPE from the token into global BUFFER
  char *extracted_val = read_line_and_extract(mode);

  // Compare expected and current token
  if (strcmp(extracted_val, expected_token) != 0) {
    printf(
        "[ERROR] -> eat: expected_token: %s does not match extracted_val: %s\n",
        expected_token, extracted_val);
    free(extracted_val);
    fatal_error(
        "Expected value and extracted value do not match. EXITING...\n");
  }

  free(extracted_val);
}

char *read_line_and_extract(ExtractMode mode) {
  char *extracted_string = malloc(EXTRACTED_SIZE);
  if (!extracted_string)
    fatal_error("[ERROR] -> read_line: Memory allocation failed. EXITING...");

  if (!fgets(buffer, BUFFER_SIZE, tokens_xml))
    fatal_error(
        "[ERROR] -> read_line: Could not read the next line. EXITING...");

  char *start, *end;
  int size = 0;

  if (mode == DATA) {
    start = strchr(buffer, '>');
    if (!start)
      fatal_error(
          "[ERROR] -> read_line: Could not locate '>' character. EXITING...");

    start++; // Move past '>'

    end = strchr(start, '<');
    if (!end)
      fatal_error(
          "[ERROR] -> read_line: Could not locate '<' character. EXITING...");

    size = end - start;

    strncpy(extracted_string, start, size);
    extracted_string[size] = '\0';
    return extracted_string;

  } else if (mode == TYPE) {
    start = strchr(buffer, '<');
    if (!start)
      fatal_error(
          "[ERROR] -> read_line: Could not locate '<' character. EXITING...");

    start++; // Move past '<'

    end = strchr(start, '>');
    if (!end)
      fatal_error(
          "[ERROR] -> read_line: Could not locate '>' character. EXITING...");

    size = end - start;

    strncpy(extracted_string, start, size);
    extracted_string[size] = '\0';
    return extracted_string;

  } else {
    free(extracted_string);
    fatal_error("[ERROR] -> read_line: Invalid extract mode. EXITING...");
  }

  return NULL; // unreachable but silences warnings
}

void skip_line() {
  if (!fgets(buffer, BUFFER_SIZE, tokens_xml))
    fatal_error(
        "[ERROR] -> skip_line: Could not read the next line. EXITING...");
}

void write_buffer_to_file() { fprintf(parsed_xml, "%s\n", buffer); }

void write_tag_to_file(char *tag) { fprintf(parsed_xml, "%s\n", tag); }
