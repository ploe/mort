#include <stdio.h>
#include <stdlib.h>

#include "mort/mmu/mmu_t.h"
#include "mort/mmu/rom_t.h"

/* macros */

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

/* static methods - forward declarations */

static uint8_t rom0Get(void *, uint16_t);
static uint8_t romxGet(void *, uint16_t);
static void rom0Set(void *, uint16_t, uint8_t);
static void romxSet(void *, uint16_t, uint8_t);

/* constants */

enum {
  /* ROM Ranges as constants */
  ROM0_START = 0x0000,
  ROM0_STOP = 0x3FFF,
  ROMX_START = 0x4000,
  ROMX_STOP = 0x7FFF,
};

MemoryRange_t ROM_GET_RANGES[] = {
  /* get ranges */
  {
    .get = rom0Get,
    .set = rom0Set,
    .start = ROM0_START,
    .stop = ROM0_STOP,
  },
  {
    .get = romxGet,
    .set = romxSet,
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

Mbc_t *NewMbc(const char *path) {
  Mbc_t *mbc = calloc(1, sizeof(Mbc_t));
  if (mbc) {
    mbc->rom = OpenRom(path);
  }

  return mbc;
}

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
  if (rom->start) free(rom->start);
  free(rom);

  return NULL;
}

uint8_t RomGet(Rom_t *rom, uint16_t address) {
  int i;
  for (i = 0; i < ARRAY_LENGTH(ROM_GET_RANGES); i++) {
    MemoryRange_t *range = &(ROM_GET_RANGES[i]);

    if ((address >= range->start) && (address <= range->stop))
        return range->get(rom, address);
  }

  fprintf(stderr, "RomGet: address invalid '0x%04hX'\n", address);
  return 0;
}
