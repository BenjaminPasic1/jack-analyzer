#include "../../include/parser/jack_parser.h"
#include "../../include/parser/compile_class.h"
#include "../../include/util.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Declared as global in .h via extern
FILE *tokens_xml;
FILE *parsed_xml;
char buffer[BUFFER_SIZE];

// type → 'int' | 'char' | 'boolean' | className
PossibleTokens var_type_keywords[VAR_TYPE_KEYWORDS_SIZE] = {
    {"int", DATA}, {"char", DATA}, {"boolean", DATA}, {"identifier", TYPE}};

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

  // Load next line into the global BUFFER
  read_line();
  char *extracted_string = extract_from_buffer(mode);

  printf("[DEBUG] eat -> extracted_string: %s, expected_string: %s\n",
         extracted_string, expected_token);

  // Compare expected and current token
  if (strcmp(extracted_string, expected_token) != 0) {
    printf("[ERROR] -> eat: expected_token: %s does not match "
           "extracted_string: %s\n",
           expected_token, extracted_string);
    free(extracted_string);
    fatal_error(
        "Expected value and extracted value do not match. EXITING...\n");
  }

  free(extracted_string);
}

// One keyword can have multiple names. need a way to check multiple possible
// keywords at a single token.
void eat_any(PossibleTokens *tokens, size_t tokens_size) {
  read_line();

  char *extracted_string = NULL;

  for (int i = 0; i < tokens_size; i++) {
    extracted_string = extract_from_buffer(tokens[i].mode);

    printf("[DEBUG] eat_any -> extracted_string: %s -- tokens[i]: %s\n",
           extracted_string, tokens[i].token);

    if (strcmp(extracted_string, tokens[i].token) == 0) {
      free(extracted_string);
      return;
    }
    free(extracted_string);
  }

  fatal_error("[ERROR] -> eat_any: None of the possible tokens match the "
              "current line. Exiting...\n");
}

void read_line() {
  if (!fgets(buffer, BUFFER_SIZE, tokens_xml))
    fatal_error(
        "[ERROR] -> read_line: Could not read the next line. EXITING...");
}

char *extract_from_buffer(ExtractMode mode) {
  char *extracted_string = malloc(EXTRACTED_SIZE);

  if (!extracted_string) {
    fatal_error("[ERROR] -> extract_from_buffer: Couldn't allocate memory for "
                "extracted_string. EXITING...\n");
  }

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

int is_next_any_of(PossibleTokens *tokens, size_t tokens_size) {

  for (int i = 0; i < tokens_size; i++) {
    char *extracted_string = peek_next_line(tokens->mode);

    printf(
        "[DEBUG] is_next_any_of -> extracted_string: %s, expected_token: %s\n",
        extracted_string, tokens[i].token);

    if (strcmp(extracted_string, tokens[i].token) == 0) {
      free(extracted_string);
      return 1;
    }

    free(extracted_string);
  }

  return 0;
}

int is_next(char *expected_token, ExtractMode mode) {
  char *next_line = peek_next_line(mode);
  int flag = 0;

  if (strcmp(expected_token, next_line) == 0)
    flag = 1;

  free(next_line);
  return flag;
}

// Only check for square brackets.
void check_for_square_brackets_only() {
  char *next_token = peek_next_line(DATA);

  if (strcmp(next_token, "[") == 0) {
    eat("[", DATA);
    write_buffer_to_file();

    eat("]", DATA);
    write_buffer_to_file();
  }

  free(next_token);
}

// Check for square brackets and for expression inside of the brackets.
void check_for_brackets_and_expression() {
  char *next_token = peek_next_line(DATA);

  if (strcmp(next_token, "[") == 0) {
    eat("[", DATA);
    write_buffer_to_file();

    free(next_token);
    next_token = peek_next_line(DATA);

    if (strcmp(next_token, "]") != 0) {
      // Compile expression
    }

    eat("]", DATA);
    write_buffer_to_file();
  }
}

// Peek line and get either the DATA or TYPE
char *peek_next_line(ExtractMode mode) {
  long current_position = ftell(tokens_xml);

  read_line();
  char *extracted_string = extract_from_buffer(mode);

  fseek(tokens_xml, current_position, SEEK_SET);

  return extracted_string;
}

void skip_line() {
  if (!fgets(buffer, BUFFER_SIZE, tokens_xml))
    fatal_error(
        "[ERROR] -> skip_line: Could not read the next line. EXITING...");
}

void write_buffer_to_file() { fprintf(parsed_xml, "%s\n", buffer); }

void write_tag_to_file(char *tag) { fprintf(parsed_xml, "%s\n", tag); }
