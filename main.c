#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//#include "mort/cpu.h"

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

typedef struct Rom_t {
  /* Rom_t is a container for the ROM data */
  char *data;
  long count;
} Rom_t;

typedef struct Wram_t {
  /* Wram_t is a container for the WRAM data */
  uint8_t bank0[8192], *bankx;
} Wram_t;

/* MAX_SYMFILE_LINE_LENGTH: Max symbol length in rgbasm is set to 256, so this
is overshooting it a tad. */
#define MAX_SYMFILE_LINE_LENGTH 512

/* MAX_SYMFILE_LINE_LENGTH: Max symbol length in rgbasm is set to 256 */
#define MAX_SYMFILE_SYMBOL_LENGTH 256

typedef struct Symbol_t {
  /* Symbol_t is a container for a parsed Symfile symbol */
  uint16_t address;
  uint8_t bank;
  char key[MAX_SYMFILE_SYMBOL_LENGTH];
} Symbol_t;

typedef struct Symfile_t {
  /* Symfile_t is a hash map of the parsed Symfile symbols */
  unsigned long mask;
  Symbol_t *symbols;
} Symfile_t;

/* DEFAULT_SYMFILE_MASK: To preserve space, a whole unsigned longs-worth of
Symbols is not allocated. We use mask as both the max and the bitwise mask
for Symbol indices. If we require more, we rehash. */
#define DEFAULT_SYMFILE_MASK 0xFF

Symfile_t *NewSymfile() {
  /* Allocate and set the defaults for a new Symfile_t */
  Symfile_t *symfile = calloc(1, sizeof(symfile));

  if (symfile) {
    symfile->symbols = calloc(DEFAULT_SYMFILE_MASK, sizeof(Symbol_t));

    if (!symfile->symbols) {
      free(symfile);
      return NULL;
    }

    symfile->mask = DEFAULT_SYMFILE_MASK;
  }

  return symfile;
}

typedef unsigned long Hash_t;


#define DJB2_MAGIC_NUMBER 5381

static Hash_t djb2Hash(Hash_t hash, const char *key) {
  /* Generate a hash using the djb2 hashing algorithm */
  int c;
  while ((c = *key++)) {
    hash = (hash << 5) + hash + c;
  }

  return hash;
}

/* djb2: Generate a hash using the djb2 hashing algorithm, using the
DJB2_MAGIC_NUMBER */
#define djb2(key) djb2Hash(DJB2_MAGIC_NUMBER, key)

#define SYMBOL_COLLISION_LIMIT 3

Symbol_t *SymfileSet(Symfile_t *symfile, Symbol_t symbol) {
  /* Set symbol at the correct index in symfile->symbols. Returns NULL on
  failure i.e. 3 collisions - if this happens we should rehash the symbols. */
  Hash_t index = DJB2_MAGIC_NUMBER;

  int collisions;
  for (collisions = SYMBOL_COLLISION_LIMIT; collisions; collisions -= 1) {
    puts(".");
    index = (djb2Hash(index, symbol.key) & symfile->mask);

    Symbol_t *target = &(symfile->symbols[index]);
    if (!target->address) {
      *target = symbol;
      return target;
    }
  }

  puts("3 collisions - not set");
  return NULL;
}

Symfile_t *OpenSymfile(char *path) {
  /* OpenSymfile creates and returns new Symfile_t object by parsing the file at
   path. Returns NULL on failure. */
  Symfile_t *symfile = NewSymfile();

  if (symfile) {
    char buffer[MAX_SYMFILE_LINE_LENGTH];

    FILE *stream = fopen(path, "r");

    while (fgets(buffer, sizeof(buffer), stream)) {
      Symbol_t symbol;
      int count = sscanf(buffer, "%02hhu:%04hX %256s\n", &symbol.bank, &symbol.address, symbol.key);

      if (count != 3) continue;
      printf("bank: %02hhu, address: 0x%04hX, label: %s\n", symbol.bank, symbol.address, symbol.key);

      SymfileSet(symfile, symbol);
    }

    fclose(stream);
  }

  return symfile;
}

typedef struct Mmu_t {
  /* Mmu_t is a container for the various other memory range containers */
  Rom_t *rom;
  Symfile_t *symfile;
  Wram_t wram;
} Mmu_t;

Rom_t *OpenRom(const char *path) {
  /* Opens a ROM file and puts it in the struct */
  FILE *stream = fopen(path, "rb");
  if (!stream) return NULL;

  Rom_t *rom = calloc(1, sizeof(Rom_t));
  if (rom) {
    fseek(stream, 0, SEEK_END);
    long count = ftell(stream);

    fseek(stream, 0, SEEK_SET);

    char *data = calloc(count + 1, sizeof(char));
    fread(data, sizeof(char), count, stream);

    fclose(stream);

    *rom =  (Rom_t) {
      .count = count,
      .data = data
    };
  }

  return rom;
}

Mmu_t *NewMmu(const char *path) {
  /* NewMmu allocates, initialises and returns a new Mmu_t. Returns NULL
  on failure. */

  Mmu_t *mmu = calloc(1, sizeof(Mmu_t));

  if (mmu) {
    mmu->rom = OpenRom(path);
    mmu->wram.bankx = &(mmu->wram.bank0[0x1000]);
  }

  return mmu;
}

Rom_t *CloseRom(Rom_t *rom) {
  /* Deallocates a Rom_t */
  if (rom->data) free(rom->data);
  free(rom);

  return NULL;
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

    OpenSymfile("../pulp/pulp.sym");
    Mmu_t *mmu = NewMmu("../pulp/pulp.gb");

    putchar(mmu->rom->data[0x113A]);
    putchar(mmu->rom->data[0x113B]);
    putchar(mmu->rom->data[0x113C]);
    putchar(mmu->rom->data[0x113D]);

    find(0x4001);

//    op.instruction(&cpu, NULL);

    return 0;
}
