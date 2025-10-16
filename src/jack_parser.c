#include "../include/jack_parser.h"
#include "../include/jack_tokenizer.h"
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char buffer[MAX_TOKEN_LINE_SIZE];

FILE *parse_to_xml() {
  FILE *token_xml = fopen("tokens_output.xml", "r");
  FILE *output_file = create_output_file("parse_output.xml");

  if (!token_xml || !output_file) {
    perror("ERROR: Parsing failed. exiting...");
    exit(EXIT_FAILURE);
  }

  return output_file;
}

void compile_class(FILE *output_file, FILE *token_xml) {
  eat("class", token_xml, TOKEN_VALUE);
  fprintf(output_file, "<class>\n");
  fprintf(output_file, "%s\n", buffer);
  eat("identifier", token_xml, TOKEN_TYPE);
  fprintf(output_file, "%s\n", buffer);
  eat("{", token_xml, TOKEN_VALUE);
  fprintf(output_file, "%s\n", buffer);

  compile_class_var_dec(output_file, token_xml);
}

void compile_class_var_dec(FILE *output_file, FILE *token_xml) {
  fprintf(output_file, "<classVarDec>\n");

  const char *possible_keywords[] = {"static", "field"};
  size_t size = sizeof(possible_keywords) / sizeof(possible_keywords[0]);

  peek_line(token_xml);
  while (!check_for_match("constructor", "constructor", TOKEN_VALUE)) {
    eat_any(possible_keywords, size, token_xml, TOKEN_VALUE);
    fprintf(output_file, "%s\n", buffer);
  }
}

void eat(char *expected, FILE *token_xml, MatchMode mode) {
  if (!fgets(buffer, MAX_TOKEN_LINE_SIZE, token_xml)) {
    perror("Error: Unexpected end of file... Exiting...\n");
    exit(EXIT_FAILURE);
  }

  if (!check_for_match(buffer, expected, mode)) {
    fprintf(stderr, "Error: Expected '%s', but got '%s' instead. Exiting...\n",
            expected, buffer);
    exit(EXIT_FAILURE);
  }
}

void eat_any(const char *expected[], int size, FILE *token_xml,
             MatchMode mode) {
  if (!fgets(buffer, MAX_TOKEN_LINE_SIZE, token_xml)) {
    perror("Error: Unexpected end of file... Exiting...\n");
    exit(EXIT_FAILURE);
  }

  int ok = 0;
  for (int i = 0; i < size; i++) {
    if (check_for_match(buffer, expected[i], mode))
      ok = 1;
  }

  if (!ok) {
    perror("Issues in eat_any, none of the expected options match the buffer. "
           "EXITING...\n");
    exit(EXIT_FAILURE);
  }
}

int check_for_match(char *current, char *expected, MatchMode mode) {
  char *start, *end;
  size_t len;

  if (mode == TOKEN_VALUE) {
    start = strchr(current, '>');
    if (!start)
      return 0;
    start++;
    end = strchr(start, '<'); // after '>'
    if (!end)
      return 0;
  } else { // TOKEN_TYPE
    start = strchr(current, '<');
    if (!start)
      return 0;
    start++;
    end = strchr(start, '>');
    if (!end)
      return 0;
  }

  len = end - start;
  return (strlen(expected) == len && strncmp(start, expected, len) == 0);
}

// Place next line into buffer, but return pointer back so it's not incremented.
void peek_line(FILE *file) {
  long pos = ftell(file);

  fgets(buffer, MAX_TOKEN_LINE_SIZE, file);

  fseek(file, pos, SEEK_SET);
}
