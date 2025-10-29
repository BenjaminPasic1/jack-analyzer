#include "../../include/parser/compile_var_dec.h"
#include "../../include/parser/jack_parser.h"
#include <stdlib.h>
#include <string.h>

// var type varName (',' varName)* ';'
void compile_var_dec() {

  // Subroutine var declarations are optional
  char *var_keyword = peek_next_line(DATA);
  if (strcmp(var_keyword, "var") != 0) {
    free(var_keyword);
    // No var declarations
    return;
  }

  while (strcmp(var_keyword, "var") == 0) {
    write_tag_to_file("<varDec>");

    eat("var", DATA);
    write_buffer_to_file();

    eat_any(var_type_keywords, VAR_TYPE_KEYWORDS_SIZE);
    write_buffer_to_file();

    check_for_square_brackets_only();

    eat("identifier", TYPE);
    write_buffer_to_file();

    char *next_line = peek_next_line(DATA);
    while (strcmp(next_line, ",") == 0) {
      eat(",", DATA);
      write_buffer_to_file();

      eat("identifier", TYPE);
      write_buffer_to_file();

      free(next_line);
      next_line = peek_next_line(DATA);
    }

    eat(";", DATA);
    write_buffer_to_file();

    write_tag_to_file("</varDec>");

    free(var_keyword);
    var_keyword = peek_next_line(DATA);
  }
}
