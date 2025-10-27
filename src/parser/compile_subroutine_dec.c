#include "../../include/parser/compile_subroutine_dec.h"
#include "../../include/parser/jack_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// subroutineDec:
//     ('constructor' | 'function' | 'method') ('void' | type) subroutineName
//     '(' parameterList ')'
//     subroutineBody

PossibleTokens subroutine_type_keywords[SUBROUTINE_TYPE_KEYWORDS_SIZE] = {
    {"constructor", DATA},
    {"function", DATA},
    {"method", DATA},
};

PossibleTokens
    subroutine_return_type_keywords[SUBROUTINE_RETURN_TYPE_KEYWORDS_SIZE] = {
        {"int", DATA},
        {"char", DATA},
        {"boolean", DATA},
        {"identifier", TYPE},
        {"void", DATA}};

void compile_subroutine_dec() {

  while (
      is_next_any_of(subroutine_type_keywords, SUBROUTINE_TYPE_KEYWORDS_SIZE)) {
    // Opening Tag
    write_tag_to_file("<subroutineDec>");

    // (constructor | function | method)
    eat_any(subroutine_type_keywords, SUBROUTINE_TYPE_KEYWORDS_SIZE);
    write_buffer_to_file();

    // (void | type), type -> className, int, char, boolean
    eat_any(subroutine_return_type_keywords,
            SUBROUTINE_RETURN_TYPE_KEYWORDS_SIZE);
    write_buffer_to_file();

    // subroutineName
    eat("identifier", TYPE);
    write_buffer_to_file();

    // Opening bracket for parameterList
    eat("(", DATA);
    write_buffer_to_file();

    // parameterList logic here
    compile_parameter_list();

    // Closing bracket for parameterList
    eat(")", DATA);
    write_buffer_to_file();

    eat("{", DATA);
    write_buffer_to_file();

    // subrotuineBody Logic will go here
    //
    //
    //
    //

    eat("}", DATA);
    write_buffer_to_file();

    write_tag_to_file("</subroutineDec>");
  }
}

// parameterList:
// ((type varName) (',' type varName)*)?
void compile_parameter_list() {
  write_tag_to_file("<parameterList>");

  char *next_line = peek_next_line(DATA);
  if (strcmp(next_line, ")") != 0) {

    // type -> className, int, char, boolean
    eat_any(var_type_keywords, VAR_TYPE_KEYWORDS_SIZE);
    write_buffer_to_file();

    // varName
    eat("identifier", TYPE);
    write_buffer_to_file();

    char *next_token = peek_next_line(DATA);

    while (strcmp(",", next_token) == 0) {
      // Next parameter
      eat(",", DATA);
      write_buffer_to_file();

      // int, char, bool, className?
      eat_any(var_type_keywords, VAR_TYPE_KEYWORDS_SIZE);
      write_buffer_to_file();

      // varName
      eat("identifier", TYPE);
      write_buffer_to_file();

      free(next_token);
      next_token = peek_next_line(DATA);
    }

    free(next_token);
  }

  write_tag_to_file("</parameterList>");
}
