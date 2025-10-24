#include "../../include/parser/compile_class_var_dec.h"
#include "../../include/parser/jack_parser.h"

#include <stdio.h>

char *field_type_keywords[FIELD_TYPE_SIZE] = {"static", "field"};

//    ('static' | 'field') type varName (',' varName)* ';'
void compile_class_var_dec() {}
