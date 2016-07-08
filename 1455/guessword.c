#include "common.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LST 214972


typedef unsigned char byte;

typedef struct user_hash {
  char user[MAX_LINE];
  char salt[MAX_LINE];
  char hash[MAX_LINE];
} user_hash_t;

typedef struct plain_hash {
  char *plain;
  char *hash;
} plain_hash_t;


user_hash_t *parse_user_hash(char *line) {
  user_hash_t *uh = (user_hash_t *) malloc(sizeof(user_hash_t));
  unsigned int line_pointer;
  for (line_pointer = 0; line_pointer < MAX_LINE; line_pointer++) {
    if (line[line_pointer] == ':') {    // skip column sign
      uh->user[line_pointer] = '\0';
      line_pointer++;
      break;
    }
    uh->user[line_pointer] = line[line_pointer];
  }
  byte found_dolrs = 0;
  for (unsigned int salt_ptr = 0; line_pointer < MAX_LINE; line_pointer++, salt_ptr++) {
    if (line[line_pointer] == '$') {
      found_dolrs++;
      if (found_dolrs == 3) {   // skip 3rd dlr sign
        uh->salt[salt_ptr] = '\0';
        line_pointer++;
        break;
      }
    }
    uh->salt[salt_ptr] = line[line_pointer];
  }
  for (unsigned int hash_pointer = 0; line_pointer < MAX_LINE; line_pointer++, hash_pointer++) {
    if (line[line_pointer] == '\n') {
      uh->hash[hash_pointer] = '\0';
      break;
    }
    uh->hash[hash_pointer] = line[line_pointer];
  }
  return uh;
}


int compare_plain_hash(const void *ph1, const void *ph2) {
  plain_hash_t *cph1 = (plain_hash_t *) ph1;
  plain_hash_t *cph2 = (plain_hash_t *) ph2;
  return strcmp(cph1->hash, cph2->hash);
}


plain_hash_t *build_dictionary(char *filename, char *salt) {
  // open dictionary file
  FILE *dict_file = fopen(filename, "r");
  if (!dict_file) {
    perror("[fopen] dictionary file");
    exit(errno);
    return NULL;
  }

  // parse dictionary file and build lookup table
  char dict_line[MAX_LINE];
  plain_hash_t *table = malloc(MAX_LST * sizeof(plain_hash_t));
  for (unsigned int i = 0; i < MAX_LST; i++) {
    if (!feof(dict_file) && fgets(dict_line, MAX_LINE - 1, dict_file)) {
      unsigned int len = remove_newline(dict_line);
      table[i] = (plain_hash_t) {
        .plain=(char *) malloc(len * sizeof(char)),
        .hash=(char *) malloc(MAX_LINE * sizeof(char))
      };
      strncpy(table[i].plain, dict_line, len * sizeof(char));
      strcpy(table[i].hash, crypt(dict_line, salt));
    } else {
      fprintf(stderr, "Unexpected end of dictionary file or reading error\n");
      exit(EXIT_FAILURE);
      return NULL;
    }
  }

  fclose(dict_file);
  return table;
}


int main(int argc, char const *argv[]) {
  if (argc != 2) {
    fprintf(stdout, "Usage: %s passwd-path\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // open passwd file
  FILE *pwd_hash_file = fopen(argv[1], "r");
  if (!pwd_hash_file) {
    perror("[fopen] hashes file");
    exit(errno);
  }

  // parse first line to get the salt (equal for all)
  char pwd_line[MAX_LINE];
  fgets(pwd_line, MAX_LINE - 1, pwd_hash_file);
  user_hash_t *first_uh = parse_user_hash(pwd_line);

  plain_hash_t *table = build_dictionary("./11575/merged.lst", first_uh->salt);

  qsort(table, MAX_LST, sizeof(plain_hash_t), compare_plain_hash);

  // parse passwd file
  while (!feof(pwd_hash_file) && fgets(pwd_line, MAX_LINE - 1, pwd_hash_file)) {
    user_hash_t *uh = parse_user_hash(pwd_line);
    plain_hash_t ph = (plain_hash_t) {
      .plain=NULL,
      .hash=uh->hash
    };
    plain_hash_t *res = (plain_hash_t *) bsearch(&ph, table, MAX_LST, sizeof(plain_hash_t), compare_plain_hash);
    if (res) {
      printf("%s:%s\n", uh->user, res->plain);
    }
    // fprintf(stdout, "%s\t%s\t%s\n", uh->user, uh->salt, uh->hash);
    free(uh);
  }

  fclose(pwd_hash_file);

  exit(EXIT_SUCCESS);
}
