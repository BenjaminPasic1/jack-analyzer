#include "../../include/parser/compile_expression.h"
#include "../../include/parser/jack_parser.h"

#include <stdlib.h>
#include <string.h>

PossibleTokens term_keywords[TERM_KEYWORDS_SIZE] = {
    {"integerConstant", TYPE}, {"stringConstant", TYPE}, {"identifier", TYPE}};

PossibleTokens operator_keywords[OPERATOR_KEYWORDS_SIZE] = {
    {"+", DATA}, {"-", DATA},    {"*", DATA},    {"/", DATA}, {"&amp;", DATA},
    {"|", DATA}, {"&lt;", DATA}, {"&gt;", DATA}, {"=", DATA},
};

PossibleTokens constant_keywords[CONSTANT_KEYWORDS_SIZE] = {
    {"true", DATA}, {"false", DATA}, {"null", DATA}, {"this", DATA}};

PossibleTokens unary_operator_keywords[UNARY_OPERATOR_KEYWORDS_SIZE] = {
    {"-", DATA},
    {"~", DATA},
};

// expression: term (op term)*
// term: integerConstant | stringConstant | keywordConstant | varName | varName
// '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
// op: '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
// unaryOp: '-' | '~'
// keywordConstant: 'true' | 'false' | 'null' | 'this'
// subroutineCall: Output.printInt(x) or someMethod()

void compile_expression() {

  write_tag_to_file("<expression>");

  // Always start with a term
  compile_term();

  // In case there are more terms, call compile_term again
  // Based on if there are any
  while (is_next_any_of(operator_keywords, OPERATOR_KEYWORDS_SIZE)) {
    eat_any(operator_keywords, OPERATOR_KEYWORDS_SIZE);
    write_buffer_to_file();

    compile_term();
  }

  write_tag_to_file("</expression>");
}

void compile_term() {
  write_tag_to_file("<term>");

  // Check if unary op comes before the actual term
  if (is_next_any_of(unary_operator_keywords, UNARY_OPERATOR_KEYWORDS_SIZE)) {
    eat_any(unary_operator_keywords, UNARY_OPERATOR_KEYWORDS_SIZE);
    write_buffer_to_file();
  }

  // Check parantheses first
  if (is_next("(", DATA)) {
    eat("(", DATA);
    write_buffer_to_file();

    compile_expression();

    eat(")", DATA);
    write_buffer_to_file();

  } else if (is_next_any_of(constant_keywords, CONSTANT_KEYWORDS_SIZE)) {
    eat_any(constant_keywords, CONSTANT_KEYWORDS_SIZE);
    write_buffer_to_file();
  } else if (is_next_any_of(term_keywords, TERM_KEYWORDS_SIZE)) {
    eat_any(term_keywords, TERM_KEYWORDS_SIZE);
    write_buffer_to_file();
  } else if (is_next("identifier", TYPE)) { // identifier

    // varName
    eat("identifier", TYPE);
    write_buffer_to_file();

    // Identifer can be an array or call to a subroutine later on.
    // Array access
    if (is_next("[", DATA)) {
      eat("[", DATA);
      write_buffer_to_file();

      compile_expression();

      eat("]", DATA);
      write_buffer_to_file();
    } else if (is_next("(", DATA)) {

      eat("(", DATA);
      write_buffer_to_file();

      // compile_expression_list goes here
      compile_expression_list();

      eat(")", DATA);
      write_buffer_to_file();
    } else if (is_next(".", DATA)) {
      eat(".", DATA);
      write_buffer_to_file();

      eat("identifier", TYPE);
      write_buffer_to_file();

      eat("(", DATA);
      write_buffer_to_file();

      // compile_expression_list goes here
      compile_expression_list();

      eat(")", DATA);
      write_buffer_to_file();
    }
  }

  write_tag_to_file("</term>");
}
// expressionList → (expression (',' expression)*)?
// expression     → term (op term)*
void compile_expression_list() {
  write_tag_to_file("<expressionList>");

  if (!is_next(")", DATA)) {
    compile_expression();

    while (is_next(",", DATA)) {
      eat(",", DATA);
      write_buffer_to_file();

      compile_expression();
    }
  }

  write_tag_to_file("</expressionList>");
}
