#define _GNU_SOURCE
#include <stdio.h>    // default
#include <stdlib.h>   // for free(), EXIT_FAILURE
#include <string.h>   // strtok_r
#include <sys/wait.h> // for waitpid
#include <unistd.h>   // for fork, execl

int main() {
  // for getline
  char *line = NULL;
  size_t len = 0;

  // for fork()
  pid_t pid;
  printf("Enter programs to run.\n");

  while (1) {
    printf("> ");
    ssize_t nread = getline(&line, &len, stdin);
    if (nread == -1) {
      // error
      break;
    }

    // Remove \n
    if (nread > 0 && line[nread - 1] == '\n') {
      line[nread - 1] = '\0';
    }

    // strtok_r input line spaces
    char *token;
    char *saveptr;
    token = strtok_r(line, " ", &saveptr);

    if (token == NULL) {
      // Empty input, skip
      continue;
    }

    pid = fork();
    if (pid < 0) {
      perror("fork failed");
      continue;
    }

    if (pid == 0) {
      // Child process. execl
      execl(token, token, (char *)NULL);
      // runs if there is error( This line shouldn't run if successful)
      printf("Exec failure\n");
      exit(EXIT_FAILURE);
    } else {
      // Parent process. wait until child finishes (0)
      int status;
      waitpid(pid, &status, 0);
    }
  }

  free(line);
  return 0;
}
