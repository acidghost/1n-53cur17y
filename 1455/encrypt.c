#include <errno.h>
#include <stdio.h>

#define MAX_LINE 255


void remove_newline(char *line) {
  for (unsigned int i=0; i<MAX_LINE; i++) {
    if (line[i] == '\n') {
      line[i] = '\0';
      return;
    }
  }
}

int main() {
  FILE* pwd_list = fopen("./11575/10000.txt", "r");
  if (!pwd_list) {
    perror("[fopen]");
    return errno;
  }

  char line[MAX_LINE];
  while (!feof(pwd_list) && fgets(line, MAX_LINE - 1, pwd_list)) {
    remove_newline(line);
    fprintf(stdout, "%s\n", line);
  }

  return 0;
}
