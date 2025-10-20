#include "../include/jack_parser.h"
#include "../include/jack_tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RETURN_TYPES_KEYWORDS_SIZE 5
#define VAR_KIND_KEYWORDS_SIZE 2
#define SUBROUTINE_KIND_KEYWORDS_SIZE 3
#define EXPRESSION_SYMBOLS_SIZE 10

static char buffer[MAX_TOKEN_LINE_SIZE];
static ExpectedToken return_types_keywords[] = {{"int", TOKEN_VALUE},
                                                {"char", TOKEN_VALUE},
                                                {"boolean", TOKEN_VALUE},
                                                {"identifier", TOKEN_TYPE},
                                                {"void", TOKEN_VALUE}};

static ExpectedToken var_kind_keywords[] = {{"static", TOKEN_VALUE},
                                            {"field", TOKEN_VALUE}};

static ExpectedToken subroutine_kind_keywords[] = {{"constructor", TOKEN_VALUE},
                                                   {"function", TOKEN_VALUE},
                                                   {"method", TOKEN_VALUE}};

static ExpectedToken term_keywords[] = {
    {"integerConstant", TOKEN_TYPE}, {"stringConstant", TOKEN_TYPE},
    {"true", TOKEN_VALUE}, {"false", TOKEN_VALUE}, {"null", TOKEN_VALUE},
    {"this", TOKEN_VALUE}, {"identifier", TOKEN_TYPE};

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

  peek_line(token_xml);
  while (!check_for_match(buffer, "}", TOKEN_VALUE)) {
    fprintf(output_file, "<subroutineDec>\n");
    compile_subroutine_dec(output_file, token_xml);
    fprintf(output_file, "</subroutineDec>\n");
  }
}

void compile_subroutine_dec(FILE *output_file, FILE *token_xml) {
  // Check kind of subrutine
  eat_any(subroutine_kind_keywords, SUBROUTINE_KIND_KEYWORDS_SIZE, token_xml);
  fprintf(output_file, "%s\n", buffer);
  // Check type of subroutine
  eat_any(return_types_keywords, RETURN_TYPES_KEYWORDS_SIZE, token_xml);
  fprintf(output_file, "%s\n", buffer);
  // Check for function name (identifier)
  eat("identifier", token_xml, TOKEN_TYPE);
  fprintf(output_file, "%s\n", buffer);

  // Opening
  eat("(", token_xml, TOKEN_VALUE);
  fprintf(output_file, "%s\n", buffer);

  // Everything checks out, then look through the parameter list.
  compile_parameter_list(output_file, token_xml);

  // Closing
  eat(")", token_xml, TOKEN_VALUE);
  fprintf(output_file, "%s\n", buffer);

  compile_subroutine_body(output_file, token_xml);
}

void compile_subroutine_body(FILE *output_file, FILE *token_xml) {
  fprintf(output_file, "<subroutineBody>\n");

  // Beginning of function body
  eat("{", token_xml, TOKEN_VALUE);
  fprintf(output_file, "%s\n", buffer);

  peek_line(token_xml);
  while (!check_for_match(buffer, "}", TOKEN_VALUE)) {

    if (check_for_match(buffer, "var", TOKEN_VALUE)) {
      compile_var_declaration(output_file, token_xml);
    } else if (check_for_match(buffer, "let", TOKEN_VALUE)) {

    } else if (check_for_match(buffer, "do", TOKEN_VALUE)) {

    } else if (check_for_match(buffer, "while", TOKEN_VALUE)) {

    } else if (check_for_match(buffer, "return", TOKEN_VALUE)) {
    }
  }

  fprintf(output_file, "<subroutineBody>\n");
}

void compile_let_statement(FILE *output_file, FILE *token_xml) {
  fprintf(output_file, "<letStatement>\n");

  eat("let", token_xml, TOKEN_VALUE);
  fprintf(output_file, "%s\n", buffer);

  eat("identifier", token_xml, TOKEN_TYPE);
  fprintf(output_file, "%s\n", buffer);

  peek_line(token_xml);
  if (check_for_match(buffer, "[", TOKEN_VALUE)) {
    eat("[", token_xml, TOKEN_VALUE);
    fprintf(output_file, "%s\n", buffer);

    compile_expression(output_file, token_xml);

    eat("]", token_xml, TOKEN_VALUE);
    fprintf(output_file, "%s\n", buffer);
  }
}

void compile_expression(FILE *output_file, FILE *token_xml) {
  // Rekurzivno moras uraditi ovaj dio. SKontaj 1!!!!
  fprintf(output_file, "<expression>\n");

  compile_term(output_file, token_xml);
}

void compile_term(FILE *output_file, FILE *token_xml) {
  peek_line(token_xml);

  if (check_for_match(buffer, "(", TOKEN_VALUE)) {
    eat("(", token_xml, TOKEN_VALUE);
    fprintf(output_file, "%s\n", buffer);

    compile_expression(output_file, token_xml);

    eat(")", token_xml, TOKEN_VALUE);
    fprintf(output_file, "%s\n", buffer);
    return;
  }
}

void compile_var_declaration(FILE *output_file, FILE *token_xml) {
  fprintf(output_file, "<varDec>\n");

  eat("var", token_xml, TOKEN_VALUE);
  fprintf(output_file, "%s\n", buffer);

  eat_any(return_types_keywords, RETURN_TYPES_KEYWORDS_SIZE, token_xml);
  fprintf(output_file, "%s\n", buffer);

  eat("identifier", token_xml, TOKEN_TYPE);
  fprintf(output_file, "%s\n", buffer);

  // If it's an array declaration
  peek_line(token_xml);
  if (check_for_match(buffer, "[", TOKEN_VALUE)) {
    eat("[", token_xml, TOKEN_VALUE);
    fprintf(output_file, "%s\n", buffer);

    eat("integerConstant", token_xml, TOKEN_TYPE);
    fprintf(output_file, "%s\n", buffer);

    eat("]", token_xml, TOKEN_VALUE);
    fprintf(output_file, "%s\n", buffer);
  } else if (check_for_match(buffer, ",", TOKEN_VALUE)) {
    peek_line(token_xml);
    while (!check_for_match(buffer, ";", TOKEN_VALUE)) {

      eat(",", token_xml, TOKEN_VALUE);
      fprintf(output_file, "%s\n", buffer);

      eat("identifier", token_xml, TOKEN_TYPE);
      fprintf(output_file, "%s\n", buffer);

      peek_line(token_xml);
    }
  }

  eat(";", token_xml, TOKEN_VALUE);
  fprintf(output_file, "%s\n", buffer);
  fprintf(output_file, "</varDec>\n");
}

void compile_parameter_list(FILE *output_file, FILE *token_xml) {
  fprintf(output_file, "<parameterList>\n");

  // If the next token is a closing parantheses, no parameters in the function
  peek_line(token_xml);
  if (check_for_match(buffer, ")", TOKEN_VALUE)) {
    fprintf(output_file, "</parameterList>\n");
    return;
  }

  // Used for adding commas, if it's the first iteration no need for ,
  int first_loop = 1;

  peek_line(token_xml);
  while (!check_for_match(buffer, ")", TOKEN_VALUE)) {

    if (!first_loop) {
      eat(",", token_xml, TOKEN_VALUE);
      fprintf(output_file, "%s\n", buffer);
    }

    eat_any(return_types_keywords, RETURN_TYPES_KEYWORDS_SIZE, token_xml);
    // parameter type
    fprintf(output_file, "%s\n", buffer);
    // parameter name
    eat("identifier", token_xml, TOKEN_TYPE);
    fprintf(output_file, "%s\n", buffer);
    peek_line(token_xml);

    first_loop = 0;
    peek_line(token_xml);
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
    fprintf(output_file, "<classVarDec>\n");
    compile_var_dec(output_file, token_xml);
    fprintf(output_file, "</classVarDec>\n");
    peek_line(token_xml);
  }
}

void compile_var_dec(FILE *output_file, FILE *token_xml) {
  // is it static or field? proceed
  eat_any(var_kind_keywords, VAR_KIND_KEYWORDS_SIZE, token_xml);
  fprintf(output_file, "%s\n", buffer);

  // matches any of the allowed data types? proceed.
  eat_any(return_types_keywords, RETURN_TYPES_KEYWORDS_SIZE, token_xml);
  fprintf(output_file, "%s\n", buffer);

  eat("identifier", token_xml, TOKEN_TYPE);
  fprintf(output_file, "%s\n", buffer);

  // after static int x for example, it can be the end of the statement
  // or it can be a comma where we want to define another variable of the same
  // kind and type

  peek_line(token_xml);
  while (check_for_match(buffer, ",", TOKEN_VALUE)) {
    eat(",", token_xml, TOKEN_VALUE);
    fprintf(output_file, "%s\n", buffer);
    eat("identifier", token_xml, TOKEN_TYPE);
    fprintf(output_file, "%s\n", buffer);
    peek_line(token_xml);
  }

  eat(";", token_xml, TOKEN_VALUE);
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

// Place next line into buffer, but return file pointer back so it's not
// incremented.
void peek_line(FILE *file) {
  long pos = ftell(file);

  fgets(buffer, MAX_TOKEN_LINE_SIZE, file);

  fseek(file, pos, SEEK_SET);
}
