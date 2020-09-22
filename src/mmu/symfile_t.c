#include "mort/mmu/symfile_t.h"

#include <stdio.h>

Symfile_t *NewSymfile() {
  /* Allocate and set the defaults for a new Symfile_t */
  Symfile_t *symfile = calloc(1, sizeof(symfile));

  if (symfile) {
    symfile->symbols = calloc(DEFAULT_SYMFILE_MASK, sizeof(Symbol_t));

    if (!symfile->symbols) {
      free(symfile);
      return NULL;
    }

    symfile->mask = DEFAULT_SYMFILE_MASK;
  }

  return symfile;
}

typedef unsigned long Hash_t;


#define DJB2_MAGIC_NUMBER 5381

static Hash_t djb2Hash(Hash_t hash, const char *key) {
  /* Generate a hash using the djb2 hashing algorithm */
  int c;
  while ((c = *key++)) {
    hash = (hash << 5) + hash + c;
  }

  return hash;
}

/* djb2: Generate a hash using the djb2 hashing algorithm, using the
DJB2_MAGIC_NUMBER */
#define djb2(key) djb2Hash(DJB2_MAGIC_NUMBER, key)

#define SYMBOL_COLLISION_LIMIT 3

Symbol_t *SymfileSet(Symfile_t *symfile, Symbol_t symbol) {
  /* Set symbol at the correct index in symfile->symbols. Returns NULL on
  failure i.e. 3 collisions - if this happens we should rehash the symbols. */
  Hash_t index = DJB2_MAGIC_NUMBER;

  int collisions;
  for (collisions = SYMBOL_COLLISION_LIMIT; collisions; collisions -= 1) {
    puts(".");
    index = (djb2Hash(index, symbol.key) & symfile->mask);

    Symbol_t *target = &(symfile->symbols[index]);
    if (!target->address) {
      *target = symbol;
      return target;
    }
  }

  puts("3 collisions - not set");
  return NULL;
}

Symfile_t *OpenSymfile(char *path) {
  /* OpenSymfile creates and returns new Symfile_t object by parsing the file at
   path. Returns NULL on failure. */
  Symfile_t *symfile = NewSymfile();

  if (symfile) {
    char buffer[MAX_SYMFILE_LINE_LENGTH];

    FILE *stream = fopen(path, "r");

    while (fgets(buffer, sizeof(buffer), stream)) {
      Symbol_t symbol;
      int count = sscanf(buffer, "%02hhu:%04hX %256s\n", &symbol.bank, &symbol.address, symbol.key);

      if (count != 3) continue;
      printf("bank: %02hhu, address: 0x%04hX, label: %s\n", symbol.bank, symbol.address, symbol.key);

      SymfileSet(symfile, symbol);
    }

    fclose(stream);
  }

  return symfile;
}
