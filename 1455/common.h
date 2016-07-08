#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 255


unsigned int remove_newline(char *line) {
  for (unsigned int i=0; i<MAX_LINE; i++) {
    if (line[i] == '\r' || line[i] == '\n') {
      line[i] = '\0';
      return i;
    }
  }
  return -1;
}
