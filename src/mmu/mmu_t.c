
#include <stdlib.h>

#include "mort/mmu/mmu_t.h"

Mmu_t *DestroyMmu(Mmu_t *mmu) {
  /* Deallocate the Mmu_t */
  if (mmu) {
    mmu->mbc->rom = CloseRom(mmu->mbc->rom);

    free(mmu);
  }

  return NULL;
}

Mmu_t *NewMmu(char *romfile) {
  /* NewMmu allocates, initialises and returns a new Mmu_t. Returns NULL
  on failure. */

  Mmu_t *mmu = calloc(1, sizeof(Mmu_t));

  if (mmu) {
    mmu->mbc = NewMbc(romfile);
    mmu->wram.bankx = &(mmu->wram.bank0[0x1000]);
  }

  return mmu;
}
