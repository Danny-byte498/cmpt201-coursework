#define _GNU_SOURCE // for getline(), strdup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_history(char *history[], int count, const int size) {
  int start;
  if (count >= size) {    // if >= 5 lines have been entered
    start = count - size; // start at new start
  } else {
    start = 0; // else if < 5 lines, start at 0
  }

  for (int i = start; i < count; ++i) {
    int index = i % size;
    printf("%s", history[index]); // print line at index location
  }
}

void free_history(char *history[],
                  const int size) { // free to prevent memory leak
  for (int i = 0; i < size; ++i) {
    free(history[i]);
  }
}

int main() {
  const int size = 5;
  char *history[5] = {NULL}; // initialize history array of char(size 5) to Null
  int count = 0;             // lines entered by user

  // variables for using getline()
  char *line = NULL;
  size_t len = 0;

  while (1) {
    printf("Enter input: ");
    ssize_t read = getline(&line, &len, stdin); // get input (stdin)
    if (read == -1)
      break; // if ctrl+c, break

    int index =
        count % size; // use circular array as we are updating LILO order

    if (history[index] !=
        NULL) {             // if space is already occupied with pervious lines
      free(history[index]); // free previous memory to prevent memory leak
    }

    history[index] =
        strdup(line); // copy new line to space where memory was just free
    count++;          // add line

    if (strcmp(line, "print\n") == 0) {
      print_history(history, count, size);
    }
  }

  free_history(history, size);
  free(line);
  return 0;
}
