#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 255


void remove_newline(char *line) {
  for (unsigned int i=0; i<MAX_LINE; i++) {
    if (line[i] == '\n') {
      line[i] = '\0';
      return;
    }
  }
}
