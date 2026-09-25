#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char *line = NULL; // getline() will allocate a buffer for us
  size_t len = 0;    // and track its size
  ssize_t nread;

  while (1) {
    printf("Enter programs to run.\n> ");
    fflush(stdout); // make sure the prompt shows up before we block on input

    nread = getline(&line, &len, stdin);
    if (nread == -1) {
      // EOF (Ctrl+D) or a read error - either way, stop looping
      break;
    }

    // getline() keeps the trailing '\n' - strip it off
    if (nread > 0 && line[nread - 1] == '\n') {
      line[nread - 1] = '\0';
    }

    // Optional: skip empty input (just pressing Enter) instead of
    // trying to fork/exec a program with no name.
    if (line[0] == '\0') {
      continue;
    }

    pid_t pid = fork();

    if (pid < 0) {
      // fork() failed
      perror("fork");
      free(line);
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      // ---- child process ----
      // execlp(file, arg0, ..., NULL)
      // arg0 (the "name" of the program) is passed by convention;
      // we don't support extra command-line arguments here.
      execlp(line, line, (char *)NULL);

      // If execlp() returns at all, it failed
      printf("Exec failure\n");
      exit(EXIT_FAILURE);
    } else {
      // ---- parent process ----
      int status;
      if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
      }
    }
  }

  free(line); // getline() may have allocated memory for us - clean it up
  return 0;
}
