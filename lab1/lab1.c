#define _GNU_SOURCE // for getline()
#include <stdio.h>
#include <stdlib.h> // for EXIT_FAILURE, free()
#include <string.h>
int main() {
  // for getline()
  char *line = NULL;
  size_t len = 0;

  // for strtok_r()
  char *token;
  char *saveptr;

  printf("Please enter some text: ");
  getline(&line, &len, stdin); // get line from stdin with length len(automatic)
  if (len == -1) {
    perror("getline failed");
    exit(EXIT_FAILURE);
  }

  token = strtok_r(
      line, " ", &saveptr); // takeout" " from line and add to saveptr as tokens

  printf("Tokens:\n");
  while (token != NULL) {
    printf(" %s\n", token);                //%s is for string output
    token = strtok_r(NULL, " ", &saveptr); // go to next token
  }
  free(line);
  return 0;
}
