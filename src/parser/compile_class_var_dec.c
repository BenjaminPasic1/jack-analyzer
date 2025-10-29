#include "../../include/parser/compile_class_var_dec.h"
#include "../../include/parser/jack_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// What's the kind of variable? (static | field)
PossibleTokens var_kind_keyword[VAR_KIND_SIZE] = {{"static", DATA},
                                                  {"field", DATA}};
size_t var_kind_keyword_size =
    sizeof(var_kind_keyword) / sizeof(var_kind_keyword[0]);

//    ('static' | 'field') type varName (',' varName)* ';'
void compile_class_var_dec() {

  // A class can have 0 variable delclarations
  if (!is_next_any_of(var_kind_keyword, VAR_KIND_SIZE)) {
    printf(
        "[DEBUG] -> compile_class_var_dec: NO CLASS VARIABLE DEFINITIONS!\n");
    return;
  }

  while (is_next_any_of(var_kind_keyword, VAR_KIND_SIZE)) {
    // Opening Tag
    write_tag_to_file("<classVarDec>");
    // static | field
    eat_any(var_kind_keyword, var_kind_keyword_size);
    write_buffer_to_file();

    // type -> int, char, boolean, or user defined className
    eat_any(var_type_keywords, VAR_TYPE_KEYWORDS_SIZE);
    write_buffer_to_file();

    // identifier can be an array declaration as well. -> numbers[]
    check_for_square_brackets_only();

    // varName
    eat("identifier", TYPE);
    write_buffer_to_file();

    // Peek next
    char *next_token = peek_next_line(DATA);

    // If it's a comma there are more varNames coming up, loop untill ;
    while (strcmp(",", next_token) == 0) {
      // Comma
      eat(",", DATA);
      write_buffer_to_file();

      // Identifier
      eat("identifier", TYPE);
      write_buffer_to_file();

      free(next_token);
      next_token = peek_next_line(DATA);
    }

    free(next_token);

    eat(";", DATA);
    write_buffer_to_file();
    // Closing Tag
    write_tag_to_file("</classVarDec>");
  }
}
