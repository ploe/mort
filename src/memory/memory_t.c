
#include <stdlib.h>

#include "mort/memory/memory_t.h"

Memory_t *DestroyMemory(Memory_t *memory) {
  /* Deallocate the Memory_t */
  if (memory) {
    memory->mbc->rom = CloseRom(memory->mbc->rom);

    free(memory);
  }

  return NULL;
}

Memory_t *NewMemory(char *romfile) {
  /* NewMemory allocates, initialises and returns a new Memory_t. Returns NULL
  on failure. */

  Memory_t *memory = calloc(1, sizeof(Memory_t));

  if (memory) {
    memory->mbc = NewMbc(romfile);
    memory->wram.bankx = &(memory->wram.bank0[0x1000]);
  }

  return memory;
}
