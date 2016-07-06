#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 255


int main(int argc, char const *argv[]) {
  if (argc != 2) {
    fprintf(stdout, "Usage: %s passwd-path\n", argv[0]);
    return 0;
  }

  FILE *dict_file = fopen("./11575/merged.lst", "r");
  if (!dict_file) {
    perror("[fopen] dictionary file");
    return errno;
  }

  FILE *pwd_hash_file = fopen(argv[1], "r");
  if (!pwd_hash_file) {
    perror("[fopen] hashes file");
    return errno;
  }

  fclose(pwd_hash_file);
  fclose(dict_file);
  return 0;
}
