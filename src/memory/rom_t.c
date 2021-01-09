#include <stdio.h>
#include <stdlib.h>

#include "mort/memory/memory_t.h"
#include "mort/memory/rom_t.h"

/* static methods - forward declarations */

static uint8_t rom0Get(void *, uint16_t);
static uint8_t romxGet(void *, uint16_t);
static void rom0Set(void *, uint16_t, uint8_t);
static void romxSet(void *, uint16_t, uint8_t);

/* constants */

enum {
  /* ROM MemoryRanges as constants */
  ROM0_START = 0x0000,
  ROM0_STOP = 0x3FFF,
  ROMX_START = 0x4000,
  ROMX_STOP = 0x7FFF,
};

MemoryRange_t rom_ranges[] = {
  /* get ranges */
  {
    .get = NULL,
    .set = NULL,
    .start = ROM0_START,
    .stop = ROM0_STOP,
  },
  {
    .get = NULL,
    .set = NULL,
    .start = ROMX_START,
    .stop = ROMX_STOP,
  },
};

/* static methods */

static uint8_t rom0Get(void *module, uint16_t address) {
  /* Return the value in rom0 at address */
  Rom_t *rom = (Rom_t *) module;

  return (uint8_t) rom->start[address];
}

static uint8_t romxGet(void *module, uint16_t address) {
  /* Return the value in romx at address */
  Rom_t *rom = (Rom_t *) module;
  address -= ROMX_START;

  return (uint8_t) rom->bank[address];

  return 0;
}

static void rom0Set(void *module, uint16_t address, uint8_t value) {
  return;
}

static void romxSet(void *module, uint16_t address, uint8_t value) {
  return;
}

/* public methods */

Rom_t *OpenRom(const char *path) {
  /* Opens a ROM file and puts it in the struct */
  FILE *stream = fopen(path, "rb");
  if (!stream) return NULL;

  Rom_t *rom = calloc(1, sizeof(Rom_t));
  if (rom) {
    fseek(stream, 0, SEEK_END);
    long count = ftell(stream);

    fseek(stream, 0, SEEK_SET);

    char *start = calloc(count + 1, sizeof(char));
    fread(start, sizeof(char), count, stream);

    fclose(stream);

    *rom =  (Rom_t) {
      .start = start,
      .bank = &start[ROMX_START],
      .count = count,
    };
  }

  return rom;
}

Rom_t *CloseRom(Rom_t *rom) {
  /* Deallocates a Rom_t */
  if (rom) {
    if (rom->start) free(rom->start);
    free(rom);
  }

  return NULL;
}

uint8_t RomGet(Memory_t *memory, uint16_t address) {
  const int length = ARRAY_LENGTH(rom_ranges);
  return MemoryRangeGet(memory, address, rom_ranges, length);
}
