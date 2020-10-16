#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "mort/types/symfile_t.h"

/* constants */

#define DJB2_MAGIC_NUMBER 5381

#define SYMBOL_COLLISION_LIMIT 3

/* types */

/* symbolCallback_t is the signature for callbacks passed to findSymbol,
tryEachSymbol and forEachSymbol */

typedef Symbol_t *(*symbolCallback_t)(Symfile_t *, Symbol_t *, void *);

/* static methods - forward declarations */

static bool tryEachSymbol(Symfile_t *, symbolCallback_t, void *);
static Symbol_t *copySymbol(Symfile_t *, Symbol_t *, void *);
static Symbol_t *findSymbol(Symfile_t *, symbolCallback_t, void *);
static Symbol_t *setSymbol(Symfile_t *, Symbol_t);
static Symfile_t *growSymfile(Symfile_t *);
static Symfile_t *newSymfile(unsigned int);
static unsigned int djb2Hash(unsigned int, const char *);
static void forEachSymbol(Symfile_t *, symbolCallback_t, void *);

/* static methods - implementations */

static bool tryEachSymbol(Symfile_t *symfile, symbolCallback_t callback, void *probe) {
  /* Iterate over each symbol in symfile and call callback on it. If success is
  NULL, it will break the loop and return that Symbol. probe can be used to pass
  in any extra data. */
  unsigned int i;
  for (i = symfile->mask; i; i--) {
    Symbol_t *symbol = &symfile->symbols[i];

    Symbol_t *success = callback(symfile, symbol, probe);
    if (!success) return false;
  }

  return true;
}

static Symbol_t *copySymbol(Symfile_t *symfile, Symbol_t *symbol, void *probe) {
  /* Copy attempts to set symbol on dst. Returns NULL if fails. */
  if (symbol->label[0] == '\0') return symbol;

  return setSymbol(probe, *symbol);
}

static Symbol_t *findSymbol(Symfile_t *symfile, symbolCallback_t callback, void *probe) {
  /* Iterate over each symbol in symfile and call callback on it. If found is an
  address, it will break the loop and return that Symbol. probe can be used to
  pass in any extra data. */
  unsigned int i;
  for (i = symfile->mask; i; i--) {
    Symbol_t *symbol = &symfile->symbols[i];

    Symbol_t *found = callback(symfile, symbol, probe);
    if (found) return found;
  }

  return NULL;
}

static Symbol_t *setSymbol(Symfile_t *symfile, Symbol_t symbol) {
  /* Set symbol at the correct index in symfile->symbols. Returns NULL on
  failure i.e. 3 collisions - if this happens we should rehash the symbols. */
  unsigned int index = DJB2_MAGIC_NUMBER;

  int collisions;
  for (collisions = SYMBOL_COLLISION_LIMIT; collisions; collisions -= 1) {
    index = (djb2Hash(index, symbol.label) & symfile->mask);

    Symbol_t *target = &(symfile->symbols[index]);
    if (!target->address) {
      *target = symbol;
      return target;
    }
  }

  return NULL;
}

static Symfile_t *growSymfile(Symfile_t *symfile) {
  /* Reallocates the symbols array in symfile by increasing the size of the mask
  by one bit. Returns dst on success. Returns NULL if fails. */
  unsigned int mask = (symfile->mask << 1) + 1;

  Symfile_t *dst = newSymfile(mask);
  if (dst && !tryEachSymbol(symfile, copySymbol, dst))
      dst = CloseSymfile(dst);

  return dst;
}

static Symfile_t *newSymfile(unsigned int mask) {
  /* Allocate and set the defaults for a new Symfile_t */
  Symfile_t *symfile = calloc(1, sizeof(symfile));

  if (symfile) {
    symfile->symbols = calloc(mask, sizeof(Symbol_t));

    if (!symfile->symbols) {
      free(symfile);
      return NULL;
    }

    symfile->mask = mask;
  }

  return symfile;
}

static unsigned int djb2Hash(unsigned int hash, const char *label) {
  /* Generate a unsigned int using the djb2 hashing algorithm */
  int c;
  while ((c = *label++)) hash = (hash << 5) + hash + c;

  return hash;
}

static void forEachSymbol(Symfile_t *symfile, symbolCallback_t callback, void *probe) {
  /* Iterate over each symbol in symfile and call callback on it. probe can be
  used to pass in any extra data. */
  unsigned int i;
  for (i = symfile->mask; i; i--) {
    Symbol_t *symbol = &symfile->symbols[i];
    callback(symfile, symbol, probe);
  }
}

/* public methods */

Symbol_t *GetSymbol(Symfile_t *symfile, char *label) {
  /* Returns the address of the Symbol_t at label. Returns NULL if label
  is not found. */
  unsigned int index = DJB2_MAGIC_NUMBER;

  int collisions;
  for (collisions = SYMBOL_COLLISION_LIMIT; collisions; collisions -= 1) {
    index = (djb2Hash(index, label) & symfile->mask);

    Symbol_t *target = &(symfile->symbols[index]);
    if (strcmp(label, target->label) == 0) {
      printf("%s found\n", label);
      return target;
    }
  }

  return NULL;
}

Symfile_t *CloseSymfile(Symfile_t *symfile) {
  /* Deallocates Symfile_t and its symbols. */
  if (symfile) {
    if (symfile->symbols) free(symfile->symbols);
    free(symfile);
  }

  return NULL;
}

Symfile_t *OpenSymfile(char *path) {
  /* OpenSymfile creates and returns new Symfile_t object by parsing the file at
   path. Returns NULL on failure. */
  Symfile_t *symfile = newSymfile(DEFAULT_SYMFILE_MASK);

  if (symfile) {
    char buffer[MAX_SYMFILE_LINE_LENGTH];

    FILE *stream = fopen(path, "r");

    while (fgets(buffer, sizeof(buffer), stream)) {
      Symbol_t symbol;
      int count = sscanf(buffer, "%02hhu:%04hX %256s\n", &symbol.bank, &symbol.address, symbol.label);

      if (count != 3) continue;
      printf("bank: %02hhu, address: 0x%04hX, label: %s\n", symbol.bank, symbol.address, symbol.label);

      while (!setSymbol(symfile, symbol)) {
        /* Symfile_t requires rehashing if we get too many collisions. */
        Symfile_t *new = growSymfile(symfile);
        if (!new) return NULL;

        CloseSymfile(symfile);
        symfile = new;
      }
    }

    fclose(stream);
  }

  return symfile;
}
