#include "./include/jack_tokenizer.h"
#include "include/hashmap.h"
#include "include/jack_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  FILE *source_code = open_file("test.txt");
  HashMap *jack_tokens = generate_token_hashmap();

  // OPTIONAL
  // Get all tokens in XML format, used for testing if we get the correct output
  // for tokens
  FILE *tokens_xml = tokenize_to_xml(source_code, jack_tokens);

  fclose(tokens_xml);

  // Actual parse tree of the source code.
  FILE *source_xml = parse_to_xml();

  fclose(source_code);
}
