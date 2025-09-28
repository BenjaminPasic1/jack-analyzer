#include "./include/jack_tokenizer.h"
#include "include/hashmap.h"
#include "include/jack_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  FILE *source_code = open_file("test.txt");
  HashMap *jack_tokens = generate_token_hashmap();

  // Get all tokens in XML format, used for testing if we get the correct output
  FILE *token_xml_output = tokenize_to_xml(source_code, jack_tokens);

  fclose(token_xml_output);
  fclose(source_code);
}
