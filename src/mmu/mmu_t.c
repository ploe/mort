
#include <stdlib.h>

#include "mort/mmu/mmu_t.h"

uint8 MmuRead(uint16_t address) {
  
}

Mmu_t *DestroyMmu(Mmu_t *mmu) {
  /* Deallocate the Mmu_t */
  if (mmu) {
    mmu->rom = CloseRom(mmu->rom);
    mmu->symfile = CloseSymfile(mmu->symfile);

    free(mmu);
  }

  return NULL;
}

Mmu_t *NewMmu(char *romfile, char *symfile) {
  /* NewMmu allocates, initialises and returns a new Mmu_t. Returns NULL
  on failure. */

  Mmu_t *mmu = calloc(1, sizeof(Mmu_t));

  if (mmu) {
    mmu->rom = OpenRom(romfile);
    mmu->wram.bankx = &(mmu->wram.bank0[0x1000]);
    mmu->symfile = OpenSymfile(symfile);
  }

  return mmu;
}
