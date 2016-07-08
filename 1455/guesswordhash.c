#include "common.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define SUBT_HASH 5862067UL
#define MAX_HASH 18442910239535856459UL
#define HASHT_SIZE 214972 + 800000


typedef unsigned char byte;

typedef struct user_hash {
  char user[MAX_LINE];
  char salt[MAX_LINE];
  char hash[MAX_LINE];
} user_hash_t;


unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
}

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

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    fprintf(stdout, "Usage: %s passwd-path\n", argv[0]);
    return 0;
  }

  // open dictionary file
  FILE *dict_file = fopen("./11575/merged.lst", "r");
  if (!dict_file) {
    perror("[fopen] dictionary file");
    return errno;
  }

  // open passwd file
  FILE *pwd_hash_file = fopen(argv[1], "r");
  if (!pwd_hash_file) {
    perror("[fopen] hashes file");
    return errno;
  }

  // parse first line to get the salt (equal for all)
  char pwd_line[MAX_LINE];
  fgets(pwd_line, MAX_LINE - 1, pwd_hash_file);
  user_hash_t *first_uh = parse_user_hash(pwd_line);


  // parse dictionary file and build hash table
  char dict_line[MAX_LINE];
  char *hash_table[HASHT_SIZE];
  for (unsigned int i = 0; i < HASHT_SIZE; i++) {
    hash_table[i] = NULL;
  }
  while (!feof(dict_file) && fgets(dict_line, MAX_LINE - 1, dict_file)) {
    unsigned int len = remove_newline(dict_line);
    char *encrypted = crypt(dict_line, first_uh->salt);
    unsigned long h = hash(encrypted) % HASHT_SIZE;
    if (hash_table[h]) {
      // fprintf(stderr, "Hash collision for word %s and %s\n", hash_table[h], dict_line);
      // return -1;
    }
    // printf("Inserting word %s\n", dict_line);
    hash_table[h] = (char *) malloc(len * sizeof(char));
    strncpy(hash_table[h], dict_line, len * sizeof(char));
  }


  // parse passwd file
  while (!feof(pwd_hash_file) && fgets(pwd_line, MAX_LINE - 1, pwd_hash_file)) {
    user_hash_t *uh = parse_user_hash(pwd_line);
    unsigned long h = hash(uh->hash) % HASHT_SIZE;
    if (hash_table[h]) {
      printf("%s:%s\n", uh->user, hash_table[h]);
    }
    // fprintf(stdout, "%s\t%s\t%s\n", uh->user, uh->salt, uh->hash);
    free(uh);
  }


  // close file handlers
  fclose(pwd_hash_file);
  fclose(dict_file);
  return 0;
}
