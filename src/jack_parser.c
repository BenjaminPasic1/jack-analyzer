#include "../include/jack_parser.h"
#include "../include/jack_tokenizer.h"
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
  compile_class_subroutine_dec(output_file, token_xml);
}

void compile_class_subroutine_dec(FILE *output_file, FILE *token_xml) {
  // Class can only have fields & constructor. So check if there are any
  // subroutines first.

  peek_line(token_xml);
  if (!check_for_match(buffer, "constructor", TOKEN_VALUE) &&
      !check_for_match(buffer, "function", TOKEN_VALUE) &&
      !check_for_match(buffer, "method", TOKEN_VALUE))
    return;

  char *possible_keywords[] = {"constructor", "function", "method"};

  peek_line(token_xml);
  while (!check_for_match(buffer, "}", TOKEN_VALUE)) {
  }
}

void compile_class_var_dec(FILE *output_file, FILE *token_xml) {
  // fields are optional, so i should check if there are any first
  // if not, i should just return
  peek_line(token_xml);
  if (!check_for_match(buffer, "static", TOKEN_VALUE) &&
      !check_for_match(buffer, "field", TOKEN_VALUE))
    return;

  fprintf(output_file, "<classVarDec>\n");

  peek_line(token_xml);
  while (!check_for_match(buffer, "constructor", TOKEN_VALUE)) {
    compile_var_dec(output_file, token_xml);
  }

  fprintf(output_file, "</classVarDec>");
}

void compile_var_dec(FILE *output_file, FILE *token_xml) {
  ExpectedToken var_kind_keywords[] = {{"static", TOKEN_VALUE},
                                       {"field", TOKEN_VALUE}};
  size_t var_kind_size =
      sizeof(var_kind_keywords) / sizeof(var_kind_keywords[0]);

  ExpectedToken var_types_keywords[] = {{"int", TOKEN_VALUE},
                                        {"char", TOKEN_VALUE},
                                        {"boolean", TOKEN_VALUE},
                                        {"identifier", TOKEN_TYPE}};

  size_t var_types_size =
      sizeof(var_types_keywords) / sizeof(var_types_keywords[0]);

  // is it static or field? proceed
  eat_any(var_kind_keywords, var_kind_size, token_xml);
  fprintf(output_file, "%s\n", buffer);

  // matches any of the allowed data types? proceed.
  eat_any(var_types_keywords, var_types_size, token_xml);
  fprintf(output_file, "%s\n", buffer);

  // two ways to define following var names:
  // static int x,y OR static int x
  // Find a way to determine if it's x or , up next
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

void eat_any(ExpectedToken *expected, size_t size, FILE *token_xml) {
  if (!fgets(buffer, MAX_TOKEN_LINE_SIZE, token_xml)) {
    perror("Error: Unexpected end of file... Exiting...\n");
    exit(EXIT_FAILURE);
  }

  int ok = 0;
  for (int i = 0; i < size; i++) {
    if (check_for_match(buffer, expected[i].keyword, expected[i].mode))
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
