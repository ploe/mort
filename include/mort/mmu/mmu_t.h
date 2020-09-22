#ifndef _MORT_MMU_MMU_T_H
#define _MORT_MMU_MMU_T_H

#include "mort/mmu/rom_t.h"
#include "mort/mmu/symfile_t.h"
#include "mort/mmu/wram_t.h"

/* interface */

typedef struct Mmu_t {
  /* Mmu_t is a container for the various other memory range containers */
  Rom_t *rom;
  Symfile_t *symfile;
  Wram_t wram;
} Mmu_t;

/* methods */

Mmu_t *NewMmu(const char *);

#endif
