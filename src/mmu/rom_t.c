#include <stdio.h>
#include <stdlib.h>

#include "mort/mmu/rom_t.h"

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

Rom_t *CloseRom(Rom_t *rom) {
  /* Deallocates a Rom_t */
  if (rom->data) free(rom->data);
  free(rom);

  return NULL;
}
