#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mort/cpu.h"

typedef struct {
  uint16_t address;
  uint8_t bank;
} Label;

typedef int (*MemoryGet)(uint16_t);
typedef int (*MemorySet)(uint16_t, uint8_t);

typedef struct {
  uint16_t start, stop;
  const char *tag;
  MemoryGet get;
  MemorySet set;
  struct {
    MemoryGet get;
    MemorySet set;
  } raw;
} MemoryRange;

enum {
  MEMORY_RANGE_ROM0,
  MEMORY_RANGE_ROMX,
  MEMORY_RANGE_VRAM,
  MEMORY_RANGE_SRAM,
  MEMORY_RANGE_WRAM0,
  MEMORY_RANGE_WRAMX,
  MEMORY_RANGE_ECHO,
  MEMORY_RANGE_OAM,
  MEMORY_RANGE_UNUSED,
  MEMORY_RANGE_IO_PORTS,
  MEMORY_RANGE_HRAM,
  MEMORY_RANGE_IE,
};

MemoryRange memory_ranges[] = {
  {
    .start = 0x0000,
    .stop = 0x3FFF,
  },
  {
    .start = 0x4000,
    .stop = 0x7FFF,
  },
  {
    .start = 0xA000,
    .stop = 0xBFFF,
  },
  {
    .start = 0xC000,
    .stop = 0xCFFF,
  },
  {
    .start = 0xD000,
    .stop = 0xDFFF,
  },
  {
    .start = 0xE000,
    .stop = 0xFDFF,
  },
  {
    .start = 0xFE00,
    .stop = 0xFE9F,
  },
  {
    .start = 0xFEA0,
    .stop = 0xFEFF,
  },
  {
    .start = 0xFEA0,
    .stop = 0xFEFF,
  },
  {
    .start = 0xFF00,
    .stop = 0xFF7F,
  },
  {
    .start = 0xFF80,
    .stop = 0xFFFE,
  },
  {
    .start = 0xFFFF,
    .stop = 0xFFFF,
  },
};

#define MEMORY_RANGES_END (memory_ranges + (sizeof(memory_ranges) / sizeof (MemoryRange)))

static inline MemoryRange *find(uint16_t address) {
  MemoryRange *range = memory_ranges;
  for (range = memory_ranges; range < (MEMORY_RANGES_END); range += 1) {
    if ((range->start <= address) && (address <= range->stop))
      break;
  }

  return range;
}

/* LINE_LENGTH: Max symbol length in rgbasm is set to 256, so this is
overshooting it a tad. */
#define LINE_LENGTH 512

void Symbols_ParseFile(char *path) {
  char buffer[LINE_LENGTH];

  FILE *stream = fopen(path, "r");

  while(fgets(buffer, sizeof(buffer), stream)) {
    char key[LINE_LENGTH];
    Label tmp;
    int count = sscanf(buffer, "%02hhu:%04hX %512s\n", &tmp.bank, &(tmp.address), key);

    if (count != 3) continue;
    printf("bank: %02hhu, address: 0x%04hX, label: %s\n", tmp.bank, tmp.address, key);
  }

  fclose(stream);
}

int main(int argc, char *argv[]) {
//    LR35902_t cpu;

//    cpu.registers.af = 0x3604;

//    printf("af: 0x%04X\n", cpu.registers.af);
//    printf("a: 0x%02X\n", cpu.registers.a);
//    printf("f: 0x%02X\n", cpu.registers.f);

//    cpu.registers.bc = 0x4477;

//    printf("af: 0x%04X\n", cpu.registers.bc);
//    printf("a: 0x%02X\n", cpu.registers.b);
//    printf("f: 0x%02X\n", cpu.registers.c);

    // for op at cpu.pc
//    Opcode op = GetOpcode(0x33);
//    int cycles;
//    bool jumped; // =

//    if (jumped) {
//        cycles += op.cycles.jumped;
        // continue;
//    }

//    cpu.registers.pc += op.bytes;
//    cycles += op.cycles.ignore;

    Symbols_ParseFile("../pulp/pulp.sym");
    find(0x4001);

//    op.instruction(&cpu, NULL);

    return 0;
}
