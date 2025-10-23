#include <stdio.h>
#include <stdlib.h>

void fatal_error(char *error) {
  fprintf(stderr, "%s\n", error);
  exit(EXIT_FAILURE);
}
