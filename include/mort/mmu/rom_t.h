#ifndef _MORT_MMU_ROM_T_H
#define _MORT_MMU_ROM_T_H

/* interface */

typedef struct Rom_t {
  /* Rom_t is a container for the ROM data */
  char *start, *bank;
  long count;
} Rom_t;

typedef struct Mbc_t {
  /* Mbc_t is a container for the game pak data */
  Rom_t *rom;
} Mbc_t;

/* methods */

Mbc_t *NewMbc(const char *);
Rom_t *OpenRom(const char *);
Rom_t *CloseRom(Rom_t *);
uint8_t RomGet(Rom_t *, uint16_t);
void RomSet(uint16_t, uint8_t);

#endif
