#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/jack_tokenizer.h"

FILE *open_file(char *filename) {
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    perror("ERROR: Error with reading in the file input.\n");
    exit(EXIT_FAILURE);
  }

  return file;
}

FILE *create_output_file(char *filename) {
  FILE *out_file = fopen(filename, "w");

  if (out_file == NULL) {
    perror("ERROR: Error creating token outut file! Exiting...\n");
    exit(EXIT_FAILURE);
  }

  return out_file;
}

FILE *tokenize_to_xml(FILE *source_file, HashMap *jack_tokens) {
  FILE *output_file = create_output_file("tokens_output.xml");

  fprintf(output_file, "<tokens>\n");

  while (has_next_line(source_file)) {
    char *line = get_next_line(source_file);

    // Keep track of the beginning of the assigned memory
    // for line, since we will manipulate it and lose the initial reference
    char *line_start = line;

    if (*line == '\0' || *line == '/') {
      free(line);
      continue;
    }

    while (has_next_token(&line)) {
      char *token = get_next_token(&line, jack_tokens);
      Token *token_details = get_token_details(jack_tokens, token);
      write_token_to_output(token_details, output_file);

      free(token);
      free(token_details);
    }

    free(line_start);
  }

  fprintf(output_file, "</tokens>");

  return output_file;
}

HashMap *generate_token_hashmap() {
  HashMap *map = create_hashmap(HASHMAP_SIZE);

  // Keywords
  insert_hashmap(map, "class", "class");
  insert_hashmap(map, "constructor", "constructor");
  insert_hashmap(map, "function", "function");
  insert_hashmap(map, "method", "method");
  insert_hashmap(map, "int", "int");
  insert_hashmap(map, "boolean", "boolean");
  insert_hashmap(map, "char", "char");
  insert_hashmap(map, "void", "void");
  insert_hashmap(map, "var", "var");
  insert_hashmap(map, "static", "static");
  insert_hashmap(map, "field", "field");
  insert_hashmap(map, "let", "let");
  insert_hashmap(map, "do", "do");
  insert_hashmap(map, "if", "if");
  insert_hashmap(map, "else", "else");
  insert_hashmap(map, "while", "while");
  insert_hashmap(map, "return", "return");
  insert_hashmap(map, "true", "true");
  insert_hashmap(map, "false", "false");
  insert_hashmap(map, "null", "null");
  insert_hashmap(map, "this", "this");

  // Symbols
  insert_hashmap(map, "{", "{");
  insert_hashmap(map, "}", "}");
  insert_hashmap(map, "(", "(");
  insert_hashmap(map, ")", ")");
  insert_hashmap(map, "[", "[");
  insert_hashmap(map, "]", "]");
  insert_hashmap(map, ".", ".");
  insert_hashmap(map, ",", ",");
  insert_hashmap(map, ";", ";");
  insert_hashmap(map, "+", "+");
  insert_hashmap(map, "-", "-");
  insert_hashmap(map, "*", "*");
  insert_hashmap(map, "/", "/");
  insert_hashmap(map, "&", "&");
  insert_hashmap(map, "|", "|");
  insert_hashmap(map, "<", "<");
  insert_hashmap(map, ">", ">");
  insert_hashmap(map, "=", "=");

  return map;
}

int has_next_line(FILE *file) {
  int ch = fgetc(file);

  if (ch == EOF) {
    return 0;
  }

  ungetc(ch, file);
  return 1;
}

int has_next_token(char **current_line) {

  if (**current_line == '\0') {
    return 0;
  }

  return 1;
}

char *get_next_token(char **current_line, HashMap *jack_tokens) {
  char *token = calloc(MAX_TOKEN_SIZE, sizeof(char));
  int len = 0;

  // if there are multiple spaces, or the pointer is left on a space -> skip
  while (**current_line == ' ')
    (*current_line)++;

  // If it's a string, grab it
  if (**current_line == '"') {
    token[len++] = **current_line;
    (*current_line)++;

    while (**current_line != '"' && **current_line != '\0') {
      token[len++] = **current_line;
      (*current_line)++;
    }

    // include closing quote
    if (**current_line == '"') {
      token[len++] = **current_line;
      (*current_line)++;
    }

    token[len] = '\0';
    return token;
  }

  // check char by char, if a token is encountered return current strings
  // and continue where the pointer is left at.
  while (**current_line != '\0' && **current_line != ' ') {

    // map implementation works with strings instead of chars.
    char single_char_str[2] = {**current_line, '\0'};

    // token encountered
    if (has_key(jack_tokens, single_char_str)) {
      // return token not empty? null term and return
      if (len > 0) {
        token[len] = '\0';
        return token;
      }

      token[0] = **current_line;
      token[1] = '\0';
      (*current_line)++;
      return token;
    }

    token[len++] = **current_line;
    (*current_line)++;
  }

  token[len] = '\0';
  return token;
}

char *get_next_line(FILE *file) {
  char *line = malloc(MAX_LINE_SIZE);

  if (!line) {
    perror("ERROR: Failed to allocate memory for line string.");
    exit(EXIT_FAILURE);
  }

  fgets(line, MAX_LINE_SIZE, file);

  trim(line);

  return line;
}

void trim(char *line) {
  char *start = line;
  char *end = line + strlen(line) - 1;

  while (*start && isspace((unsigned char)*start))
    start++;

  while (end >= start && isspace((unsigned char)*end))
    end--;

  size_t len = end - start + 1;
  memmove(line, start, len);
  line[len] = '\0';
}

Token *get_token_details(HashMap *map, char *token) {
  Token *t = malloc(sizeof(Token));

  if (!t) {
    perror("ERROR: Couldn't allocate memeory for token. Exiting...");
    exit(EXIT_FAILURE);
  }

  // Get token details and return it!

  // if it's a string
  if (*token == '"') {
    token++;
    token[strlen(token) - 1] = '\0';
    t->token = strdup(token);
    t->type = STR_CONSTANT;
    return t;
  }

  if (strlen(token) > 1 && has_key(map, token)) {
    t->token = strdup(token);
    t->type = KEYWORD;
    return t;
  }

  if (strlen(token) == 1 && has_key(map, token)) {
    t->token = strdup(token);
    t->type = SYMBOL;
    return t;
  }

  if (is_integer(token)) {
    t->token = strdup(token);
    t->type = INT_CONSTANT;
    return t;
  }

  t->token = strdup(token);
  t->type = IDENTIFIER;
  return t;
}

int is_integer(char *token) {
  if (*token == '\0')
    return 0;

  if (*token == '-' || *token == '+')
    token++;

  if (*token == '\0')
    return 0;

  while (*token) {
    if (!isdigit(*token))
      return 0;
    token++;
  }

  return 1;
}

void write_token_to_output(Token *token, FILE *output_file) {

  switch (token->type) {
  case STR_CONSTANT:
    fprintf(output_file, "<stringConstant>%s</stringConstant>\n", token->token);
    break;
  case INT_CONSTANT:
    fprintf(output_file, "<integerConstant>%s</integerConstant>\n",
            token->token);
    break;
  case KEYWORD:
    fprintf(output_file, "<keyword>%s</keyword>\n", token->token);
    break;
  case SYMBOL:

    if (strcmp(token->token, "<") == 0) {
      fprintf(output_file, "<symbol>&lt;</symbol>\n");
    } else if (strcmp(token->token, ">") == 0) {
      fprintf(output_file, "<symbol>&gt;</symbol>\n");
    } else if (strcmp(token->token, "&") == 0) {
      fprintf(output_file, "<symbol>&amp;</symbol>\n");
    } else {
      fprintf(output_file, "<symbol>%s</symbol>\n", token->token);
    }

    break;
  case IDENTIFIER:
    fprintf(output_file, "<identifier>%s</identifier>\n", token->token);
    break;
  }
}
