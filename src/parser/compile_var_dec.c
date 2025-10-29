#include "../../include/parser/compile_var_dec.h"
#include "../../include/parser/jack_parser.h"
#include <stdlib.h>
#include <string.h>

// var type varName (',' varName)* ';'
void compile_var_dec() {

  // Subroutine var declarations are optional
  if (!is_next("var", DATA)) {
    // No var declarations
    return;
  }

  while (is_next("var", DATA)) {
    write_tag_to_file("<varDec>");

    eat("var", DATA);
    write_buffer_to_file();

    eat_any(var_type_keywords, VAR_TYPE_KEYWORDS_SIZE);
    write_buffer_to_file();

    check_for_square_brackets_only();

    eat("identifier", TYPE);
    write_buffer_to_file();

    while (is_next(",", DATA)) {
      eat(",", DATA);
      write_buffer_to_file();

      eat("identifier", TYPE);
      write_buffer_to_file();
    }

    eat(";", DATA);
    write_buffer_to_file();

    write_tag_to_file("</varDec>");
  }
}
