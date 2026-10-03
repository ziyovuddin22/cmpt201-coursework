#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 5

static void add_to_history(char *history[], int *next, int *count, char *line) {
  free(history[*next]);
  history[*next] = line;
  *next = (*next + 1) % HISTORY_SIZE;
  if (*count < HISTORY_SIZE) {
    (*count)++;
  }
}

static void print_history(char *history[], int next, int count) {
  int start = (next - count + HISTORY_SIZE) % HISTORY_SIZE;
  for (int i = 0; i < count; i++) {
    printf("%s\n", history[(start + i) % HISTORY_SIZE]);
  }
}

static void free_history(char *history[]) {
  for (int i = 0; i < HISTORY_SIZE; i++) {
    free(history[i]);
  }
}

int main(void) {
  char *history[HISTORY_SIZE] = {NULL};
  int next = 0;
  int count = 0;

  while (1) {
    printf("Enter input: ");
    fflush(stdout);

    char *line = NULL;
    size_t cap = 0;
    if (getline(&line, &cap, stdin) == -1) {
      free(line);
      break;
    }

    line[strcspn(line, "\n")] = '\0';
    add_to_history(history, &next, &count, line);

    if (strcmp(line, "print") == 0) {
      print_history(history, next, count);
    }
  }

  free_history(history);
  return 0;
}
