#ifndef _MORT_MMU_SYMFILE_T_H
#define _MORT_MMU_SYMFILE_T_H

#include <stdlib.h>

/* constants */

/* DEFAULT_SYMFILE_MASK: To preserve space, a whole unsigned ints-worth of
Symbols is not allocated. We use mask as both the max and the bitwise mask
for Symbol indices. If we require more, we rehash. */
#define DEFAULT_SYMFILE_MASK 0xFF

/* MAX_SYMFILE_LINE_LENGTH: Max symbol length in rgbasm is set to 256, so this
is overshooting it a tad. */
#define MAX_SYMFILE_LINE_LENGTH 512

/* MAX_SYMFILE_LINE_LENGTH: Max symbol length in rgbasm is set to 256 */
#define MAX_SYMFILE_LABEL_LENGTH 256

/* types */

typedef struct Symbol_t {
  /* Symbol_t is a container for a parsed Symfile symbol */
  uint16_t address;
  uint8_t bank;
  char label[MAX_SYMFILE_LABEL_LENGTH];
} Symbol_t;

typedef struct Symfile_t {
  /* Symfile_t is a hash map of the parsed Symfile symbols */
  unsigned int mask;
  Symbol_t *symbols;
} Symfile_t;

/* methods */

Symbol_t *GetSymbol(Symfile_t *, char *);
Symfile_t *CloseSymfile(Symfile_t *);
Symfile_t *OpenSymfile(char *);

#endif
