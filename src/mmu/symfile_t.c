#include <stdio.h>

#include "mort/mmu/symfile_t.h"

/* constants */

#define DJB2_MAGIC_NUMBER 5381

#define SYMBOL_COLLISION_LIMIT 3

/* methods */

static Symfile_t *new_symfile() {
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

static unsigned long djb2_hash(unsigned long hash, const char *key) {
  /* Generate a unsigned long using the djb2 hashing algorithm */
  int c;
  while ((c = *key++)) {
    hash = (hash << 5) + hash + c;
  }

  return hash;
}

static Symbol_t *set_symbol(Symfile_t *symfile, Symbol_t symbol) {
  /* Set symbol at the correct index in symfile->symbols. Returns NULL on
  failure i.e. 3 collisions - if this happens we should rehash the symbols. */
  unsigned long index = DJB2_MAGIC_NUMBER;

  int collisions;
  for (collisions = SYMBOL_COLLISION_LIMIT; collisions; collisions -= 1) {
    puts(".");
    index = (djb2_hash(index, symbol.key) & symfile->mask);

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
  Symfile_t *symfile = new_symfile();

  if (symfile) {
    char buffer[MAX_SYMFILE_LINE_LENGTH];

    FILE *stream = fopen(path, "r");

    while (fgets(buffer, sizeof(buffer), stream)) {
      Symbol_t symbol;
      int count = sscanf(buffer, "%02hhu:%04hX %256s\n", &symbol.bank, &symbol.address, symbol.key);

      if (count != 3) continue;
      printf("bank: %02hhu, address: 0x%04hX, label: %s\n", symbol.bank, symbol.address, symbol.key);

      set_symbol(symfile, symbol);
    }

    fclose(stream);
  }

  return symfile;
}

Symfile_t *CloseSymfile(Symfile_t *symfile) {
  /* Deallocates Symfile_t and its symbols. */
  if (symfile) {
    if (symfile->symbols) free(symfile->symbols);
    
    free(symfile);
  }

  return NULL;
}
