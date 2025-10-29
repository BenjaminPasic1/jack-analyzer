#include "../../include/parser/compile_statements.h"
#include "../../include/parser/compile_expression.h"
#include "../../include/parser/jack_parser.h"

#include <string.h>

PossibleTokens statement_keywords[STATEMENT_KEYWORDS_SIZE] = {
    {"let", DATA}, {"if", DATA},     {"while", DATA},
    {"do", DATA},  {"return", DATA},
};

// statements:
//     statement*

// statement:
//     letStatement | ifStatement | whileStatement | doStatement |
//     returnStatement
void compile_statements() {
  write_tag_to_file("<statements>");

  // check if there are any statements
  if (!is_next_any_of(statement_keywords, STATEMENT_KEYWORDS_SIZE)) {
    write_tag_to_file("</statements>");
    return;
  }

  while (is_next_any_of(statement_keywords, STATEMENT_KEYWORDS_SIZE)) {

    if (is_next("let", DATA)) {
      compile_let_statement();
    } else if (is_next("if", DATA)) {
      void compile_if_statement();
    }
  }
}

// ifStatement → 'if' '(' expression ')' '{' statements '}' ('else' '{'
// statements '}')?
void compile_if_statement() {
  write_tag_to_file("<ifStatement>");

  eat("if", DATA);
  write_buffer_to_file();

  eat("(", DATA);
  write_buffer_to_file();

  compile_expression();

  eat(")", DATA);
  write_buffer_to_file();

  eat("{", DATA);
  write_buffer_to_file();

  compile_statements();

  eat("}", DATA);
  write_buffer_to_file();

  // Optionally there is a single else
  if (is_next("else", DATA)) {
    eat("else", DATA);
    write_buffer_to_file();

    eat("{", DATA);
    write_buffer_to_file();

    compile_statements();

    eat("}", DATA);
    write_buffer_to_file();
  }

  write_tag_to_file("</ifStatement>");
}

// letStatement: let varName ('[' expression ']')? = expression ';'
void compile_let_statement() {
  write_tag_to_file("<letStatement>");

  // let
  eat("let", DATA);
  write_buffer_to_file();

  // varName
  eat("identifier", TYPE);
  write_buffer_to_file();

  // (['expression']) ?
  if (is_next("[", DATA)) {
    eat("[", DATA);
    write_buffer_to_file();

    compile_expression();

    eat("]", DATA);
    write_buffer_to_file();
  }

  eat("=", DATA);
  write_buffer_to_file();

  compile_expression();

  eat(";", DATA);
  write_buffer_to_file();

  write_tag_to_file("</letStatement>");
}
