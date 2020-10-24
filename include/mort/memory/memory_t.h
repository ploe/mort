#ifndef _MORT_MMU_MMU_T_H
#define _MORT_MMU_MMU_T_H

#include <stdlib.h>

#include "mort/memory/rom_t.h"
#include "mort/memory/wram_t.h"

/* prototypes */

struct _Memory_t;
typedef struct _Memory_t Memory_t;

/* types */

struct _Memory_t {
  /* Memory_t is a container for the various other memory range containers */
  Rom_t *rom;
  Wram_t wram;
};

typedef uint8_t (*MemoryRangeGetter_t)(Memory_t *, uint16_t);
typedef void (*MemoryRangeSetter_t)(void *, uint16_t, uint8_t);

typedef struct {
  char *tag;
  MemoryRangeGetter_t get;
  MemoryRangeSetter_t set;
  uint16_t start, stop;
} MemoryRange_t;

/* macros */

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

/* methods */

uint8_t MemoryRangeGet(Memory_t *, uint16_t, MemoryRange_t *, int);
uint8_t MemoryRangeGetNotImplemented(Memory_t *, uint16_t);

Memory_t *DestroyMemory(Memory_t *);
Memory_t *NewMemory(char *);
uint8_t MemoryGet(Memory_t *memory, uint16_t address);


#endif
