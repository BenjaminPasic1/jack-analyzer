#include "../../include/parser/compile_subroutine_dec.h"
#include "../../include/parser/jack_parser.h"

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
    // (constructor | function | method)

    eat_any(subroutine_type_keywords, SUBROUTINE_TYPE_KEYWORDS_SIZE);
    write_buffer_to_file();

    // (void | type), type = int, char, bool, className

    eat_any(subroutine_return_type_keywords,
            SUBROUTINE_RETURN_TYPE_KEYWORDS_SIZE);
    write_buffer_to_file();
  }
}
