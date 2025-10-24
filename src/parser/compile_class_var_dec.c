#include "../../include/parser/compile_class_var_dec.h"
#include "../../include/parser/jack_parser.h"

#include <stdio.h>

// What's the kind of variable? (static | field)
PossibleTokens var_kind_keyword[VAR_KIND_SIZE] = {{"static", DATA},
                                                  {"field", DATA}};
size_t var_kind_keyword_size =
    sizeof(var_kind_keyword) / sizeof(var_kind_keyword[0]);

//    ('static' | 'field') type varName (',' varName)* ';'
void compile_class_var_dec() {
  // Opening Tag
  write_tag_to_file("<classVarDec>");

  // static | field
  eat_any(var_kind_keyword, var_kind_keyword_size);
  write_buffer_to_file();

  // Closing Tag
  write_tag_to_file("</classVarDec>");
}
