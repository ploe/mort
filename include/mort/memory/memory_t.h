#ifndef _MORT_MMU_MMU_T_H
#define _MORT_MMU_MMU_T_H

#include <stdlib.h>

#include "mort/memory/mbc_t.h"
#include "mort/memory/wram_t.h"

/* types */

typedef uint8_t (*MemoryRangeGetter_t)(void *, uint16_t);
typedef void (*MemoryRangeSetter_t)(void *, uint16_t, uint8_t);

typedef struct {
  MemoryRangeGetter_t get;
  MemoryRangeSetter_t set;
  uint16_t start, stop;
} MemoryRange_t;

typedef struct Memory_t {
  /* Memory_t is a container for the various other memory range containers */
  Mbc_t *mbc;
  Wram_t wram;
} Memory_t;

/* methods */

Memory_t *NewMemory(char *);
Memory_t *DestroyMemory(Memory_t *);

#endif
