
#include <stdlib.h>
#include <stdio.h>

#include "mort/memory/memory_t.h"

/* constants */

static MemoryRange_t memory_ranges[] = {
  {
    .get = MemoryRangeGetNotImplemented,
    .set = NULL,
    .start = 0x0000,
    .stop = 0x7FFF,
    .tag = "rom",
  },
  {
    .get = MemoryRangeGetNotImplemented,
    .set = NULL,
    .start = 0x8000,
    .stop = 0x9FFF,
    .tag = "vram",
  },
  {
    .get = MemoryRangeGetNotImplemented,
    .set = NULL,
    .start = 0xA000,
    .stop = 0xBFFF,
    .tag = "sram",
  },
  {
    .get = MemoryRangeGetNotImplemented,
    .set = NULL,
    .start = 0xC000,
    .stop = 0xDFFF,
    .tag = "wram",
  },
  {
    .get = MemoryRangeGetNotImplemented,
    .set = NULL,
    .start = 0xE000,
    .stop = 0xFDFF,
    .tag = "echo",
  },
  {
    .get = MemoryRangeGetNotImplemented,
    .set = NULL,
    .start = 0xFEA0,
    .stop = 0xFEFF,
    .tag = "unused",
  },
  {
    .get = MemoryRangeGetNotImplemented,
    .set = NULL,
    .start = 0xFF00,
    .stop = 0xFF7F,
    .tag = "io",
  },
  {
    .get = MemoryRangeGetNotImplemented,
    .set = NULL,
    .start = 0xFF80,
    .stop = 0xFFFE,
    .tag = "hram",
  },
  {
    .get = MemoryRangeGetNotImplemented,
    .set = NULL,
    .start = 0xFFFF,
    .stop = 0xFFFF,
    .tag = "ie",
  },
};

/* methods */

Memory_t *DestroyMemory(Memory_t *memory) {
  /* Deallocate the Memory_t */
  if (memory) {
    memory->rom = CloseRom(memory->rom);

    free(memory);
  }

  return NULL;
}

Memory_t *NewMemory(char *romfile) {
  /* NewMemory allocates, initialises and returns a new Memory_t. Returns NULL
  on failure. */

  Memory_t *memory = calloc(1, sizeof(Memory_t));

  if (memory) {
    memory->rom = OpenRom(romfile);
    memory->wram.bankx = &(memory->wram.bank0[0x1000]);
  }

  return memory;
}

uint8_t MemoryRangeGetNotImplemented(Memory_t *memory, uint16_t address) {
  fprintf(stderr, "MemoryGet: Address 0x%04hX not implemented.\n", address);
  return 0;
}

uint8_t MemoryRangeGet(Memory_t *memory, uint16_t address, MemoryRange_t *ranges, int length) {
  int i;
  for (i = 0; i < length; i++) {
    MemoryRange_t *range = &ranges[i];

    if ((address >= range->start) && (address <= range->stop))
      return range->get(memory, address);
  }

  return 0;
}

uint8_t MemoryGet(Memory_t *memory, uint16_t address) {
  const int length = ARRAY_LENGTH(memory_ranges);
  return MemoryRangeGet(memory, address, memory_ranges, length);
}
