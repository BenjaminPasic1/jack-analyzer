#include "../../include/parser/compile_class_var_dec.h"
#include "../../include/parser/jack_parser.h"
#include <stdio.h>

//  Initial Version
//  class Main{
//    var declarationsclassVarDec:
//    ('static' | 'field') type varName (',' varName)* ';'
//
//    subroutine declarations
//  }

// Should be able to check if next up is class. If it is proceed. Not? quit.

void compile_class() {
  // Skip <tokens> tag
  skip_line();

  // Opening <class> tag
  write_tag_to_file("<class>");

  eat("class", DATA);
  write_buffer_to_file();

  eat("identifier", TYPE);
  write_buffer_to_file();

  eat("{", DATA);
  write_buffer_to_file();

  // Class Variable Declarations function goes here.
  compile_class_var_dec();

  eat("}", DATA);
  write_buffer_to_file();

  // Skip closing </tokens> tag
  skip_line();

  write_tag_to_file("</class>");
}
