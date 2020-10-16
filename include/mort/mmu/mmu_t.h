#ifndef _MORT_MMU_MMU_T_H
#define _MORT_MMU_MMU_T_H

#include <stdlib.h>

#include "mort/mmu/rom_t.h"
#include "mort/mmu/wram_t.h"

/* types */

typedef uint8_t (*MemoryRangeGetter_t)(void *, uint16_t);
typedef void (*MemoryRangeSetter_t)(void *, uint16_t, uint8_t);

typedef struct {
  MemoryRangeGetter_t get;
  MemoryRangeSetter_t set;
  uint16_t start, stop;
} MemoryRange_t;

typedef struct Mmu_t {
  /* Mmu_t is a container for the various other memory range containers */
  Rom_t *rom;
  Wram_t wram;
} Mmu_t;

/* methods */

Mmu_t *NewMmu(char *);
Mmu_t *DestroyMmu(Mmu_t *);

#endif
