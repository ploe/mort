#ifndef _MORT_MMU_ROM_T_H
#define _MORT_MMU_ROM_T_H

/* interface */

typedef struct Rom_t {
  /* Rom_t is a container for the ROM data */
  char *bank0, *bankx;
  long count;
} Rom_t;

/* methods */

Rom_t *OpenRom(const char *);
Rom_t *CloseRom(Rom_t *);
uint8_t RomGet(Rom_t *, uint16_t);
void RomSet(uint16_t, uint8_t);

#endif
