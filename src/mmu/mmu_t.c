
#include <stdlib.h>

#include "mort/mmu/mmu_t.h"

Mmu_t *NewMmu(const char *path) {
  /* NewMmu allocates, initialises and returns a new Mmu_t. Returns NULL
  on failure. */

  Mmu_t *mmu = calloc(1, sizeof(Mmu_t));

  if (mmu) {
    mmu->rom = OpenRom(path);
    mmu->wram.bankx = &(mmu->wram.bank0[0x1000]);
    mmu->symfile = OpenSymfile("../pulp/pulp.sym");
  }

  return mmu;
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
