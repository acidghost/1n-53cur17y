#include "common.h"

#define SALT "$1$M9"


int main() {
  FILE* pwd_list = fopen("./11575/10000.txt", "r");
  if (!pwd_list) {
    perror("[fopen]");
    return errno;
  }

  char line[MAX_LINE];
  unsigned int i = 0;
  while (!feof(pwd_list) && fgets(line, MAX_LINE - 1, pwd_list)) {
    remove_newline(line);
    char *encrypted = crypt(line, SALT);
    fprintf(stdout, "usr%d:%s%s\n", ++i, SALT, encrypted);
  }

  return 0;
}
